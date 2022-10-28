#include <arpa/inet.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <comdb2rle.h>
#include <tohex.h>

#define data 0x08
#define null 0x02

#define V1 "hi"
#define V2 "HI"

#define hdr uint8_t

int main()
{
    #pragma pack(1)
    struct row {
        hdr h1;
        short s1;

        hdr h2;
        short s2;

        hdr h3;
        struct {
            int32_t len;
            uint8_t dta[10000];
        } v1;

        hdr h4;
        short s3;

        hdr h5;
        struct {
            int32_t len;
            uint8_t dta[5000];
        } v2;
    };
    #pragma pack()

    struct row in =  {
        .h1 = data,
        .s1 = 0,

        .h2 = data,
        .s2 = 0,

        .h3 = data,
        .v1 = { .len = htonl(sizeof(V1)), .dta = V1},

        .h4 = data,
        .s3 = 1,

        .h5 = data,
        .v2 = { .len = htonl(sizeof(V2)), .dta = V2},
    };

    print_hex((char *)&in, 15);

    char out[sizeof(in) / 2];
    char out_in[sizeof(in)];

    uint16_t hints[] = { 
        sizeof(in.s1) + 1,
        sizeof(in.s2) + 1,
        sizeof(in.v1) + 1,
        sizeof(in.s3) + 1,
        sizeof(in.v2) + 1,
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
