#include <stdio.h>
#include <assert.h>
#include <include/fasta.hpp>

using namespace std;

#define BUFF_SIZE 8192

char buff[BUFF_SIZE];

int
main(int argc, char *argv[]) {
    FastaReader fr(stdin);
    int r = fr.read(buff, BUFF_SIZE-1);
    while (r) {
        int i = 0, o = 0;
        while (i < r) {
            if (buff[i] != 'N' && buff[i] != 'n') {
                buff[o++] = toupper(buff[i++]);
            }
            else {
                ++i;
            }
        }
        buff[o] = '\0';
        printf("%s", buff);
        r = fr.read(buff, BUFF_SIZE);
    }
}
