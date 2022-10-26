#include <arpa/inet.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <comdb2rle.h>
#include <tohex.h>

#define data 0x08
#define null 0x02

int main()
{
    #pragma pack(1)
    struct row {
        uint8_t h1; /* header 1 */
        short c1; //uint8_t c1[2]; /* column 1 */

        uint8_t h2; 
        struct {
            int32_t len;
            uint8_t dta[1000];
        } c2;

        uint8_t h3; 
        struct {
            int32_t len;
            uint8_t dta[500];
        } c3;
    };
    #pragma pack()

    struct row in =  {
        .h1 = data,
        .c1 = 0,

        .h2 = data,
        .c2 = { .len = htonl(strlen("hi") + 1), .dta = "hi"},

        .h3 = data,
        .c3 = { .len = htonl(strlen("hj") + 1), .dta = "hj"},
    };

    char out[sizeof(in) / 2];
    char out_in[sizeof(in)];

    uint16_t hints[] = { 
        sizeof(in.c1) + 1,
        sizeof(in.c2) + 1,
        sizeof(in.c3) + 1,
        0
    };

    printf("hints: ");
    uint16_t *h = hints;
    while (*h) {
        printf("%d ", *h);
        ++h;
    }
    puts("");

    Comdb2RLE c = { .in = (uint8_t *)&in, .insz = sizeof(in), .out = out, .outsz = sizeof(out) };

    int rc;
    puts("compressComdb2RLE_hints");
    rc = compressComdb2RLE_hints(&c, hints);
    if (rc != 0) {
        fprintf(stderr, "compressComdb2RLE_hints failed\n");
        return 1;
    }
    puts("\ndecompressComdb2RLE");
    Comdb2RLE d = { .in = (uint8_t *)out, .insz = c.outsz, .out = out_in, .outsz = sizeof(out_in) };
    rc = decompressComdb2RLE(&d);
    if (rc != 0) {
        fprintf(stderr, "decompress failed\n");
        return 2;
    }
    if (memcmp(&in, out_in, sizeof(in)) != 0) {
        fprintf(stderr, "decompress mismatch\n");
        return 3;
    }
    printf("\nsuccess %zu -> %zu -> %zu\n", c.insz, c.outsz, d.outsz);
    return 0;
}
