#ifndef INCLUDE_TOHEX_H
#define INCLUDE_TOHEX_H
#define print_hex(p, s)                                                                            \
    ({                                                                                             \
        char map[] = {'0', '1', '2', '3', '4', '5', '6', '7',                                      \
                      '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};                                     \
        char *from = p;                                                                            \
        char *to = p + s;                                                                          \
        while (from < to) {                                                                        \
            char a = (*from & 0xf0) >> 4;                                                          \
            char b = (*from & 0x0f);                                                               \
            printf("%c%c\n", map[a], map[b]);                                                      \
            ++from;                                                                                \
        }                                                                                          \
        puts("");                                                                                  \
    })
#endif //INCLUDE_TOHEX_H
