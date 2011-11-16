#include <iostream>
#include <include/suffix_array.hpp>
#include <include/algorithm.hpp>
#include <include/fasta.hpp>

using namespace std;

void
find_best_ranges(std::string const &haystack, std::string const &needles, vr_t &rout) {
    std::string t = haystack + "#" + needles + "$"; /* $ added to help pairwise LCP */

    // O(n log^2 n)
    SuffixArray sa(t);

    vpii_t slcp;

    // The top of set-2 starts at s2top
    const int s2top = (int)haystack.size() + 1 /* Skip the # symbol */;

    // O(n log n)
    sa.lcp_across_sets(s2top, slcp);

    vi_t rin, _rout;
    rin.resize(needles.size());
    for (size_t i = s2top; i < slcp.size() - 1; ++i) {
        rin[i-s2top] = slcp[i].LENGTH;
    }

    // O(n log n)
    compress_ranges(rin, _rout);

    rout.clear();

    for (size_t i = 0; i < _rout.size(); ) {
        int rlen = _rout[i] - i;
        rout.push_back(Range(i, slcp[i+s2top].INDEX, rlen));
        i = _rout[i];
    }

}

void
print_ranges(vr_t &ranges) {
    for (size_t i = 0; i < ranges.size(); ++i) {
        printf("1 %d %d %d\n", ranges[i].srci, ranges[i].desti, ranges[i].len);
    }
}


int
main() {
    // std::string ref = "ACCGATCCAGTAGACACGATACCAGATGACAGATATTTACAGTAGATACAGATACAGATGAGAG";
    // std::string tgt = "GATACCATGACATTTTGACACCAGTGACAGATGACACCCAGGAGGTTTAGAGGGCCTCTCTGGGGGCGCGGTAGAGATTT";

    std::string ref = "abcdefghijklmnopqrstuvwxyz pragya pande stony brook university";
    std::string tgt = "saxena pragya suny sb";

    vr_t rout;
    find_best_ranges(ref, tgt, rout);

    print_ranges(rout);
}
