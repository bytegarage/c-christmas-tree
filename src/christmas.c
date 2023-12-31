#include <stdio.h>
#include <stdlib.h>
#include "console.c"

/* --------------------------------------------------------
* Math helper functions
-------------------------------------------------------- */

double random01() {
    return (double)random() / RAND_MAX;
}

double random_min_max(double min, double max) {
    return random01() * (max - min) + min;
}

void plot(char *fstr) {
    char *p = fstr;
    while(*p) {
        if(*p > 0) {
            set_color(color_green);
            putc(*p, stdout);
        } else {
            unsigned char color = -(*p * 2);
            set_color_rgb(color, 0, 0);
            putc('*', stdout);
            (*p)-=5;
            if (*p < -100)
                *p = -1;
        }
        p++;
    }
    fflush(stdout);
}

char* load_happiness(const char* fname, char *buffer, size_t size) {
    FILE *f = fopen(fname, "r");
    char *p = buffer;
    size_t free_buffer = size - 1;

    if (f == NULL)
        return NULL;

    while((fgets(p, free_buffer, f)) != NULL) {
        int sl = strlen(p);
        free_buffer -= sl;
        p += sl;
    }
    *p = '\0';

    return buffer;
}

int main() {
    char str[10000];
    char line[sizeof(str)];

    if(load_happiness("tree.txt", str, sizeof(str)) == NULL) {
        printf("\nFile not found!\n");
        return EXIT_FAILURE;
    }

    char *pc = str;
    char *pl = line;

    while(*pc) {
        *pl++ = (*pc != '$') ? *pc : (char)random_min_max(-100, -1);
        pc++;
    }
    *pl = '\0';

    int i = 0;
    clrscr();
    while (i++ < 200)
    {
        gotoxy(1, 1);
        plot(line);
        sleepms(100);
    }

    return EXIT_SUCCESS;
}