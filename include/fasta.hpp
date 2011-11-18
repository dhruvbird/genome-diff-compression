#if !defined GC_FASTA_HPP
#define GC_FASTA_HPP

#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <assert.h>

using namespace std;

class FastaReader {
    FILE *pf;
    std::string fpath;
    char *buff;
    int bsz;
    size_t sz;

    void
    readsome() {
        // Assume that buff is empty
        if (feof(this->pf)) {
            this->bsz = 0;
            return;
        }

        ssize_t ret = ::getline(&this->buff, &this->sz, this->pf);
        if (ret < 0) {
            this->bsz = 0;
            return;
        }

        if (ret > 0 && this->buff[0] == '>') {
            readsome();
            return;
        }

        if (ret > 0 && this->buff[ret-1] == '\n') {
            --ret;
        }
        this->bsz = ret;
    }

public:
    FastaReader(std::string path)
        : pf(NULL), fpath(path), buff(NULL), bsz(0), sz(0) {
        this->pf = fopen(path.c_str(), "r");
        assert(this->pf);
    }

    int
    read(char *out, int size) {
        int ctr = 0;
        int tr = size;
        while (tr) {
            if (!this->bsz) {
                readsome();
                if (!this->bsz) {
                    return ctr;
                }
            }

            int m = this->bsz < tr ? this->bsz : tr;
            memmove(out, this->buff, m);

            int rem = this->bsz - m;
            if (rem) {
                memmove(this->buff, this->buff + m, rem);
            }

            this->bsz = rem;
            out += m;
            tr -= m;
            ctr += m;
        }
        return ctr;
    }


};

namespace fasta {
    int
    test() {
        // return 0;
        // FastaReader fr("data/chr/chr1.fa");
        FastaReader fr("data/fasta_Watson_0");
        char buff[1024];
        int r = fr.read(buff, 50);

        while (r) {
            buff[r] = '\0';
            printf("%s", buff);
            r = fr.read(buff, 50);
        }
        return 0;
    }
}

#endif // GC_FASTA_HPP
