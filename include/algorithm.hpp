#if !defined GC_ALGORITHM_HPP
#define GC_ALGORITHM_HPP

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
#include <include/segtree.hpp>

using namespace std;

// 'rin' is the input range set
//
// 'rout' is the output range set. It is guaranteed that 'rout'
// contains the mapping needed to re-contruct the least number of
// ranges needed to represent the complete range. Start with rout[0]
// and keep following the indexes till you reach some 'k' for which
// rout[k] == n.
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
            printf("%4d", i);
            i = rout[i];
        }
        printf("%4d", rout.size());
        printf("\n");

        return 0;
    }

}

#endif // GC_ALGORITHM_HPP
