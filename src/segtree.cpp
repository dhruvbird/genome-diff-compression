#if !defined GC_SEGTREE_HPP
#define GC_SEGTREE_HPP


#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdio.h>
#include <assert.h>

#include <include/types.hpp>
#include <include/utils.hpp>

using namespace std;


class SegmentTree {
    vpii_t repr;
    int len, v;
    // For each element, first is the index where the min. value
    // occur, and ssecond is the min. value in the range covered by
    // this node.

public:
    SegmentTree(int _len = 0, int val = 0) {
        this->initialize(_len, val);
    }

    void
    initialize(int _len, int val) {
        len = _len;
        v = val;
        this->repr.clear();
        this->repr.resize(1 << (log2(_len) + 2), make_pair(_len + 1, val));
    }

    void
    insert(int index, int val) {
        this->_insert(0, 0, this->len - 1, index, val);
    }

    pii_t
    _insert(int ni, int b, int e, int index, int val) {
        if (!(index >= b && index <= e)) {
            // Not in range.
            return make_pair(this->len + 1, this->v);
        }

        if (e == b) {
            if (val < this->repr[ni].LENGTH) {
                this->repr[ni] = make_pair(index, val);
            }
            return this->repr[ni];
        }

        int m = b + (e-b) / 2;
        pii_t r;
        if (index <= m) {
            r = this->_insert(ni*2 + 1, b, m, index, val);
        }
        else {
            r = this->_insert(ni*2 + 2, m+1, e, index, val);
        }

        if (r.LENGTH < this->repr[ni].LENGTH) {
            this->repr[ni] = r;
        }
        return this->repr[ni];
    }

    pii_t
    _query_min(int ni, int b, int e, int qf, int ql) {
        // printf("_query_max(%u, %u, %u, %u, %u)\n", ni, b, e, qf, ql);
        if (b > e || qf > e || ql < b) {
            // printf("[1] Returning: (-1, -1)\n");
            return pii_t(this->len + 1, v);
        }

        if (b >= qf && e <= ql) {
            // printf("[2] Returning: (%d, %d)\n", this->repr[ni].first, this->repr[ni].second);
            return this->repr[ni];
        }

        int m = b + (e-b) / 2;
        pii_t lhs = this->_query_min(ni*2 + 1, b, m, qf, ql);
        pii_t rhs = this->_query_min(ni*2 + 2, m+1, e, qf, ql);

        if (lhs.LENGTH == this->v) {
            // printf("[3] Returning: (%d, %d)\n", rhs.first, rhs.second);
            return rhs;
        }
        else if (rhs.LENGTH == this->v) {
            // printf("[4] Returning: (%d, %d)\n", lhs.first, lhs.second);
            return lhs;
        }
        else {
            pii_t &tmp = lhs.LENGTH < rhs.LENGTH ? lhs : rhs;
            // printf("[5] Returning: (%d, %d)\n", tmp.first, tmp.second);
            return tmp;
        }
    }

    // qf & ql are indexes; both inclusive.
    // first -> value, second -> index
    pii_t
    query_min(int qf, int ql) {
        return this->_query_min(0, 0, this->len - 1, qf, ql);
    }

};




namespace segtree {

    pii_t
    naive_query_min(vi_t const& v, int i, int j) {
        uint_t mv = v[i];
        uint_t mi = i;
        while (i <= j) {
            if (v[i] < mv) {
                mv = v[i];
                mi = i;
            }
            ++i;
        }
        return pii_t(mi, mv);
    }

    int
    test() {
        SegmentTree st;
        st.initialize(14, 100);

        st.insert(0, 45);
        st.insert(1, 4);
        st.insert(2, 5);
        st.insert(3, 2);
        st.insert(4, 99);
        st.insert(5, 41);
        st.insert(6, 45);
        st.insert(7, 45);
        st.insert(8, 51);
        st.insert(9, 89);
        st.insert(10, 1);
        st.insert(11, 3);
        st.insert(12, 5);
        st.insert(13, 98);

        vi_t v;
        v.push_back(45);
        v.push_back(4);
        v.push_back(5);
        v.push_back(2);
        v.push_back(99);
        v.push_back(41);
        v.push_back(45);
        v.push_back(45);
        v.push_back(51);
        v.push_back(89);
        v.push_back(1);
        v.push_back(3);
        v.push_back(5);
        v.push_back(98);

        for (int i = 0; i < 14; ++i) {
            for (int j = i; j < 14; ++j) {
                pii_t one = st.query_min(i, j);
                pii_t two = naive_query_min(v, i, j);
                printf("query_min(%d, %d) == (%d, %d)\n", i, j, one.INDEX, two.INDEX);
                assert(one.LENGTH == two.LENGTH);
            }
        }

        return 0;
    }
}

#if defined TEST
int
main() {
    return segtree::test();
}
#endif // TEST

#endif // GC_SEGTREE_HPP
