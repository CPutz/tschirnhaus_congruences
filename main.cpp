#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "polgenerator.hpp"

// Input
std::string treefile = "input/tree.txt";
std::fstream treestream;
long degree;
long discriminant;
long modulus;
std::vector<long> divisors;
std::vector<long> fixedcoefficients;

// Output
std::string outputfile = "output.txt";
std::fstream outputstream;


/*
 * Process a single work-unit
 */
void process_work(PolGenerator &generator, std::vector<int> const &combination) {
    long coefficients[degree+1];
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
            for (int i = 2; i <= degree; i++) {
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
            //polynomial is monic
            divisors.push_back(1);
            while (ss >> d) {
                divisors.push_back(d);
                ss.ignore(1, ',');
            }
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
        } else if (cmdarg == "-modulus") {
            if (ni >= argc) {
                std::cout << "-modulus <name> : set modulus" << std::endl;
                std::exit(1);
            }
            std::stringstream ss(argv[ni]);
            ss >> modulus;
            i = ni;
        } else if (cmdarg == "-degree") {
            if (ni >= argc) {
                std::cout << "-degree <name> : set degree" << std::endl;
                std::exit(1);
            }
            std::stringstream ss(argv[ni]);
            ss >> degree;
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

    // Create polynomial generator
    PolGenerator generator(degree, modulus, fixedcoefficients, divisors, discriminant, treestream);
    treestream.close();

    outputstream << "{" << modulus;
    for (int i = 1; i < degree; i++) {
        outputstream << ", " << modulus; 
    }
    outputstream << "}" << std::endl;
    outputstream << "[" << std::endl;

    std::vector<int> combination = std::vector<int>(); //empty vector
    process_work(generator, combination);

    outputstream << "]" << std::endl;
    outputstream.close();
    
    return 0;
}
