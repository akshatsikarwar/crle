#include <arpa/inet.h>
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include <comdb2rle.h>
#include <tohex.h>

#define data 0x08
#define null 0x02

#define HAVE_10K
#undef HAVE_REPEAT
#undef VUTF8_DATA

int main()
{
    #pragma pack(1)
    struct row {
        uint8_t h00; /* header 0 */
        uint8_t c00[4]; /* column 0 */

#       ifdef HAVE_REPEAT
        uint8_t h01; /* header 0 */
        uint8_t c01[4]; /* column 0 */
#       endif

        uint8_t h1; /* header 1 */
        uint8_t c1[2]; /* column 1 */

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
        .c00 = {0x80, 0, 0, 0}, /* +0 */

#       ifdef HAVE_REPEAT
            .h01 = data,
            .c01 = {0x80, 0, 0, 0}, /* +0 */
#       endif /* HAVE_REPEAT */

        .h1 = data,
        .c1 = {0x7f, 0xff}, /* -1 */

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

    uint8_t out[sizeof(in) * 2];

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

#   if 0
    int rc = compressComdb2RLE(&c);
    printf("compressComdb2RLE rc:%d insz:%ld outsz:%ld\n", rc, c.insz, c.outsz);
#   else
    int rc = compressComdb2RLE_hints(&c, hints);
    printf("compressComdb2RLE_hints rc:%d insz:%ld outsz:%ld\n", rc, c.insz, c.outsz);
#   endif
    print_hex(&out, c.outsz);

    return 0;
}
