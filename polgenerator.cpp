#include <algorithm>
#include <vector>
#include <iostream>
#include "polgenerator.hpp"
#include "mathextra.hpp"

PolGenerator::PolGenerator(int const n, int const p,
        std::vector<long> const &fixedcoefficients, long *divisors,
        long const discriminant, std::fstream &treestream) {
    this->divisors = divisors;
    this->discriminant = discriminant;
    this->n = n;
    this->p = p;
    this->fixed = fixedcoefficients.size();

    avalues = new long[n + 1];
    aupper = new long[n + 1];
    alower = new long[n + 1];

    this->disctree = new PolTree(treestream, p);

    //initialize the fixed coefficients
    avalues[0] = 1;
    for (int i = 1; i < this->fixed; i++) {
        avalues[i] = fixedcoefficients[i];
    }
    //Initialize bounds
    for (int i = this->fixed; i <= n; i++) {
        alower[i] = 0;
        aupper[i] = p-1;
    }

    //create squares mod p lookup table
    squares_p = new bool[p];
    for (int i = 0; i < p; i++) {
        squares_p[i] = false;
    }
    for (int i = 0; i < p; i++) {
        squares_p[(i*i) % p] = true;
    }
}

PolGenerator::~PolGenerator() {
    delete[] divisors;
    delete[] avalues;
    delete[] alower;
    delete[] aupper;
    delete disctree;
}

/*
 * Evaluates the disciminant for a_k
 */
void PolGenerator::eval_disc(int k) {
    this->disctree->eval(n - k, avalues[k]);
}

/*
 * Initialize generator to starting values
 */
bool PolGenerator::init(std::vector<int> const &coefficients) {
    std::copy(coefficients.begin(), coefficients.end(),
        this->avalues + this->fixed);
    this->locked = this->fixed + coefficients.size();
    
    bool res = true;

    for (int i = locked; i <= n && res; i++) {
        res = res && init_a(i);
    }

    if (res) {
        for (int i = this->fixed; i <= n; i++) {
            eval_disc(i);
        }
    }

    return res;
}

/*
 * Initializes a_i using the bounds on a_i
 */
bool PolGenerator::init_a(int i) {
    avalues[i] = alower[i];
    return true;
}

/*
 * Gives the next configuration of a_1,...,a_{level}
 */
bool PolGenerator::next(int level) {
    avalues[level]++;
    eval_disc(level);

    if (avalues[level] > aupper[level]) {
        //if we reached the last non-locked level we are done
        if (level == locked) {
            return false;
        }
        bool res = next(level - 1) && init_a(level);
        if (res) {
            eval_disc(level);
        }
        return res;
    }
    return true;
}

/*
 * Configures the generator from the current values to the next iteration
 */
bool PolGenerator::configure_next() {
    return next(n);
}

/*
 * Stores the coefficients of the polynomial
 * x^7 + a_1d_1x^6 + a_2d_2x^5 + a_3d_3x^4 + a_4d_4x^3 + a_5d_5x^2 + a_6d_6x + a_7d_7
 * in descending order of degree
 */
void PolGenerator::create_coefficients(long container[]) {
    std::copy(avalues, avalues + n + 1, container);
    for (int i = 0; i <= n; i++) {
        container[i] *= divisors[i];
    }
}
 
/*
 * Returns if the discriminant of the current polynomial is a square
 * times the desired discriminant for the number field
 */
bool PolGenerator::disc_is_square() {
    long disc = this->disctree->get_val();
    long disc_p = mod(disc * this->discriminant, this->p);
    if (avalues[1] == 0 && avalues[2] == 0 && avalues[3] == 0 && avalues[4] == 0 &&
        avalues[5] == 0 && avalues[6] == 0 && avalues[7] == 1 && avalues[8] == 2) {
        std::cout << mod(disc, this->p) << " " << disc_p << std::endl;
        this->disctree->print();
    }
    return squares_p[disc_p];
}