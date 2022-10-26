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
        uint8_t h00; /* header 00 */
        int32_t c00; //uint8_t c00[4]; /* column 00 */

#       ifdef HAVE_REPEAT
        uint8_t h01; /* header 01 */
        int32_t c01; //uint8_t c01[4]; /* column 01 */
#       endif

        uint8_t h1; /* header 1 */
        short c1; //uint8_t c1[2]; /* column 1 */

        uint8_t h2; 
        uint8_t c2[5000];

#       ifdef HAVE_10K
        uint8_t h3; 
        uint8_t c3[10000];
#       endif // HAVE_10K
    };
    #pragma pack()

    struct row in =  {
        .h00 = data,
        //.c00 = {0x80, 0, 0, 0}, /* +0 */
        //.c00 = {0xde, 0xad, 0xbe, 0xef}, /* +0 */
        .c00 = 99,

#       ifdef HAVE_REPEAT
            .h01 = data,
            //.c01 = {0x80, 0, 0, 0}, /* +0 */
            //.c01 = {0xde, 0xad, 0xbe, 0xef}, /* +0 */
            .c01 = 99,
#       endif /* HAVE_REPEAT */

        .h1 = data,
        //.c1 = {0x7f, 0xff}, /* -1 */
        //.c1 = {0xca, 0xfe}, /* -1 */
        .c1 = 999,

#       ifdef VUTF8_DATA
            .h2 = data,
            .c2 = "hello",
#       else
            .h2 = null,
#       endif /* VUTF8_DATA */

#       ifdef HAVE_10K
#           ifdef VUTF8_DATA
                .h3 = data,
                .c3 = "world"
#           else
                .h3 = null,
#           endif /* VUTF8_DATA */
#       endif /* HAVE_10K */
    };
    //print_hex(&in, sizeof(in));

    char out[sizeof(in) / 2];
    char out_in[sizeof(in)];

    uint16_t hints[] = { sizeof(in.c00) + 1,
#                        ifdef HAVE_REPEAT
                             sizeof(in.c01) + 1,
#                        endif /* HAVE_REPEAT */
                         sizeof(in.c1) + 1,
                         sizeof(in.c2) + 1,
#                        ifdef HAVE_10K
                             sizeof(in.c3) + 1,
#                        endif /* HAVE_10K */
                         0
                        };

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
    printf("\nsuccess %d -> %d -> %d\n", c.insz, c.outsz, d.outsz);
    return 0;
}
