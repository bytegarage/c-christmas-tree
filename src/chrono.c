/*
* CHRONO - ByteGarage Chrono Library
* Author: Byte Garage
*
*
*/

#ifndef __CHRONO__
#define __CHRONO__

#include <time.h>

typedef struct chrono_st {
    clock_t start;
    clock_t end;
    clock_t lap_start;
    clock_t lap_end;
    char *name;
} chrono_t;

chrono_t* chrono_restart(chrono_t *t) {
    t->start = clock();
    t->lap_start = t->start;
    t->lap_end = 0;
    t->end = 0;
    return t;
}

chrono_t chrono_start() {
    chrono_t t;
    chrono_restart(&t);
    t.name = NULL;
    return t;
}

chrono_t* chrono_lap(chrono_t *t) {
    if(t->lap_end != 0)
        t->lap_start = t->lap_end;
    t->lap_end = clock();

    return t;
}

chrono_t* chrono_stop(chrono_t *t) {
    t = chrono_lap(t);
    t->end = t->lap_end;
    return t;
}

double get_chrono_elapsed(chrono_t *t) {
    return (((t->end != 0) ? t->end : clock()) - t->start) / (double)CLOCKS_PER_SEC;
}

double get_chrono_lap(chrono_t *t) {
    return (((t->lap_end != 0) ? t->lap_end : clock()) - t->lap_start) / (double)CLOCKS_PER_SEC;
}

#endif