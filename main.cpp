#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "polgenerator.hpp"

// Input
std::string treefile = "input/tree.txt";
std::fstream treestream;
long discriminant;
long *divisors;
std::vector<long> fixedcoefficients;

const int POL_TERMS = 9;
const int POL_DEGREE = 8;

// Output
std::string outputfile = "output.txt";
std::fstream outputstream;


/*
 * Process a single work-unit
 */
void process_work(PolGenerator &generator, std::vector<int> const &combination) {
    long coefficients[POL_TERMS];
    bool polAvailable = true;
    // Initialize generator with given congruence values, otherwise return;
    if (!generator.init(combination)) {
        return;
    }

    while (polAvailable) {       
        // Do the tests
        bool isSquare = generator.disc_is_square();

        if (isSquare) {
            generator.create_coefficients(coefficients);
            outputstream << "<" << coefficients[1];
            for (int i = 2; i < POL_TERMS; i++) {
                outputstream << ", " << coefficients[i];
            }
            outputstream << ">," << std::endl;
        }
        polAvailable = generator.configure_next();
    }
}

/*
 * Parse commandline arguments
 */
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
        } else if (cmdarg == "-coeffs") {
            if (ni >= argc) {
                std::cout << "-coeffs <c...> : set fixed coefficients, at least 1 is required" << std::endl;
                std::exit(1);
            }
            std::stringstream ss(argv[ni]);
            int c;
            while (ss >> c) {
                fixedcoefficients.push_back(c);
                ss.ignore(1, ',');
            }
            i = ni;
        } else if (cmdarg == "-divisors") {
            if (ni >= argc) {
                std::cout << "-divisors <c...> : set fixed divisors" << std::endl;
                std::exit(1);
            }
            std::stringstream ss(argv[ni]);
            long d;
            std::vector<long> divisors_tmp;
            while (ss >> d) {
                divisors_tmp.push_back(d);
                ss.ignore(1, ',');
            }
            divisors = new long[POL_TERMS];
            //extend the divisors with trailing 1's
            std::fill_n(divisors, POL_TERMS, 1);
            std::copy(divisors_tmp.begin(), divisors_tmp.end(),
                divisors + POL_TERMS - divisors_tmp.size());
            i = ni;
        } else if (cmdarg == "-discriminant") {
            if (ni >= argc) {
                std::cout << "-discriminant <amount> : set discriminant" << std::endl;
                std::exit(1);
            }
            std::stringstream ss(argv[ni]);
            ss >> discriminant;
            i = ni;
        } else if (cmdarg == "-tree") {
            if (ni >= argc) {
                std::cout << "-tree <name> : use given filename for discriminant tree [default = input/tree.txt]" << std::endl;
                std::exit(1);
            }
            treefile = argv[ni];
            i = ni;
        } else {
            std::cout << "Unknown argument " << argv[i] << std::endl;
            exit(1);
        }
    }
}

int main(int argc, char **argv) {
    init_arg(argc, argv);

    // Open output file stream
    outputstream.open(outputfile, std::ios::app | std::ios::in);
    if (!outputstream.is_open()) {
        std::cout << "Could not open file " << outputfile << ", exiting" << std::endl;
        return 1;
    }

    // Open tree file stream
    std::fstream treestream;
    treestream.open(treefile);
    if (!treestream.is_open()) {
        std::cout << "Could not open file " << treefile << ", exiting" << std::endl;
        return 1;
    }

    int p = 3;

    // Create polynomial generator
    PolGenerator generator(POL_DEGREE, p, fixedcoefficients, divisors, discriminant, treestream);
    treestream.close();

    outputstream << "{" << p;
    for (int i = 1; i < POL_DEGREE; i++) {
        outputstream << ", " << p; 
    }
    outputstream << "}" << std::endl;
    outputstream << "[" << std::endl;

    std::vector<int> combination = std::vector<int>(); //empty vector
    process_work(generator, combination);

    outputstream.close();
    outputstream << "]" << std::endl;
    
    return 0;
}
