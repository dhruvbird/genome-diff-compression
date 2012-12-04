#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <include/utils.hpp>

using namespace std;

void
write_uint(int x, FILE *pout) {
    if (x < 128) {
        uint8_t clen = x;
        clen |= 128;
        fwrite(&clen, sizeof(clen), 1, pout);
    }
    else if (x < 16384) {
        uint16_t slen = x;
        slen |= 16384;
        fwrite(&slen, sizeof(slen), 1, pout);
    }
    else {
        fwrite(&x, sizeof(x), 1, pout);
    }
}

int
main(int argc, char *argv[]) {
    assert(argc > 1);
    const char *tmpl = "out%d.%d.txt";
    char fbuff[1024];
    int code, doff, soff, len;
    int chr_num = atoi(argv[1]);

    sprintf(fbuff, tmpl, chr_num, 0);
    FILE *pout = fopen(fbuff, "w");
    assert(pout);
    int prev_offset = 0;

    for (int i = 1; i < 100; ++i) {
        sprintf(fbuff, tmpl, chr_num, i);
        if (!file_exists(fbuff)) {
            break;
        }
        FILE *fin = fopen(fbuff, "r");
        assert(fin);
        fscanf(fin, "%d %d %d %d", &code, &doff, &soff, &len);
        while (!feof(fin)) {
            fscanf(fin, "%d %d %d %d", &code, &doff, &soff, &len);

#if 1
            fwrite(&soff, sizeof(soff), 1, pout);
#else
            // Difference encode the offsets as well.
            if (soff < prev_offset) {
                prev_offset = soff;

                // Set bit number 29 to indicate an absolute value.
                soff |= 536870912;
            }
            else {
                int temp = soff;
                soff -= prev_offset;
                prev_offset = temp;
            }
            write_uint(soff, pout);
#endif

            // Write out the length
            write_uint(len, pout);
        }
        fclose(fin);
    }
    fclose(pout);
}
