#include <stdio.h>
#include <tohex.h>

void print_hex(char *p, int s)
{
    char map[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                     '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    char *from = (char *)p;
    char *to = from + s;
    while (from < to) {
        char a = (*from & 0xf0) >> 4;
        char b = (*from & 0x0f);
        printf("%c%c", map[a], map[b]);
        ++from;
    }
    puts("");
}
