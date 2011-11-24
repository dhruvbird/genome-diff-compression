#if !defined GC_ALGORITHM_HPP
#define GC_ALGORITHM_HPP

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <stack>
#include <algorithm>

#include <include/types.hpp>
#include <include/utils.hpp>
#include <include/segtree.hpp>
#include <include/suffix_array.hpp>

using namespace std;

// 'rin' is the input range set
//
// 'rout' is the output range set. It is guaranteed that 'rout'
// contains the mapping needed to re-contruct the least number of
// ranges needed to represent the complete range. Start with rout[0]
// and keep following the indexes till you reach some 'k' for which
// rout[k] == n.
//
// rout[i] contains the next index within rout (in sorted string
// order) where you need to jump to find the next range that completes
// the smallest number of ranges that cover the entire range.
// 
// you will need to index into 'slcp' to locate the index of this
// range in the original string
//
void
compress_ranges(vi_t &rin, vi_t &rout) {
    SegmentTree st;
    rout.clear();
    if (rin.empty()) {
        return;
    }

    const int n = rin.size();
    st.initialize(n + 1, n + 1);
    rout.resize(n);
    st.insert(n, 0);

    for (int i = rin.size() - 1; i >= 0; --i) {
        assert(rin[i] > 0);
        int l = i + 1, r = std::min(n, i+rin[i]);
        pii_t m = st.query_min(l, r);
        rout[i] = m.INDEX;
        st.insert(i, m.LENGTH + 1);
    }
}



int
find_ranges(std::string const &haystack, std::string const &needles, vi_t &ranges, vpii_t &slcp) {

    std::string t = haystack + "#" + needles + "$"; /* $ added to help pairwise LCP */

    // O(n log^2 n)
    SuffixArray sa(t);

    // The top of set-2 starts at s2top
    const int s2top = (int)haystack.size() + 1 /* Skip the # symbol */;

    // O(n log n)
    sa.lcp_across_sets(s2top, slcp);

    // 'ranges' holds the longest range for every index in 'needles'
    ranges.resize(needles.size());
    for (size_t i = s2top; i < slcp.size() - 1; ++i) {
        ranges[i-s2top] = slcp[i].LENGTH;
    }
    return s2top;
}

void
find_best_ranges(std::string const &haystack, std::string const &needles, vr_t &rout) {

    vi_t ranges, _rout;
    vpii_t slcp;

    int s2top = find_ranges(haystack, needles, ranges, slcp);

    // O(n log n)
    compress_ranges(ranges, _rout);

    rout.clear();

    for (size_t i = 0; i < _rout.size(); ) {
        int rlen = _rout[i] - i;
        rout.push_back(Range(i, slcp[i+s2top].INDEX, rlen));
        i = _rout[i];
    }
}

void
print_ranges(vr_t &ranges, FILE *pf, int soff, int doff) {
    for (size_t i = 0; i < ranges.size(); ++i) {
        fprintf(pf, "1 %d %d %d\n", ranges[i].srci+soff, ranges[i].desti+doff, ranges[i].len);
    }
}


#define MOD  70000027L
// #define MOD  325000799L
// #define MOD  325000021L
// #define MOD  322500019L
#define BASE 10L


struct SHSorter {
    bool
    operator()(piui_t const &lhs, piui_t const &rhs) const {
        return lhs.HASH < rhs.HASH;
    }
};

struct SHFinder {
    bool
    operator()(piui_t const &lhs, piui_t const &rhs) const {
        return lhs.HASH < rhs.HASH;
    }
};


unsigned int
get_hash(const char *p, int len) {
    unsigned long long h = 0;
    for (int i = 0; i < len; ++i) {
        h = (h * BASE) % MOD;
        h = (h + p[i]) % MOD;
    }
    return (unsigned int)h;
}

unsigned int
update_hash(int prev, int curr, unsigned int hash, unsigned long long msb_mul) {
    unsigned long long h = hash;
    unsigned long long sub = ((unsigned long long)prev * msb_mul) % MOD;
    h = (h + MOD - sub) % MOD;
    // cout<<"h is: "<<h<<endl;
    h = (h * BASE) % MOD;
    h = (h + curr) % MOD;
    // h = h < 0 ? MOD - h : h;
    // cout<<"h: "<<h<<endl;
    return static_cast<unsigned int>(h);
}

int
find_and_update(unsigned int h, vpii_t &out, vpiui_t &sorted_hashes) {
    pair<vpiui_t::iterator, vpiui_t::iterator> range = 
        std::equal_range(sorted_hashes.begin(), sorted_hashes.end(), make_pair(-1, h), SHFinder());

    int i1 = range.first - sorted_hashes.begin();
    int i2 = range.second - sorted_hashes.begin();

    if (i1 < sorted_hashes.size() && sorted_hashes[i1].HASH != h) {
        i1 = i2 = sorted_hashes.size();
    }
    else if (i1 < sorted_hashes.size()) {
        assert(sorted_hashes[i1].HASH == h);
        assert(sorted_hashes[i2-1].HASH == h);
    }

    out.push_back(make_pair(i1, i2));
    return i2-i1;
}

//
// Finds all indexes in 'haystack' where every substring of 'needles'
// of length 'bsz' occurs (with high probability). The haystack and
// needles are assumed to be arrays of comparable size (i.e. within a
// constant factor of each other)
//
// Time Complexity: O(n log n)
// Space Complexity: O(n)
//
// n -> size of the haystack or needles array
//
void
find_matches(const char *haystack, const char *needles, int bsz, vpii_t &out, vpiui_t &sorted_hashes) {
    // Step-1: Compute a rolling hash for every substring of size
    // 'bsz' in haystack.
    unsigned int h = 0;
    long long msb_mul = 1;
    int hlen = strlen(haystack);
    int nlen = strlen(needles);

    assert(bsz > 0);
    sorted_hashes.clear();
    sorted_hashes.reserve(hlen);
    out.clear();
    out.reserve(nlen);

    if (hlen < bsz || nlen < bsz) {
        return;
    }

    for (int i = 0; i < bsz-1; ++i) {
        msb_mul = (msb_mul * BASE) % MOD;
    }

    // printf("msb_mul: %u\n", (unsigned int)msb_mul);

    h = get_hash(haystack, bsz);
    sorted_hashes.push_back(make_pair(0, h));
    // printf("HASH[0] = %u\n", h);

    for (int i = 1; i < hlen-bsz+1; ++i) {
        h = update_hash(haystack[i-1], haystack[i + bsz - 1], h, msb_mul);
        sorted_hashes.push_back(make_pair(i, h));
        assert(h < MOD);
        // printf("removing %c, adding: %c, HASH[%d] = %u\n", haystack[i-1], haystack[i+bsz-1], i, h);
    }

    // Step-2: Sort all these hashes, preserving the indexes from
    // where the substrings for these hashes came from. Set this array
    // to 'sorted_hashes'.
    std::sort(sorted_hashes.begin(), sorted_hashes.end(), SHSorter());

    // Step-3: Compute a rolling hash for every substring of size
    // 'bsz' in needles and try to find the range of indexes where
    // that hash occurs in the sorted array we got from step-2
    // above. We can do this in time O(log n) per search. Add this
    // range of indexes into 'out'. Hence, every element of 'out'
    // references some range in 'sorted_hashes'.
    h = get_hash(needles, bsz);
    find_and_update(h, out, sorted_hashes);

    for (int i = 1; i < nlen-bsz+1; ++i) {
        h = update_hash(needles[i-1], needles[i + bsz - 1], h, msb_mul);
        int rlen = find_and_update(h, out, sorted_hashes);

        /*
        if (rlen > 10000) {
            printf("range difference: %d, HASH: %u, index: %u\n", rlen, h, i);
            char buff[60];
            memmove(buff, needles+i, 50);
            buff[50] = 0;
            printf("string: %s", buff);
        }
        */

        // printf("Searching hash: %u\n", h);
    }

    // Step-4: We are done!!
}

namespace algorithm {

    int
    test() {
        vi_t rin, rout;
        rin.resize(15);
        rin[0]  = 2;
        rin[1]  = 1;
        rin[2]  = 8;
        rin[3]  = 3;
        rin[4]  = 5;
        rin[5]  = 3;
        rin[6]  = 5;
        rin[7]  = 3;
        rin[8]  = 1;
        rin[9]  = 1;
        rin[10] = 1;
        rin[11] = 1;
        rin[12] = 3;
        rin[13] = 8;
        rin[14] = 6;

        compress_ranges(rin, rout);

        printf("Indexes: ");
        for (size_t i = 0; i < rin.size(); ++i) {
            printf("%4d", i);
        }
        printf("\n");

        printf("rin:     ");
        for (size_t i = 0; i < rin.size(); ++i) {
            printf("%4d", rin[i]);
        }
        printf("\n");

        printf("rout:    ");
        for (size_t i = 0; i < rout.size(); ++i) {
            printf("%4d", rout[i]);
        }
        printf("\n");

        printf("Ranges   ");
        for (size_t i = 0; i < rout.size(); ) {
            printf("%4d(%2d)", i, rout[i]-i);
            i = rout[i];
        }
        printf("%4d", rout.size());
        printf("\n");

        const char *s1 = "the quick brown fox huffs and puffs cats and runs and enthusiastically jumps over the lazy dogs";
        const char *s2 = "it's raining enthusiastically cats and dogs";
        vpii_t out;
        vpiui_t sh;
        int bsz = 12;

        find_matches(s1, s2, bsz, out, sh);

        for (size_t i = 0; i < out.size(); ++i) {
            printf("%.*s: %d, %d\n", bsz, s2+i, out[i].first, out[i].second);
        }

        printf("hash(e quick brow) == %u\n", get_hash("e quick brow", 12));
        printf("hash(enthusiastic) == %u\n", get_hash("enthusiastic", 12));
        return 0;
    }

}

#endif // GC_ALGORITHM_HPP
