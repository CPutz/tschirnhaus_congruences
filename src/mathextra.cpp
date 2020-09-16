#include "mathextra.hpp"

long mod(long const a, long const m) {
    long r = a % m;
    return r < 0 ? r + m : r;
}

int inverse(int a, int m) {
    int t, nt;
    int r, nr;
    int q;
    int tmp;

    if (m < 0) {
        m = -m;
    }
    if (a < 0) {
        a = m - (-a % m);
    }
    t = 0;
    nt = 1;
    r = m;
    nr = a % m;
    while (nr != 0) {
        q = r/nr;
        tmp = nt;  nt = t - q*nt;  t = tmp;
        tmp = nr;  nr = r - q*nr;  r = tmp;
    }
    if (r > 1) {
        return -1;  /* No inverse */
    }
    if (t < 0) {
        t += m;
    }
    return t;
}

int chinese_remainder(int const *a, int const *m, int const size) {
    int p;
    int prod = 1;
    int sum = 0;
    
    for (int i = 0; i < size; i++) {
        prod *= m[i];
    }
    for (int i = 0; i < size; i++) {
        p = prod / m[i];
        sum += a[i] * inverse(p, m[i]) * p;
    }
    
    return sum % prod;
}
