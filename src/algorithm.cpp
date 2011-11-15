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

void
compress_ranges(vi_t &rin, vi_t &rout) {
    SegmentTree st;
    rout.clear();
    if (rin.empty()) {
        return;
    }

    const int n = rin.size();
    rout.resize(n);
    st.insert(n, n, 0);
    rout[n-1] = n;

    for (int i = rin.size() - 1; i >= 0; --i) {
        int l = i + 1, r = std::min(n, i+rin[i]);
        pii_t m = st.query_min(l, r);
        rout[i] = m.INDEX;
        st.insert(n-1, n-1, m.LENGTH + 1);
    }
}
