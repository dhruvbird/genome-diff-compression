#include <iostream>
#include <include/suffix_array.hpp>
#include <include/algorithm.hpp>
#include <include/fasta.hpp>

using namespace std;

char *ref_buff = NULL;
char *tgt_buff = NULL;
int ref_len = 0;
int tgt_len = 0;

// 4MB
#define REF_BUFF_SIZE 8*1024*1024
// 2MB
#define TGT_BUFF_SIZE 16*1024*1024/2
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
print_ranges(vr_t &ranges, FILE *pf) {
    for (size_t i = 0; i < ranges.size(); ++i) {
        fprintf(pf, "1 %d %d %d\n", ranges[i].srci, ranges[i].desti, ranges[i].len);
    }
}

// Computes the ranges from ref_path that tgt_path needs to be split
// into so that it is completely covered. Places the ranges in
// diff_path.
void
diff_files(std::string ref_path, std::string tgt_path, std::string diff_path) {
    FastaReader ref_reader(ref_path);
    FastaReader tgt_reader(tgt_path);

    memcpy(ref_buff, "NNNNNNNNNN", 10);
    ref_len = ref_reader.read(ref_buff + 10, REF_BUFF_SIZE - 10) + 10;
    tgt_len = tgt_reader.read(tgt_buff, TGT_BUFF_SIZE);

    cerr<<"Done reading files\n";

    std::string ref(ref_buff, ref_len);
    std::string tgt(tgt_buff, tgt_len);

    vr_t rout;
    find_best_ranges(ref, tgt, rout);

    print_ranges(rout, stdout);

}


void
init() {
    ref_buff = (char*)malloc(REF_BUFF_SIZE);
    tgt_buff = (char*)malloc(TGT_BUFF_SIZE);
    ref_len = 0;
    tgt_len = 0;
}

int
main() {
#if 0
    // std::string ref = "ACCGATCCAGTAGACACGATACCAGATGACAGATATTTACAGTAGATACAGATACAGATGAGAG";
    // std::string tgt = "GATACCATGACATTTTGACACCAGTGACAGATGACACCCAGGAGGTTTAGAGGGCCTCTCTGGGGGCGCGGTAGAGATTT";

    std::string ref = "abcdefghijklmnopqrstuvwxyz pragya pande stony brook university";
    std::string tgt = "saxena pragya suny sb";

    vr_t rout;
    find_best_ranges(ref, tgt, rout);

    print_ranges(rout, stdout);
#else
    init();
    // "data/chr/chr1.fa"
    // "data/HuRef/hs_alt_Celera_chr1.fa", "data/HuRef/hs_alt_Celera_chr1.fa"
    diff_files("data/chr/chr1.fa", "data/fasta_Watson_0", "");
#endif

}
