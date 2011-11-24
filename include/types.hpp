#include <vector>
#include <utility>

#if !defined GC_TYPES_HPP
#define GC_TYPES_HPP

typedef unsigned int uint_t;
typedef std::pair<int, int> pii_t;
typedef std::pair<int, unsigned int> piui_t;
typedef std::vector<char> vc_t;
typedef std::vector<int>  vi_t;
typedef std::vector<vi_t> vvi_t;
typedef std::vector<pii_t> vpii_t;
typedef std::vector<piui_t> vpiui_t;

struct Range {
    int srci, desti, len;

    Range(int si, int di, int l)
        : srci(si), desti(di), len(l)
    { }
};

typedef std::vector<Range> vr_t;

#define INDEX  first
#define LENGTH second
#define HASH   second


#endif // GC_TYPES_HPP
