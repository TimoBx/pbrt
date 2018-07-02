// This file is part of Gratin, a programmable Node-based System
// for GPU-friendly Applications.
//
// Copyright (C) 2013-2014 Romain Vergne <romain.vergne@inria.fr>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "mvgLoader.h"
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <stack>
#include <queue>

#include "misc/extinclude.h"
#include "misc/glutils.h"
#include "core/pbgraph.h"
#include "misc/mesh.h"
#include "misc/objLoader.h"

using namespace std;
using namespace Vitelotte;


QDir MVGLoaderWidget::_currentPath = QDir::currentPath();


bool MVGLoaderWidget::showLoadError(std::string& msg, unsigned line,
                                    MVGLoaderWidget* loader) {
  QMessageBox::warning(loader, "Failed to load MVG",
    QString("An error occured while loading \"%1\", line %2:\n\n%3\n")
      .arg("TODO").arg(line).arg(QString::fromStdString(msg)));
  return false;
}

MVGLoaderWidget::MVGLoaderWidget(MVGLoaderNode *node)
  : GenericCustomWidget(node),
    _load(new QPushButton(tr("Load..."))),
    _default(new QPushButton(tr("Reset cam"))) {

  _wireframe = new QCheckBox(tr("Show wireframe"));
  _wireframe->setChecked(node->showWireframe());
  _camWidget = new TrackballCameraWidget(node,tr("Camera"),node->camera());

  // default parameters
  userLayout()->addWidget(_load);
  userLayout()->addWidget(_wireframe);
  userLayout()->addWidget(_default);
  userLayout()->addWidget(_camWidget);

  connect(_load,SIGNAL(clicked()),this,SLOT(loadClicked()));
  connect(_wireframe,SIGNAL(clicked(bool)),this,SLOT(setWireframe(bool)));
  connect(_default,SIGNAL(clicked()),this,SLOT(defaultClicked()));
}

void MVGLoaderWidget::loadClicked() {
  QString filename = QFileDialog::getOpenFileName(
	  0, "Load mvg", _currentPath.absolutePath(),
	  "Mesh-based vector graphics (*.mvg);;All (*.*)");

  if(filename.isEmpty()) {
    return;
  }

  QDir d(".");
  _currentPath = d.filePath(filename);

  ((MVGLoaderNode *)node())->loadObject(filename, this);
  updateGraph();
}

void MVGLoaderWidget::defaultClicked() {
  ((MVGLoaderNode *)node())->initCam();
  updateGraph();
}

void MVGLoaderWidget::setWireframe(bool on) {
  ((MVGLoaderNode *)node())->setShowWireframe(on);
  updateGraph();
}

//---------------------------------------------------------------------------//

MVGLoaderNode::MVGLoaderNode(PbGraph *parent,NodeHandle *handle)
  : GenericCustomNode(true,true,false,true,parent,handle,true,false,Vector2f(0.0f,0.0f),Vector2f(512.0f,512.0f)),
    _filename(GRATIN_APPLI_DATA_PATH+"/mvgs/sample.mvg"),
    _camera(new TrackballCamera(Vector2i(512,512),Vector3f(0,0,0),1.0f,0)),
    _vao(NULL),
    _depth(NULL),
    _nodesBuffer(0),
    _nodesTexture(0),
    _w(NULL),
    _nTriangles(0),
    _nSingulars(0),
    _smoothness(8),
    _is3d(false),
    _isQuadratic(false),
    _showWireframe(false) {

  _w = new MVGLoaderWidget(this);

  initShaderSource();
  loadObject(_filename);

  _w->updateWidget();
}

MVGLoaderNode::~MVGLoaderNode() {
  _glf->glDeleteTextures(1, &_nodesTexture);
  _glf->glDeleteBuffers(1, &_nodesBuffer);
  delete _vao;
  delete _camera;
  delete _depth;
}

void MVGLoaderNode::apply() {
  CHECK_GL_ERROR
  Glutils::setViewport(outputTex(0)->w(),outputTex(0)->h());
  setOutputParams();

  _fbo.bind();
  _glf->glDrawBuffers(nbOutputs(),buffersOfOutputTex(0));

  initOpenGLState();

  enableShaders();
  Matrix4f mdv = _camera->viewMatrix() * _camera->modelMatrix();
  _p->setUniformMatrix4fv("viewMatrix",(GLfloat *)mdv.data());
  _p->setUniformMatrix4fv("projMatrix",(GLfloat *)_camera->projMatrix().data());
  _p->setUniform1f("zmin",_camera->zmin());
  _p->setUniform1f("zmax",_camera->zmax());
  _p->setUniform2f("viewportSize",outputTex(0)->w(),outputTex(0)->h());
  _p->setUniform1i("nodes",0);
  _p->setUniform1f("smoothness",_smoothness);
  _p->setUniform1i("enableShading",_is3d);
  _p->setUniform1i("showWireframe",_showWireframe);

  _vao->bind();

  _glf->glPatchParameteri(GL_PATCH_VERTICES, 9);

  _glf->glActiveTexture(GL_TEXTURE0);
  _glf->glBindTexture(GL_TEXTURE_BUFFER, _nodesTexture);

  _p->setUniform1i("baseNodeIndex",0);
  _p->setUniform1i("singularTriangles",false);
  _vao->drawElements(GL_PATCHES, _nTriangles * 9);

  int nodePerTriangle = _isQuadratic? 6: 3;
  _p->setUniform1i("baseNodeIndex",_nTriangles * nodePerTriangle);
  _p->setUniform1i("singularTriangles",true);
  _vao->drawElements(GL_PATCHES, _nSingulars * 9, _nTriangles * 9 * sizeof(unsigned));

  _glf->glBindTexture(GL_TEXTURE_BUFFER, 0);

  _vao->unbind();
  FramebufferObject::unbind();

  disableShaders();
  cleanOpenGLState();
  CHECK_GL_ERROR
}

void MVGLoaderNode::loadObject(const QString &filename, MVGLoaderWidget *widget) {
  QString f = filename;
  QDir d(".");
  f = d.absoluteFilePath(f);

  typedef DCMesh<float, Vitelotte::Dynamic, Vitelotte::Dynamic> Mesh;
  typedef Vitelotte::MVGWithCurvesReader<Mesh> Reader;
  Mesh mesh;
  Reader reader;
  if(widget) {
    reader.setErrorCallback(
        (Reader::ErrorCallback)MVGLoaderWidget::showLoadError,
        PatateCommon::defaultWarningCallback,
        widget);
  }
  std::ifstream in(filename.toLocal8Bit());
  if(!reader.read(in, mesh)) {
    return;
  }


  mesh.PatateCommon::SurfaceMesh::triangulate();
  mesh.setAttributes(Mesh::FV_FLAGS);
  if(mesh.nCurves() || mesh.nPointConstraints()) {
    mesh.setNodesFromCurves();
  }
  if(!mesh.nNodes() || mesh.nUnknowns() == mesh.nNodes()) {
    QMessageBox::warning(widget, "Failed to load MVG",
      QString("Can not load \"%1\":\n\n"
              "This MVG file has no constrained nodes. Solve impossible.\n")
              .arg(filename));
    return;
  }

  mesh.finalize();

  typedef FVElementBuilder<Mesh, double> FVElemBuilder;
  typedef SingularElementDecorator<FVElemBuilder> ElemBuilder;
  typedef FemSolver<Mesh, ElemBuilder> Solver;
  Solver solver;

  solver.build(mesh);
  solver.solve(mesh);

  if(solver.error().status() == SolverError::STATUS_ERROR) {
    QMessageBox::warning(widget, "Failed to load MVG",
      QString("Can not load \"%1\":\n\n"
              "Diffusion failed: %2.\n")
              .arg(filename).arg(QString::fromStdString(solver.error().message())));
    return;
  }

  _filename = f;

  AlignedBox3f bb;
  for(Mesh::VertexIterator vit = mesh.verticesBegin();
      vit != mesh.verticesEnd(); ++vit) {
    bb.extend(toPosition(mesh.position(*vit)));
  }

  // init camera
  _camera->setSceneParams(bb.center(),bb.diagonal().norm() / 2.f);
  initCam();


  //-----------------------------------------------------------------------//
  // Do the dirty work
  _isQuadratic = mesh.hasEdgeValue();
  _is3d        = mesh.nDims() == 3;
  //Vitelotte::ColorSpace m_meshColorSpace = mesh.colorSpace();

  int nodePerTriangle = _isQuadratic? 6: 3;

  // Compute number of singular and normal triangles
  _nSingulars = mesh.nSingularFaces();
  _nTriangles = mesh.nFaces() - _nSingulars;

  if(_nSingulars + _nTriangles == 0) {
    return;
  }

  // Reserve buffers.
  typedef std::vector<Vector3f> Vec3List;
  typedef std::vector<unsigned> IndexList;
  typedef std::vector<Vector4f> ColorList;
  Vec3List vertices;
  Vec3List normals;
  IndexList indices;
  ColorList nodes;
  vertices.resize(mesh.verticesSize() + 2 * mesh.edgesSize());
  normals.resize(mesh.verticesSize() + 2 * mesh.edgesSize());
  indices.resize((_nTriangles * 3 + _nSingulars * 3) * 3);
  nodes.resize(_nTriangles * nodePerTriangle +
	   _nSingulars * (nodePerTriangle + 1));

  // Push vertices positions
  for(typename Mesh::VertexIterator vit = mesh.verticesBegin();
      vit != mesh.verticesEnd(); ++vit)
  {
    vertices[(*vit).idx()] = toPosition(mesh.position(*vit));
    normals[(*vit).idx()].setZero();
  }

  // Subdivide curved edges
  unsigned nVertices = mesh.verticesSize();
  for(typename Mesh::EdgeIterator eit = mesh.edgesBegin();
      eit != mesh.edgesEnd(); ++eit)
  {
    unsigned i1 = nVertices + 2 * (*eit).idx();
    unsigned i2 = i1 + 1;

    typename Mesh::Halfedge h = mesh.halfedge(*eit, 0);
    bool orient = mesh.halfedgeOrientation(h);
    const Vector3f& v0 = toPosition(mesh.position(orient? mesh.toVertex(h): mesh.fromVertex(h)));
    const Vector3f& v3 = toPosition(mesh.position(orient? mesh.fromVertex(h): mesh.toVertex(h)));
    if(!mesh.isCurved(*eit) || mesh.edgeCurve(*eit).type() == BEZIER_LINEAR) {
      vertices[i1] = (2 * v0 + v3) / 3;
      vertices[i2] = (v0 + 2 * v3) / 3;
    } else if(mesh.edgeCurve(*eit).type() == BEZIER_QUADRATIC) {
      vertices[i1] = (v0 + 2 * toPosition(mesh.edgeCurve(*eit).point(1))) / 3;
      vertices[i2] = (2 * toPosition(mesh.edgeCurve(*eit).point(1)) + v3) / 3;
    } else {
      vertices[i1] = toPosition(mesh.edgeCurve(*eit).point(1));
      vertices[i2] = toPosition(mesh.edgeCurve(*eit).point(2));
    }
    // Normals are interpolated later.
  }

  // Push faces indices and nodes
  unsigned triIndex = 0;
  unsigned singIndex = _nTriangles * 3;
  unsigned triNodeIndex = 0;
  unsigned singNodeIndex = _nTriangles * nodePerTriangle;
  for(typename Mesh::FaceIterator fit = mesh.facesBegin();
      fit != mesh.facesEnd(); ++fit)
  {
    // Ensure we work with triangles
    assert(mesh.valence(*fit) == 3);
    typename Mesh::Halfedge h = mesh.halfedge(*fit);

    bool isSingular = mesh.nSingulars(*fit);
    if(isSingular)
    {
      // The first vertex must be the singular one
      while(!mesh.isSingular(h)) { h = mesh.nextHalfedge(h); }
    }

    unsigned& index = isSingular? singIndex: triIndex;
    unsigned& nodeIndex = isSingular? singNodeIndex: triNodeIndex;
    Vector3f pts[3];
    // Push vertices nodes
    for(int ei = 0; ei < 3; ++ei)
    {
      pts[ei] = toPosition(mesh.position(mesh.toVertex(h)));
      indices[3 * (index + ei)] = mesh.toVertex(h).idx();
      h = mesh.nextHalfedge(h);
      indices[3 * (index + ei) + 1] = nVertices + 2 * mesh.edge(h).idx()
      + mesh.halfedgeOrientation(h);
      indices[3 * (index + ei) + 2] = nVertices + 2 * mesh.edge(h).idx()
      + 1 - mesh.halfedgeOrientation(h);
      nodes[nodeIndex + ei] = mesh.hasToVertexValue()?
	toColor(mesh.value(mesh.fromVertexValueNode(h))):
	Eigen::Vector4f(.8, .8, .8, 1.);
    }
    // Singular node is the last one
    if(isSingular)
      nodes[nodeIndex + nodePerTriangle] = toColor(mesh.value(mesh.toVertexValueNode(h)));

    if(_isQuadratic)
    {
      // Push edge nodes
      h = mesh.prevHalfedge(h);
      for(int ei = 0; ei < 3; ++ei)
      {
    nodes[nodeIndex + 3 + ei] = toColor(mesh.value(mesh.edgeValueNode(h)));
    h = mesh.nextHalfedge(h);
      }
    }

    // Compute normals
    Vector3f fn = (pts[1] - pts[0]).cross(pts[2] - pts[0]);
    for(int ei = 0; ei < 3; ++ei) {
      normals[mesh.toVertex(h).idx()] += fn;
      h = mesh.nextHalfedge(h);
    }

    index += 3;
    nodeIndex += nodePerTriangle + isSingular;
  }

  // Normalize normals
  for(typename Mesh::VertexIterator vit = mesh.verticesBegin();
      vit != mesh.verticesEnd(); ++vit) {
    normals[(*vit).idx()].normalize();
  }

  if(!_nodesBuffer) {
    _glf->glGenBuffers(1, &_nodesBuffer);
  }
  _glf->glBindBuffer(GL_ARRAY_BUFFER, _nodesBuffer);
  _glf->glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4f) * nodes.size(), nodes.data(),
	     GL_STATIC_DRAW);

  // Create and setup texture buffer
  if(!_nodesTexture)
  {
    _glf->glGenTextures(1, &_nodesTexture);
  }
  _glf->glBindTexture(GL_TEXTURE_BUFFER, _nodesTexture);
  _glf->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, _nodesBuffer);

  // init VAO
  delete _vao;
  _vao = new VertexarrayObject();
  _vao->addAttrib(vertices.size() * sizeof(Vector3f), vertices.data(), 3);
  _vao->addAttrib(vertices.size() * sizeof(Vector3f), normals.data(), 3);
  _vao->setIndices(indices.size() * sizeof(unsigned), indices.data());

  CHECK_GL_ERROR
}

Vector3f MVGLoaderNode::toPosition(const Eigen::VectorXf& v) const {
  switch(v.rows()) {
    case 0:
    case 1:
      return Vector3f::Zero();
    case 2: return (Vector3f() << v, 0).finished();
    default: return v.head<3>();
  }
  return Vector3f::Zero();
}

Vector4f MVGLoaderNode::toColor(const Eigen::VectorXf& v) const {
  switch(v.rows()) {
    case 0: return Vector4f::Zero();
    case 1: return Vector4f(v(0), v(0), v(0), 1);
    case 2: return Vector4f(v(0), v(0), v(0), v(1));
    case 3: return (Vector4f() << v, 1).finished();
    default: return v.head<4>();
  }
  return Vector4f::Zero();
}

void MVGLoaderNode::initCam() {
  Vector2i size(outputSize()[0],outputSize()[1]);
  _camera->reinit(size,_camera->sceneCenter(),_camera->sceneRadius());
}

void MVGLoaderNode::setShowWireframe(bool on) {
  _showWireframe = on;
}

void MVGLoaderNode::mousePressEvent(const Vector2f &p,QMouseEvent *me) {
  if(me->button()==Qt::LeftButton) {
    _camera->initRotation(p);
  } else if(me->button()==Qt::RightButton) {
    _camera->initMoveXY(p);
  } else if(me->button()==Qt::MidButton) {
    _camera->initMoveZ(p);
  }

  GenericCustomNode::mousePressEvent(p,me);
}

void MVGLoaderNode::mouseMoveEvent(const Vector2f &p,QMouseEvent *me) {
  _camera->move(p);
  GenericCustomNode::mouseMoveEvent(p,me);
}

void MVGLoaderNode::wheelEvent(const Vector2f &p,QWheelEvent *we) {
    GenericCustomNode::wheelEvent(p,we);

    const Vector2f v(0.0f,60.0f);

    _camera->initMoveZ(p);
    if(we->delta()>0) {
        _camera->move(p+v);
    } else {
        _camera->move(p-v);
    }

    update();
}

void MVGLoaderNode::initFBO() {
  NodeTexture2D::initFBO();

  _depth = new FloatTexture2D(TextureFormat(GL_TEXTURE_2D,outputSize()[0],outputSize()[1],
  					    GL_DEPTH_COMPONENT24,GL_DEPTH_COMPONENT,GL_FLOAT),
  			      TextureParams(GL_NEAREST,GL_NEAREST));

  _fbo.bind();
  _fbo.attachTexture(GL_TEXTURE_2D,_depth->id(),GL_DEPTH_ATTACHMENT);
  _fbo.isValid();

  FramebufferObject::unbind();
}

void MVGLoaderNode::cleanFBO() {
  NodeTexture2D::cleanFBO();
  delete _depth; _depth = NULL;
}

const QString MVGLoaderNode::constantVertHead()  {
  return QObject::tr("layout(location = 0) in vec3 inVertex;\n"
	     "layout(location = 1) in vec3 inNormal;\n");
}

const QString MVGLoaderNode::defaultVertBody()   {
  return QObject::tr("out vec4 ctrl_position_obj;\n"
	     "out vec3 ctrl_normal_obj;\n"
	     "out vec3 ctrl_normal_view;\n"
	     "out float ctrl_depth;\n"
	     "\n"
	     "void main(void)\n"
	     "{\n"
	     "  gl_Position       = projMatrix * viewMatrix * vec4(inVertex, 1.);\n"
	     "  ctrl_position_obj = vec4(inVertex, 1.);\n"
	     "  ctrl_normal_obj   = inNormal;\n"
	     "  ctrl_normal_view  = (projMatrix * viewMatrix * vec4(inNormal, 0)).xyz;\n"
	     "  ctrl_depth        = (clamp(-(viewMatrix*vec4(inVertex,1)).z,zmin,zmax)-zmin)/(zmax-zmin);\n"
	     "}\n");
}

const QString MVGLoaderNode::constantFragHead()  {
  return QString();
}

const QString MVGLoaderNode::defaultFragBody()   {
  return QObject::tr("in float frag_depth;\n"
	     "in vec3 frag_linearBasis;\n"
	     "in vec2 position;\n"
	     "flat in vec2 vertices[3];\n"
	     "flat in vec2 normEdges[3];\n"
	     "flat in int frag_index;\n"
	     "in vec4 frag_position_obj;\n"
	     "in vec3 frag_normal_obj;\n"
	     "in vec3 frag_normal_view;\n"
	     "noperspective in vec3 frag_edgeDist_scr;\n"
	     "flat in vec3 frag_vertices_obj[3];\n"
	     "flat in vec3 frag_normEdges_obj[3];\n"
	     "\n"
	     "\n"
	     "// Inverse radial linear interpolation.\n"
	     "// Return angle(v1, vx) / angle(v1, v2)\n"
	     "float irlerp(in vec3 vx, in vec3 v1, in vec3 v2)\n"
	     "{\n"
	     "  float alpha = acos(clamp(dot(v1, vx), -1., 1.));\n"
	     "  float beta = acos(clamp(dot(v1, v2), -1., 1.));\n"
	     "  return alpha / beta;\n"
	     "}\n"
	     "\n"
	     "\n"
	     "// Quadratic color interpolation over a triangle.\n"
	     "vec4 quadraticInterp(in vec4 colors[6])\n"
	     "{\n"
	     "  return\n"
	     "      colors[0] * frag_linearBasis.x * (2. * frag_linearBasis.x - 1.) +\n"
	     "      colors[1] * frag_linearBasis.y * (2. * frag_linearBasis.y - 1.) +\n"
	     "      colors[2] * frag_linearBasis.z * (2. * frag_linearBasis.z - 1.) +\n"
	     "      colors[3] * 4. * frag_linearBasis.y * frag_linearBasis.z +\n"
	     "      colors[4] * 4. * frag_linearBasis.z * frag_linearBasis.x +\n"
	     "      colors[5] * 4. * frag_linearBasis.x * frag_linearBasis.y;\n"
	     "}\n"
	     "\n"
	     "// Index of the first vertex node in `nodes` texture\n"
	     "int baseVxIndex = baseNodeIndex + frag_index * (6 + int(singularTriangles));\n"
	     "// Index of the first edge node in `nodes` texture\n"
	     "int baseEdgeIndex = baseVxIndex + 3;\n"
	     "\n"
	     "void main(void)\n"
	     "{\n"
	     "  vec4 colorNodes[] = vec4[6](\n"
	     "    texelFetch(nodes, baseVxIndex + 0),\n"
	     "    texelFetch(nodes, baseVxIndex + 1),\n"
	     "    texelFetch(nodes, baseVxIndex + 2),\n"
	     "    texelFetch(nodes, baseEdgeIndex + 0),\n"
	     "    texelFetch(nodes, baseEdgeIndex + 1),\n"
	     "    texelFetch(nodes, baseEdgeIndex + 2)\n"
	     "  );\n"
	     "\n"
	     "  // Singular nodes do radial interpolation between the two colors of the\n"
	     "  // singular vertex (which is always the first one).\n"
	     "  if(singularTriangles)\n"
	     "  {\n"
	     "      colorNodes[0] = mix(\n"
	     "          colorNodes[0],\n"
	     "          texelFetch(nodes, baseVxIndex + 6),\n"
	     "          irlerp(normalize(frag_position_obj.xyz - frag_vertices_obj[0]),\n"
	     "                 frag_normEdges_obj[2], -frag_normEdges_obj[1]));\n"
	     "  }\n"
	     "\n"
	     "  color = quadraticInterp(colorNodes);\n"
	     "  if(showWireframe)\n"
	     "  {\n"
	     "    float closestDist = min(min(frag_edgeDist_scr[0], frag_edgeDist_scr[1]), frag_edgeDist_scr[2]);\n"
	     "    color = mix(color, vec4(0, 0, 0, 1), clamp(1 - closestDist, 0, 1));\n"
	     "  }\n"
	     "\n"
	     "  normal = vec4(normalize(frag_normal_view), frag_depth);\n"
	     "}\n");
}

const QString MVGLoaderNode::constantTessCHead() {
  return QObject::tr("layout(vertices = 9) out;\n");
}

const QString MVGLoaderNode::defaultTessCBody()  {
return QObject::tr("in vec4 ctrl_position_obj[];\n"
	   "in vec3 ctrl_normal_obj[];\n"
	   "in vec3 ctrl_normal_view[];\n"
	   "\n"
	   "out vec4 eval_position_obj[];\n"
	   "out vec3 eval_normal_obj[];\n"
	   "out vec3 eval_normal_view[];\n"
	   "out ScreenSpaceBlock {\n"
	   "  flat vec3 heights_scr;\n"
	   "  flat vec3 vertices_obj[3];\n"
	   "  flat vec3 normEdges_obj[3];\n"
	   "} eval_ss[];\n"
	   "\n"
	   "// Test if the edge i of the patch is linear and uniform.\n"
	   "bool isLinear(int i) {\n"
	   "  vec4 p0 = eval_position_obj[3*i + 0];\n"
	   "  vec4 p1 = eval_position_obj[3*i + 1];\n"
	   "  vec4 p2 = eval_position_obj[3*i + 2];\n"
	   "  vec4 p3 = eval_position_obj[(3*i + 3)%9];\n"
	   "\n"
	   "  vec4 p1off = p1 - (2*p0 + p3) / 3;\n"
	   "  vec4 p2off = p2 - (p0 + 2*p3) / 3;\n"
	   "\n"
	   "  return dot(p1off, p1off) < 0.0001\n"
	   "      && dot(p2off, p2off) < 0.0001;\n"
	   "}\n"
	   "\n"
	   "void main(void)\n"
	   "{\n"
	   "  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
	   "  eval_position_obj[gl_InvocationID]  = ctrl_position_obj[gl_InvocationID];\n"
	   "  eval_normal_obj[gl_InvocationID]    = ctrl_normal_obj[gl_InvocationID];\n"
	   "  eval_normal_view[gl_InvocationID]   = ctrl_normal_view[gl_InvocationID];\n"
	   "\n"
	   "  // Compute corner vertices position in window space\n"
	   "  // Do not work with vertices behind the camera\n"
	   "  vec2 position_scr[3];\n"
	   "  for(int i=0; i<3; ++i)\n"
	   "  {\n"
	   "    position_scr[i] = (viewportSize * gl_in[i*3].gl_Position.xy)\n"
	   "                    / (2.0 * gl_in[i*3].gl_Position.w);\n"
	   "  }\n"
	   "  float area = abs(cross(vec3(position_scr[1] - position_scr[0], 0.0),\n"
	   "                         vec3(position_scr[2] - position_scr[0], 0.0)).z);\n"
	   "\n"
	   "  for(int i=0; i<3; ++i)\n"
	   "  {\n"
	   "    eval_ss[gl_InvocationID].heights_scr[i]\n"
	   "            = area / length(position_scr[(i+2)%3] - position_scr[(i+1)%3]);\n"
	   "    eval_ss[gl_InvocationID].vertices_obj[i]\n"
	   "            = ctrl_position_obj[i*3].xyz;\n"
	   "    eval_ss[gl_InvocationID].normEdges_obj[i]\n"
	   "            = normalize(ctrl_position_obj[((i+2)%3)*3].xyz - ctrl_position_obj[((i+1)%3)*3].xyz);\n"
	   "  }\n"
	   "\n"
	   "  bvec3 isEdgeLinear = bvec3(isLinear(0), isLinear(1), isLinear(2));\n"
	   "  gl_TessLevelOuter = float[4](\n"
	   "              isEdgeLinear[1]? 1: smoothness,\n"
	   "              isEdgeLinear[2]? 1: smoothness,\n"
	   "              isEdgeLinear[0]? 1: smoothness,\n"
	   "              1);\n"
	   "  gl_TessLevelInner = float[2](all(isEdgeLinear)? 1: smoothness, 1);\n"
	   "}\n");
}

const QString MVGLoaderNode::constantTessEHead() {
  return QString("layout(triangles) in;\n");
}

const QString MVGLoaderNode::defaultTessEBody()  {
  return QObject::tr("in vec4 eval_position_obj[];\n"
	     "in vec3 eval_normal_obj[];\n"
	     "in vec3 eval_normal_view[];\n"
	     "in ScreenSpaceBlock {\n"
	     "  flat vec3 heights_scr;\n"
	     "  flat vec3 vertices_obj[3];\n"
	     "  flat vec3 normEdges_obj[3];\n"
	     "} eval_ss[];\n"
	     "\n"
	     "flat out int frag_index;\n"
	     "out float frag_depth;\n"
	     "out vec3 frag_linearBasis;\n"
	     "out vec4 frag_position_obj;\n"
	     "out vec3 frag_normal_obj;\n"
	     "out vec3 frag_normal_view;\n"
	     "noperspective out vec3 frag_edgeDist_scr;\n"
	     "flat out vec3 frag_vertices_obj[3];\n"
	     "flat out vec3 frag_normEdges_obj[3];\n"
	     "\n"
	     "void main(void)\n"
	     "{\n"
	     "  // Evaluate a cubic Bezier patch.\n"
	     "  vec3 c1 = gl_TessCoord;\n"
	     "  vec3 c2 = c1 * c1;\n"
	     "  vec3 c3 = c2 * c1;\n"
	     "  vec4 mid = vec4(0);\n"
	     "  for(int i = 0; i < 9; ++i) mid += eval_position_obj[i];\n"
	     "  mid /= 9;  // Midpoint is the average position of other points. Might gives weird results in 3D.\n"
	     "  frag_position_obj = eval_position_obj[0] * c3[0]\n"
	     "                    + eval_position_obj[1] * c2[0] * c1[1] * 3\n"
	     "                    + eval_position_obj[2] * c1[0] * c2[1] * 3\n"
	     "                    + eval_position_obj[3] * c3[1]\n"
	     "                    + eval_position_obj[4] * c2[1] * c1[2] * 3\n"
	     "                    + eval_position_obj[5] * c1[1] * c2[2] * 3\n"
	     "                    + eval_position_obj[6] * c3[2]\n"
	     "                    + eval_position_obj[7] * c2[2] * c1[0] * 3\n"
	     "                    + eval_position_obj[8] * c1[2] * c2[0] * 3\n"
	     "                    + mid                  * c1[0] * c1[1] * c1[2] * 6;\n"
	     "\n"
	     "  gl_Position = projMatrix * (viewMatrix * frag_position_obj);\n"
	     "  frag_depth         = (clamp(-(viewMatrix*frag_position_obj).z,zmin,zmax)-zmin)/(zmax-zmin);\n"
	     "  frag_index         = gl_PrimitiveID;  // Used to compute node indice in fs.\n"
	     "  frag_linearBasis   = gl_TessCoord;    // Barycentric coordinate in the patch.\n"
	     "  frag_normal_obj    = mat3(eval_normal_obj[0],\n"
	     "                            eval_normal_obj[3],\n"
	     "                            eval_normal_obj[6]) * gl_TessCoord;\n"
	     "  frag_normal_view   = mat3(eval_normal_view[0],\n"
	     "                            eval_normal_view[3],\n"
	     "                            eval_normal_view[6]) * gl_TessCoord;\n"
	     "  // Approximate distance with respect to each edges.\n"
	     "  frag_edgeDist_scr  = eval_ss[0].heights_scr * gl_TessCoord;\n"
	     "  frag_vertices_obj  = eval_ss[0].vertices_obj;\n"
	     "  frag_normEdges_obj = eval_ss[0].normEdges_obj;\n"
	     "}\n");
}

const QString MVGLoaderNode::constantGeomHead()  {
  return QObject::tr("layout(triangles) in;\n");
}

const QString MVGLoaderNode::defaultGeomBody()   {
    return QObject::tr("layout(triangle_strip, max_vertices = 3) out;\n\n"
               "void main() {\n"
               "\tgl_Position = gl_in[0].gl_Position; EmitVertex();\n"
               "\tgl_Position = gl_in[1].gl_Position; EmitVertex();\n"
               "\tgl_Position = gl_in[2].gl_Position; EmitVertex();\n"
               "\tEndPrimitive();\n"
               "}\n");
}

bool MVGLoaderNode::save(SceneSaver  *saver) {
  bool ret = true;

  // obj file and camera 
  QString name = saver->absoluteToRelativePath(_filename);
  if(!saver->saveString("filename",name))      ret = false;
  if(!saver->saveIOData("camera",_camera))     ret = false;
  if(!saver->saveBool("showWireframe",_showWireframe)) ret = false;
  if(!GenericCustomNode::save(saver))          ret = false;

  return ret;
}

bool MVGLoaderNode::load(SceneLoader *loader) {
  bool ret = true;

  _filename = GRATIN_APPLI_DATA_PATH+"/mvgs/sample.mvg";
  QString name;
  if(!loader->loadString("filename",name)) ret = false;

  if(ret) {
    _filename = loader->relativeToAbsolutePath(name);
  }

  loadObject(_filename);
  if(!loader->loadIOData("camera",_camera))     ret = false;
  if(!loader->loadBool("showWireframe",_showWireframe)) ret = false;
  if(!GenericCustomNode::load(loader))          ret = false;

  return ret;
}
