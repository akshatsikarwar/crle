#include <stddef.h>
#include <stdio.h>
#include <comdb2rle.h>

#define data 0x08
#define null 0x02

#define HAVE_10K

int main()
{
    #pragma pack(1)
    struct row {
        uint8_t h0;
        int32_t c0;

        uint8_t h1;
        short c1;

        uint8_t h2; 
        uint8_t c2[5000];

#       ifdef HAVE_10K
        uint8_t h3; 
        uint8_t c3[5];
#       endif // HAVE_10K
    };
    #pragma pack()

    struct row in = {0};
    in.h0 = data;
    in.c0 = 99;

    in.h1 = data;
    in.c1 = 999;

    in.h2 = null;

#   ifdef HAVE_10K
    in.h3 = null;
#   endif //HAVE_10K

    uint8_t out[sizeof(in) * 2];

    Comdb2RLE c = { .in = (void *)&in, .insz = sizeof(in), .out = out, .outsz = sizeof(out) };
    uint16_t hints[] = { offsetof(struct row, h1),
                         offsetof(struct row, h2),
#                        ifdef HAVE_10K
                         offsetof(struct row, h3),
#                        endif // HAVE_10K
                         sizeof(struct row)};
    int rc = compressComdb2RLE_hints(&c, hints);
    printf("compressComdb2RLE_hints rc:%d insz:%ld outsz:%ld\n", rc, c.insz, c.outsz);

    return 0;
}
