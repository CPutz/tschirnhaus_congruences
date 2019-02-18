#ifndef POLGENERATOR_HPP
#define POLGENERATOR_HPP

#include <vector>
#include <fstream>
#include "polmodtree.hpp"


class PolGenerator
{
    std::vector<long> divisors; // divisor for each a_k (A_k)
    long *avalues; // a_k values
    long *aupper; // a_k upper bounds
    long *alower; // a_k lower bounds
    long discriminant; // the desired discriminant
    int n; // degree of the polynomial
    int fixed; // number of fixed coefficients
    int locked; // number of locked coefficients
    int p;
    bool *squares_p; //lookup table for squares mod p
    PolModTree *disctree;
    
    void eval_disc(int k);
    bool next(int level);
    bool init_a(int level);
    
public:
    PolGenerator(int const n, int const p,
        std::vector<long> const &fixedcoefficients,
        std::vector<long> const &divisors,
        long const discriminant, std::fstream &treestream);
    ~PolGenerator();
    bool init(std::vector<int> const &coefficients);
    bool configure_next();
    void create_coefficients(long container[]);
    bool disc_is_square();
};

#endif // POLGENERATOR_HPP 