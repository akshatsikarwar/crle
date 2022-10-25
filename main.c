#include <stddef.h>
#include <stdio.h>
#include <comdb2rle.h>

#define HAVE_10K

int main()
{
    #pragma pack(1)
    struct row {
        uint8_t h0;
        int32_t c0;

        uint8_t h1;
        uint16_t c1;

        uint8_t h2; 
        uint8_t c2[5000];

#       ifdef HAVE_10K
        uint8_t h3; 
        uint8_t c3[10000];
#       endif // HAVE_10K
    };
    #pragma pack()

    struct row in = {0};
    in.h0 = 0x02;
    in.h1 = 0x02;
    in.h2 = 0x02;
#   ifdef HAVE_10K
    in.h3 = 0x02;
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
