#ifndef INCLUDE_TOHEX_H
#define INCLUDE_TOHEX_H
#define print_hex(p, s)                                                        \
    ({                                                                         \
        uint8_t map[] = {'0', '1', '2', '3', '4', '5', '6', '7',               \
                         '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};              \
        uint8_t *from = (char *)p;                                             \
        uint8_t *to = from + s;                                                \
        while (from < to) {                                                    \
            uint8_t a = (*from & 0xf0) >> 4;                                   \
            uint8_t b = (*from & 0x0f);                                        \
            printf("%c%c", map[a], map[b]);                                    \
            ++from;                                                            \
        }                                                                      \
        puts("");                                                              \
    })
#endif //INCLUDE_TOHEX_H
