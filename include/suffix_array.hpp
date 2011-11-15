#if !defined GC_SUFFIX_ARRAY_HPP
#define GC_SUFFIX_ARRAY_HPP

#include <stdio.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <stack>
#include <algorithm>

#include <include/types.hpp>
#include <include/utils.hpp>

using namespace std;

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
    return i < (int)cmp.size() ? cmp[i] : 0;
}

struct SizeDoubler {
    vi_t &cmp;
    int sz;


    SizeDoubler(vi_t &c, int _sz)
        : cmp(c), sz(_sz)
    { }

    bool
    operator() (int lhs, int rhs) const {
        int l = this->cmp[lhs];
        int r = this->cmp[rhs];
        int l1 = get_rank(cmp, lhs + this->sz);
        int r1 = get_rank(cmp, rhs + this->sz);

        if (l == r) {
            return l1 < r1;
        }
        else {
            return l < r;
        }
    }
};

void
print_ranks(std::string const &s, vvi_t &dp) {
    int n = dp.empty() ? 0 : dp[0].size();
    for (size_t r = 0; r < n; ++r) {
        printf("%3c", s[r]);
        for (size_t c = 0; c < dp.size(); ++c) {
            printf("%4d", dp[c][r]);
        }
        printf("\n");
    }
}

struct SuffixArray {
    vvi_t dp;
    size_t ssize;

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
        assert(n > 0);

        ranks.resize(n);
        int rank = 1;
        int prev = data[indexes[0]];
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
    make_ranks2(vi_t &data, vi_t &indexes, vi_t &ranks, int sz) {
        ranks.clear();
        const size_t n = indexes.size();
        assert(n > 0);

        ranks.resize(n);
        int rank = 1;
        int x = data[indexes[0]];
        int y = get_rank(data, indexes[0]+sz);
        for (size_t i = 0; i < n; ++i) {
            int r1 = data[indexes[i]];
            int r2 = get_rank(data, indexes[i]+sz);
            printf("indexes[%d]: %d\n", i, indexes[i]);
            printf("data: (%d, %d), prev: (%d, %d)\n", r1, r2, x, y);
            if (x != r1 || y != r2) {
                ++rank;
                x = r1; y = r2;
            }
            ranks[indexes[i]] = rank;
        }
    }

    void
    sorted_indexes(vi_t &pos) {
        assert(this->dp.size() > 0);
        vi_t &_dp = this->dp.back();
        pos.clear();
        pos.resize(this->ssize);
        for (size_t i = 0; i < pos.size(); ++i) {
            pos[_dp[i]-1] = i;
        }
    }

public:
    SuffixArray(std::string const &s) {
        this->ssize = s.size();
        if (this->ssize == 0) {
            return;
        }
        const int n = this->ssize;
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
            std::sort(indexes.begin(), indexes.end(), SizeDoubler(this->dp[i-1], 1 << (i-1)));
            cerr<<"Done sorting\n";
            this->make_ranks2(this->dp[i-1], indexes, this->dp[i], 1 << (i-1));
            cerr<<"Done making ranks\n";
            // return;
        }
    }

    void
    lcp_pairwise(vi_t &pos, vi_t &lcp) {
        assert(this->dp.size() > 0);
        vi_t &_dp = this->dp.back();

        lcp.clear();
        lcp.resize(this->ssize, 0);
        this->sorted_indexes(pos);

        if (this->ssize < 1) {
            return;
        }

        const int n = this->ssize;
        for (size_t i = 0; i < this->ssize - 1; ++i) {
            int _lcp = 0, x = pos[i], y = pos[i+1];
            for (int j = this->dp.size() - 1; j >= 0 && x < n && y < n; --j) {
                // printf("dp[%d][%d] == %d, dp[%d][%d] == %d\n", j, x, dp[j][x], j, y, dp[j][y]);
                if (dp[j][x] == dp[j][y]) {
                    _lcp += (1 << j);
                    x += (1 << j);
                    y += (1 << j);
                }
            }
            lcp[pos[i]] = _lcp;
            printf("LCP [%d, %d] == %d\n", pos[i], pos[i+1], _lcp);
        }
    }


    // Returns the lengths of longest common prefix of every string in
    // set-2 with some string in set-1.
    // 
    // The vector 'lcp' is populated with pair of type (index, length)
    // where the i'th entry's 'index' is the index in the original
    // string where the string starting at index 'i' matches with
    // 'length' characters of match.
    void
    lcp_across_sets(int set2index, vpii_t &lcp) {
        vi_t pos, plcp;
        this->lcp_pairwise(pos, plcp);

        lcp.clear();
        lcp.resize(this->ssize, make_pair(0, 0));

        std::stack<pii_t> set2pos;

        for (size_t i = 0; i < this->ssize; ++i) {
            if (pos[i] < set2index) {
                int m = this->ssize + 1;
                while (!set2pos.empty()) {
                    pii_t t = set2pos.top();
                    set2pos.pop();
                    m = std::min(m, t.LENGTH);
                    lcp[t.INDEX] = make_pair(pos[i], m);
                }
            }
            else {
                set2pos.push(make_pair(pos[i], plcp[pos[i]]));
            }
        }

        while (!set2pos.empty()) {
            set2pos.pop();
        }

        for (int i = this->ssize - 1; i >= 0; --i) {
            if (pos[i] < set2index) {
                int m = plcp[pos[i]];
                while (!set2pos.empty()) {
                    pii_t t = set2pos.top();
                    set2pos.pop();
                    if (m > lcp[t.INDEX].LENGTH) {
                        lcp[t.INDEX] = make_pair(pos[i], m);
                    }
                    m = std::min(m, t.LENGTH);
                }
            }
            else {
                set2pos.push(make_pair(pos[i], plcp[pos[i]]));
            }
        }
    }

};

void
print_suffix_array(std::string const &s, SuffixArray &sa, vi_t *plcp = NULL) {
    vi_t pos;
    sa.sorted_indexes(pos);
    for (size_t i = 0; i < pos.size(); ++i) {
        // Limit each line to 60 characters
        if (plcp) {
            printf("%3d: [%2d]: %s\n", pos[i], (*plcp)[pos[i]], s.substr(pos[i], 60).c_str());
        }
        else {
            printf("%3d: %s\n", pos[i], s.substr(pos[i], 60).c_str());
        }
    }
}

void
print_pairwise_lcp(std::string const &s, SuffixArray &sa) {
    vi_t pos, lcp;
    sa.lcp_pairwise(pos, lcp);
    print_suffix_array(s, sa, &lcp);
}


namespace suffix_array {
    int
    test() {

        std::string s2 = "yabadabadoo";
        SuffixArray sa2(s2);
        print_ranks(s2, sa2.dp);
        print_suffix_array(s2, sa2);

        vpii_t slcp;
        sa2.lcp_across_sets(4, slcp);
        for (int i = 0; i < slcp.size(); ++i) {
            printf("slcp[%d] = (%d, %d)\n", i, slcp[i].INDEX, slcp[i].LENGTH);
        }

        print_pairwise_lcp(s2, sa2);
        // return 0;

        SuffixArray sa1("banana");
        print_ranks("banana", sa1.dp);

        SuffixArray sa3("mississippi");
        print_ranks("mississippi", sa3.dp);

        std::string s4 = "Regular expressions, or just regexes, are at the core of Perl's text processing, and certainly are one of the features that made Perl so popular. All Perl programmers pass through a stage where they try to program everything as regexes, and when that's not challenging enough, everything as a single regex. Perl's regexes have many more features than I can, or want, to present here, so I include those advanced features I find most useful and expect other Perl programmers to know about without referring to perlre, the documentation page for regexes.";
        SuffixArray sa4(s4);
        print_ranks(s4, sa4.dp);
        print_suffix_array(s4, sa4);
        
        cout<<endl;
        print_pairwise_lcp(s4, sa4);

        return 0;
    }
}

#endif // GC_SUFFIX_ARRAY_HPP
