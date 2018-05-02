#include <cstring>
#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <set>
#include <stdio.h>


static void usage() {
    fprintf(stderr, R"(usage: pbrt [<options>] <filename.pbrt...>
Options:
    --help               Print this help text.
    --new <filename>     Create a new pbrt file, instead of
                         modifying the original one.
)");
    exit(1);
}


std::string computeNewFilename(std::string filename) {
    std::string file = filename;
    std::string add = "_impmap";
    std::size_t pos = file.find_last_of(".");

    return file.substr(0,pos) + add + file.substr(pos);
}

int main(int argc, char *argv[]) {
    char* pbrtFilename, newFilename;
    std::string pbrtFilenameStr, newFilenameStr;
    bool newfile = false;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h"))
            usage();
        else if (!strcmp(argv[i], "--new") || !strcmp(argv[i], "-n"))
            newfile = true;
        else if (!pbrtFilename)
            pbrtFilename = argv[i];
        else
            usage();
    }
    if (!pbrtFilename)
        usage();

    pbrtFilenameStr = pbrtFilename;
    newFilenameStr = newFilename;


    if (newfile) {
      newFilenameStr = computeNewFilename(pbrtFilenameStr);
    }

    FILE *f = (strcmp(pbrtFilename, "-") == 0) ? stdout : fopen(pbrtFilename, "w");
    if (!f) {
        perror(pbrtFilename);
        return 1;
    }

    if (f != stdout) fclose(f);
    std::cout << "hey there" <<  std::endl;

    return 0;
}
