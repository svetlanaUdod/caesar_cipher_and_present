//
// Created by admin on 17.12.2020.
//

#ifndef LAB3_CEASAR_H
#define LAB3_CEASAR_H

#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 80

struct char_interval {
    char begin;
    char end;
};

/* All possible character intervals, such as printable ASCII characters */
static struct char_interval CHAR_INTERVALS[] = {
        { .begin = ' ', .end = '~' }
};

void caesar(char* dst, char* src, int shift);
char shift_char(char c, int shift);
int is_from_interval(char c, struct char_interval interval);
char shift_generic_char(char c, int shift, char base, char length);

#endif //LAB3_CEASAR_H


