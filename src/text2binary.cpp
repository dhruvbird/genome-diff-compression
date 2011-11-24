#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <include/utils.hpp>

using namespace std;

int
main() {
    const char *tmpl = "out22.%d.txt";
    char fbuff[1024];
    int code, doff, soff, len;

    sprintf(fbuff, tmpl, 0);
    FILE *pout = fopen(fbuff, "w");
    assert(pout);

    for (int i = 1; i < 100; ++i) {
        sprintf(fbuff, tmpl, i);
        if (!file_exists(fbuff)) {
            break;
        }
        FILE *fin = fopen(fbuff, "r");
        assert(fin);
        fscanf(fin, "%d %d %d %d", &code, &doff, &soff, &len);
        while (!feof(fin)) {
            fscanf(fin, "%d %d %d %d", &code, &doff, &soff, &len);
            fwrite(&soff, sizeof(soff), 1, pout);
            if (len < 64) {
                char clen = len;
                clen |= (1 << 7);
                fwrite(&clen, sizeof(clen), 1, pout);
            }
            else {
                fwrite(&len, sizeof(len), 1, pout);
            }
        }
        fclose(fin);
    }
    fclose(pout);
}