#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

typedef pair<int, int> pii_t;
typedef vector<char> vc_t;
typedef vector<int>  vi_t;
typedef vector<vi_t> vvi_t;

int log2(int n) {
    int lg2 = 0;
    while (n > 1) {
        n /= 2;
        ++lg2;
    }
    return lg2;
}


struct IndexesSorter {
    vi_t &cmp;

    IndexesSorter(vi_t &c)
        : cmp(c)
    { }

    bool
    operator() (int lhs, int rhs) const {
        return this->cmp[lhs] < this->cmp[rhs];
    }

};


int
get_rank(vi_t &cmp, int i) {
    return i < cmp.size() ? cmp[i] : 0;
}

struct SizeDoubler {
    vi_t &cmp;

    SizeDoubler(vi_t &c)
        : cmp(c)
    { }

    bool
    operator() (int lhs, int rhs) const {
        int l = this->cmp[lhs];
        int r = this->cmp[rhs];
        int l1 = get_rank(cmp, lhs+1);
        int r1 = get_rank(cmp, rhs+1);

        if (l == r) {
            return l1 < r1;
        }
        else {
            return l < r;
        }
    }
};

void
print_ranks(vvi_t &dp) {
    int n = dp.empty() ? 0 : dp[0].size();
    for (size_t r = 0; r < n; ++r) {
        for (size_t c = 0; c < dp.size(); ++c) {
            printf("%4d", dp[c][r]);
        }
        printf("\n");
    }
}

struct SuffixArray {
    vvi_t dp;

    void
    fill_indexes(vi_t &indexes, size_t n) {
        indexes.clear();
        indexes.resize(n);
        for (size_t i = 0; i < n; ++i) {
            indexes[i] = i;
        }
    }

    void
    make_ranks1(vi_t &data, vi_t &indexes, vi_t &ranks) {
        ranks.clear();
        const size_t n = indexes.size();
        ranks.resize(n);
        int rank = 1;
        int prev = n > 0 ? data[indexes[0]] : 0;
        for (size_t i = 0; i < n; ++i) {
            cerr<<"data: "<<data[indexes[i]]<<", prev: "<<prev<<endl;
            if (data[indexes[i]] != prev) {
                ++rank;
                prev = data[indexes[i]];
            }
            ranks[indexes[i]] = rank;
        }
    }

    void
    make_ranks2(vi_t &data, vi_t &indexes, vi_t &ranks) {
        ranks.clear();
        const size_t n = indexes.size();
        ranks.resize(n);
        int rank = 1;
        int x = n > 0 ? data[indexes[0]] : 0;
        int y = n > 0 ? get_rank(data, indexes[0]+1) : 0;
        for (size_t i = 0; i < n; ++i) {
            int r1 = data[indexes[i]];
            int r2 = get_rank(data, indexes[i]+1);
            printf("indexes[%d]: %d\n", i, indexes[i]);
            printf("data: (%d, %d), prev: (%d, %d)\n", r1, r2, x, y);
            if (x != r1 || y != r2) {
                ++rank;
                x = r1; y = r2;
            }
            ranks[indexes[i]] = rank;
        }
    }


public:
    SuffixArray(std::string const &s) {
        int n = s.size();
        if (n == 0) {
            return;
        }
        dp.resize(log2(n) + ((n & (n-1)) == n ? 1 : 2));
        vi_t buf(s.begin(), s.end());
        vi_t indexes;
        this->fill_indexes(indexes, n);
        std::sort(indexes.begin(), indexes.end(), IndexesSorter(buf));
        this->make_ranks1(buf, indexes, this->dp[0]);

        cerr<<"[1]\n";

        // return;

        for (int i = 1; i < this->dp.size(); ++i) {
            cerr<<"i: "<<i<<endl;
            this->fill_indexes(indexes, n);
            cerr<<"Done filling indexes\n";
            std::sort(indexes.begin(), indexes.end(), SizeDoubler(this->dp[i-1]));
            cerr<<"Done sorting\n";
            this->make_ranks2(this->dp[i-1], indexes, this->dp[i]);
            cerr<<"Done making ranks\n";
            // return;
        }
    }
};

namespace suffix_array {
    int
    test() {
        SuffixArray sa1("banana");
        print_ranks(sa1.dp);

        SuffixArray sa2("yabadabadoo");
        print_ranks(sa2.dp);

        SuffixArray sa3("mississippi");
        print_ranks(sa3.dp);
    }
}

#if defined TEST
int
main() {
    return suffix_array::test();
}
#endif // TEST
