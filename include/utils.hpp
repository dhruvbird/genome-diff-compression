#if !defined GC_UTIL_HPP
#define GC_UTIL_HPP

int log2(int n) {
    int lg2 = 0;
    while (n > 1) {
        n /= 2;
        ++lg2;
    }
    return lg2;
}

#endif // GC_UTIL_HPP
