#include <arpa/inet.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <comdb2rle.h>
#include <tohex.h>

#define data 0x08
#define null 0x02

#define HAVE_10K
#undef HAVE_REPEAT
#define VUTF8_DATA

int main()
{
    #pragma pack(1)
    struct row {
#       ifdef H00
            uint8_t h00; /* header 00 */
            int32_t c00; //uint8_t c00[4]; /* column 00 */
#       endif

#       ifdef H01
            uint8_t h01; /* header 01 */
            int32_t c01; //uint8_t c01[4]; /* column 01 */
#       endif

        uint8_t h1; /* header 1 */
        short c1; //uint8_t c1[2]; /* column 1 */

        uint8_t h2; 
        struct {
            int32_t len;
            uint8_t dta[1000];
        } c2;

#       ifdef HAVE_10K
        uint8_t h3; 
        struct {
            int32_t len;
            uint8_t dta[500];
        } c3;
#       endif // HAVE_10K
    };
    #pragma pack()

    struct row in =  {
#       ifdef H01 
            .h00 = data,
            //.c00 = {0x80, 0, 0, 0}, /* +0 */
            //.c00 = {0xde, 0xad, 0xbe, 0xef}, /* +0 */
            .c00 = 99,
#       endif /* H00 */

#       ifdef H01 
            .h01 = data,
            //.c01 = {0x80, 0, 0, 0}, /* +0 */
            //.c01 = {0xde, 0xad, 0xbe, 0xef}, /* +0 */
            .c01 = 99,
#       endif /* H01 */

        //.c1 = {0x7f, 0xff}, /* -1 */
        //.c1 = {0xca, 0xfe}, /* -1 */
        .h1 = data,
        .c1 = 0,

#       ifdef VUTF8_DATA
            .h2 = data,
            .c2 = { .len = htonl(strlen("hi") + 1), .dta = "hi"},
#       else
            .h2 = null,
#       endif /* VUTF8_DATA */

#       ifdef HAVE_10K
#           ifdef VUTF8_DATA
                .h3 = data,
                .c3 = { .len = htonl(strlen("hj") + 1), .dta = "hj"},
#           else
                .h3 = null,
#           endif /* VUTF8_DATA */
#       endif /* HAVE_10K */
    };
    //print_hex((char *)&in, 20);

    char out[sizeof(in) / 2];
    char out_in[sizeof(in)];

    uint16_t hints[] = { 
#       ifdef H00
            sizeof(in.c00) + 1,
#       endif /* H00 */
#       ifdef H01
            sizeof(in.c01) + 1,
#       endif /* H01 */
        sizeof(in.c1) + 1,
        sizeof(in.c2) + 1,
#       ifdef HAVE_10K
            sizeof(in.c3) + 1,
#       endif /* HAVE_10K */
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
