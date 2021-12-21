/*
* CUTX - ByteGarage CONSOLE UTILITY LIBRARY
* Author: Byte Garage
*
*/
#ifndef __CUTX__
#define __CUTX__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "chrono.c"
#include "console_color.c"

bool verify(const char *test_name, bool test_result) {
    printf("%s --> ", test_name);
    if(test_result)
        printf_ok("OK\n");
    else
        printf_error("ERROR\n");

    return test_result;
}

void print_chrono(chrono_t *c) {
    printf("Chrono %s time: %.2fs\n", c->name, get_chrono_elapsed(c));
}

void print_chrono_lap(chrono_t *c) {
    printf("Chrono %s lap: %.2fs\n", c->name, get_chrono_lap(c));
}

/* --------------------------------------------------------
* Debug
* -------------------------------------------------------- */

void _print_cwd(){
    char *path = getcwd(NULL, 0);
    printf("current directory: %s\n", path);
    free(path);
}

void _print_reverse(const char *s) {
    putc('\n', stdout);
    for (int i = strlen(s); i >= 0; i--)
        putc(s[i], stdout);

    putc('\n', stdout);
}

/* --------------------------------------------------------
* ASCII Image Print
* -------------------------------------------------------- */

const char *GRAYSCALE = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
const int GRAYSCALE_LEN = 69;

char get_shade(unsigned char pixel) {
    return GRAYSCALE[(int)pixel * GRAYSCALE_LEN / 255];
}

void plot_img(const unsigned char* img, int row, int col) {
    const unsigned char *p = img;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++) {
            putc(get_shade(*p++), stdout);
        }
        putc('\n', stdout);
    }
}


/* --------------------------------------------------------
* Progress bar
* -------------------------------------------------------- */

typedef struct progress_bar
{
    int min;
    int max;
    int value;
    int step;
    int status; // 0 normal, ...
} progress_bar;


progress_bar pb_create(int min, int max, int value, int step) {
    return (progress_bar) {min, max, value, step, 0};
}

progress_bar* pb_step(progress_bar *pb) {
    pb->value += pb->step;
    if(pb->value > pb->max)
        pb->value = pb->max;
    if(pb->value < pb->min)
        pb->value = pb->min;
    return pb;
}

progress_bar* pb_show(const char* lbl, progress_bar* pb, int size) {
    int d = pb->max - pb->min;
    int nv = (pb->value - pb->min) / (double)d * size;
    char pbstr[size + 1];
    memset(pbstr, '#', nv);
    memset(pbstr + nv, '-', size - nv);
    pbstr[size] = '\0';
    printf("\r%s: [%s]", lbl, pbstr);
    return pb;
}


/******************************************************************************
*
*   INPUT SECTION
*
******************************************************************************/

#include <termios.h>

struct termios tty_opts_backup;
bool raw_mode_active;
/*
*   Setting rawmode disable the input cache, so p.e. getch can return immediately the 
*   first character pressed
*/
void stdin_raw_mode(bool activate)
{
    if(!activate && raw_mode_active) 
    {
        // Restore previous TTY settings
        tcsetattr(STDIN_FILENO, TCSANOW, &tty_opts_backup);
        raw_mode_active = false;
    }
    else if(activate)
    {
        struct termios raw_mode;
        if (!isatty(STDIN_FILENO))
        {
            exit(1);
        }

        // Back up current TTY settings
        tcgetattr(STDIN_FILENO, &tty_opts_backup);

        // Change TTY settings to raw mode
        cfmakeraw(&raw_mode);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw_mode);

        raw_mode_active = true;
    }
}

/******************************************************************************
*
*   TIME SECTION
*
******************************************************************************/
#include <time.h>
void sleepms(long ms) {
    struct timespec ts, tr;
    ts.tv_sec = 0;
    ts.tv_nsec = ms * (long)1000000;
    nanosleep(&ts, &tr);
}
#endif