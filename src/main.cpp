#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include "polmodtree.hpp"

// Output
std::string outputfile = "output.txt";
std::fstream outputstream;

// Input
std::vector<std::vector<std::string>> treefiles;
std::vector<std::vector<int>> varmodulii;

std::vector<int> moduli;
std::vector<PolModTree *> trees;
std::vector<int> varmoduli;
std::vector<int> values;
std::set<std::vector<int>> results;
int depth;


bool next(int level) {
    values[level]++;
    if (values[level] >= varmoduli[level]) {
        if (level == 0 || !next(level-1)) {
            return false;
        }
        values[level] = 0;
    }

    for (PolModTree *t : trees) {
        t->eval(depth-1-level, values[level]);
    }

    return true;
}

void init() {
    values = std::vector<int>(depth);
    for (int i = depth-1; i >= 0; i--) {
        values[i] = 0;
        for (PolModTree *t : trees) {
            t->eval(i, 0);
        }
    }

    /*if (varmoduli.size() == 0) {
        int max_modulus = 0;
        for (int m : moduli) {
            if (max_modulus < m) {
                max_modulus = m;
            }
        }

        for (int i = 0; i < depth; i++) {
            varmoduli.push_back(max_modulus);
        }
    }*/
}

void init_arg(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        int ni = i + 1;
        std::string cmdarg = argv[i];
        if (cmdarg == "-output") {
            // Specify name of output progress file
            if (ni >= argc) {
                std::cout << "-output <name> : use given filename for output [default = output.txt]" << std::endl;
                std::exit(1);
            }
            outputfile = argv[ni];
            i = ni;
        } else if (cmdarg == "-input") {
            // Add congruence-files
            if (ni >= argc) {
                std::cout << "-input <file...> : use given comma-separated congruence-files, at least 1 file is required" << std::endl;
                std::exit(1);
            }
            std::stringstream ss(argv[ni]);
            std::string file;
            treefiles.push_back(std::vector<std::string>());
            while (std::getline(ss, file, ',')) {
                treefiles.back().push_back(file);
            }
            i = ni;
        } else if (cmdarg == "-moduli") {
            if (ni >= argc) {
                std::cout << "-moduli <c...> : set fixed moduli" << std::endl;
                std::exit(1);
            }
            std::stringstream ss(argv[ni]);
            int d;
            while (ss >> d) {
                moduli.push_back(d);
                ss.ignore(1, ',');
            }
            i = ni;
        } else if (cmdarg == "-varmoduli") {
            if (ni >= argc) {
                std::cout << "-varmoduli <c...> : set fixed moduli for the variables" << std::endl;
                std::exit(1);
            }
            std::stringstream ss(argv[ni]);
            int d;
            varmodulii.push_back(std::vector<int>());
            while (ss >> d) {
                varmodulii.back().push_back(d);
                ss.ignore(1, ',');
            }
            i = ni;
        } else {
            // Invalid commandline argument given, report and exit
            std::cout << "Unknown argument " << argv[i] << std::endl;
            exit(1);
        }
    }
}

void read_input(int index) {
    trees = std::vector<PolModTree *>();
    std::fstream treestream;
    treestream.exceptions(std::fstream::failbit);
    int i = 0;
    for (std::string file : treefiles[index]) {
        try {
            treestream.open(file);
            trees.push_back(new PolModTree(treestream, moduli[i]));
        } catch (const std::exception& e) {
            std::cout << "Opening tree file '" << file << "' failed, it either doesn't exist or is not accessible." << std::endl;
            exit(1);
        }
        treestream.close();
        i++;
    }
}

int main(int argc, char **argv) {
    init_arg(argc, argv);

    outputstream.open(outputfile, std::ios::app | std::ios::in);
    if (!outputstream.is_open()) {
        std::cout << "Could not open file " << outputfile << ", exiting" << std::endl;
        return 1;
    }
    
    for (int index = 0; index < treefiles.size(); index++) {
        varmoduli = varmodulii[index];
        read_input(index);
        //the trees all have the same depth
        depth = trees.back()->get_depth();

        init();
        do {
            std::vector<int> result;
            //TODO: hardcoded 4 here
            if (trees[0]->get_val() <= 4) {
                for (PolModTree *t : trees) {
                    result.push_back(t->get_val());
                }
                results.insert(result);
            }
        } while (next(depth-1));
    }

    for (std::vector<int> result : results) {
        outputstream << result[0];
        for (int i = 1; i < result.size(); i++) {
            outputstream << ", " << result[i];
        }
        outputstream << std::endl;
    }

    return 0;
}
