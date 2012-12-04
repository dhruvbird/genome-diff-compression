#include <iostream>
#include <include/suffix_array.hpp>
#include <include/algorithm.hpp>
#include <include/fasta.hpp>

using namespace std;

char *ref_buff = NULL;
char *tgt_buff = NULL;
int ref_len = 0;
int tgt_len = 0;

// 16MB
#define REF_BUFF_SIZE 16*1024*1024
// 6MB
#define TGT_BUFF_SIZE 6*1024*1024



// Computes the ranges from ref_path that tgt_path needs to be split
// into so that it is completely covered. Places the ranges in
// diff_path.
void
diff_files(std::string ref_path, std::string tgt_path, int seek1, int seek2) {
    FastaReader ref_reader(ref_path);
    FastaReader tgt_reader(tgt_path);

    // Fill with A*5000, C*5000, G*5000, T*5000, N*5000.
    memset(ref_buff,       'A', 5000);
    memset(ref_buff+5000,  'C', 5000);
    memset(ref_buff+10000, 'G', 5000);
    memset(ref_buff+15000, 'T', 5000);
    memset(ref_buff+20000, 'N', 5000);

    int off1 = 5000*5;
    // int seek1 = 19*1024*1024;
    // int seek2 = 30*1024*1024;

    fprintf(stderr, "seek1: %d, seek2: %d\n", seek1, seek2);
    ref_reader.seek(seek1);
    tgt_reader.seek(seek2);

    ref_len = ref_reader.read(ref_buff + off1, REF_BUFF_SIZE - off1) + off1;
    tgt_len = tgt_reader.read(tgt_buff, TGT_BUFF_SIZE);

    std::string ref(ref_buff, ref_len);
    std::string tgt(tgt_buff, tgt_len);

    free(ref_buff);
    free(tgt_buff);
    ref_buff = 0;
    tgt_buff = 0;

    vr_t rout;
    find_best_ranges(ref, tgt, rout);

    print_ranges(rout, stdout, seek2, seek1);

}


void
init() {
    ref_buff = (char*)malloc(REF_BUFF_SIZE);
    tgt_buff = (char*)malloc(TGT_BUFF_SIZE);
    ref_len = 0;
    tgt_len = 0;
}


void*
read_file(const char *path, size_t *psize) {
    *psize = file_size(path); // 1000000
    char *data = (char*)malloc(*psize + 1);
    FastaReader fr(path);
    fr.read(data, *psize);
    return data;
}


int
main(int argc, char *argv[]) {
#if 0
    // std::string ref = "ACCGATCCAGTAGACACGATACCAGATGACAGATATTTACAGTAGATACAGATACAGATGAGAG";
    // std::string tgt = "GATACCATGACATTTTGACACCAGTGACAGATGACACCCAGGAGGTTTAGAGGGCCTCTCTGGGGGCGCGGTAGAGATTT";

    std::string ref = "abcdefghijklmnopqrstuvwxyz first name stony brook university";
    std::string tgt = "last2 first suny sb";

    vr_t rout;
    find_best_ranges(ref, tgt, rout);

    print_ranges(rout, stdout);
#else
    init();
    int seek1, seek2;
    assert(argc > 4);
    seek1 = atoi(argv[3]) * 1048576;
    seek2 = atoi(argv[4]) * 1048576;
    const char *pref = argv[1];
    const char *ptgt = argv[2];

    // "data/chr/chr1.fa"
    // "data/HuRef/hs_alt_Celera_chr1.fa", "data/HuRef/hs_alt_Celera_chr1.fa"
    // diff_files("data/chr/chr1.fa", "data/fasta_Watson_0", "");
    // diff_files("data/chr/chr17.fa", "data/HuRef/hs_alt_Celera_chr17.fa", "");
    diff_files(pref, ptgt, seek1, seek2);
    return 0;

    size_t sz1, sz2;
    // "data/HuRef/hs_alt_Celera_chr18.fa"
    const char *map1 = (const char*)read_file("data/chr/chr17.fa", &sz1);
    const char *map2 = (const char*)read_file("data/HuRef/hs_alt_Celera_chr17.fa", &sz2);

    printf("Files are of size %d & %d\n", sz1, sz2);

    vpii_t out;
    vpiui_t sh;
    int bsz = atoi(argv[1]);
    printf("bsz: %d\n", bsz);

    find_matches(map1, map2, bsz, out, sh);

    long long ctr = 0;
    int lower = 0, higher = 0;
    for (size_t i = 0; i < out.size(); ++i) {
        int count = out[i].second - out[i].first;
#if 0
        if (count < 50) {
            ctr += count;
            // printf("i: %d, matches: %d\n", i, count);
        }
#else
        if (count > 50) {
            continue;
        }
        vi_t off;
        for (int j = out[i].first; j < out[i].second; ++j) {
            off.push_back(sh[j].INDEX);
        }
        std::sort(off.begin(), off.end());

        for (int j = 0; j < off.size(); ++j) {
            // int k = sh[j].INDEX;
            int k = off[j];

            if (0 || !strncmp(map2+i, map1+k, bsz)) {
                // printf("map2[%d] & map1[%d] are the same\n", i, k);
                if (!lower) {
                    lower = i;
                    i = out.size() - 2*1048576;
                    printf("source offset: %d, target offset: %d\n", k, i);
                }
                higher = i;
                ctr += 1;
            }
            // ctr += out[i].second - out[i].first;
        }
#endif
    }

    printf("Total number of overlaps of size %d are %llu\n", bsz, ctr);
    printf("Lower: %d, Higher: %d\n", lower, higher);
    // Total number of overlaps of size 100 are 19962792
    // Total number of overlaps of size 200 are 19910511
    // Total number of overlaps of size 300 are 19970779

#endif

}
