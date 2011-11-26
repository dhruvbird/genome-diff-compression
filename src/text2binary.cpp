#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <include/utils.hpp>

using namespace std;

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
            fwrite(&soff, sizeof(soff), 1, pout);
            if (len < 128) {
                uint8_t clen = len;
                clen |= 128;
                fwrite(&clen, sizeof(clen), 1, pout);
            }
            else if (len < 16384) {
                uint16_t slen = len;
                len |= 16384;
                fwrite(&slen, sizeof(slen), 1, pout);
            }
            else {
                fwrite(&len, sizeof(len), 1, pout);
            }
        }
        fclose(fin);
    }
    fclose(pout);
}
