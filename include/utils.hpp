#if !defined GC_UTIL_HPP
#define GC_UTIL_HPP

#include <iostream>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>



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

size_t
file_size(const char *path) {
    struct stat sbuf;
    int r = stat(path, &sbuf);

    assert(r == 0);
    if (r < 0) {
        return 0;
    }

    return sbuf.st_size;
}

bool
file_exists(const char *path) {
    struct stat sbuf;
    int r = stat(path, &sbuf);
    return !r;
}


void*
map_file(const char *path, size_t *psize) {
    assert(path);
    assert(psize);

    *psize = file_size(path);
    int fd = open(path, O_RDONLY);
    void *maddr = mmap(NULL, *psize, PROT_READ, MAP_SHARED, fd, 0);
    return maddr;
}

#endif // GC_UTIL_HPP
