#if !defined GC_UTIL_HPP
#define GC_UTIL_HPP

#include <iostream>
#include <assert.h>

using namespace std;

#if defined DEBUG
#define DCERR(X) cerr<<X
#define DPRINTF(M, ...) printf(M, ##__VA_ARGS__)
#else
#define DCERR(X)
#define DPRINTF(M, ...)
#endif

int log2(int n) {
    int lg2 = 0;
    while (n > 1) {
        n /= 2;
        ++lg2;
    }
    return lg2;
}

#endif // GC_UTIL_HPP
