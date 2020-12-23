//
// Created by admin on 17.12.2020.
//

#include "ceasar.h"

void caesar(char* dst, char* src, int shift) {
    int i;

    for (i = 0; i < strlen(src); i++) {
        dst[i] = shift_char(src[i], shift);
    }
    dst[i] = '\0';
}

char shift_char(char c, int shift) {
    char base;
    int length;

    int num_intervals = sizeof(CHAR_INTERVALS)/sizeof(struct char_interval);

    for (int i = 0; i < num_intervals; i++) {
        if (is_from_interval(c, CHAR_INTERVALS[i])) {
            base = CHAR_INTERVALS[i].begin;
            length = CHAR_INTERVALS[i].end - CHAR_INTERVALS[i].begin + 1;
            return shift_generic_char(c, shift, base, length);
        }
    }

    return ' ';
}

int is_from_interval(char c, struct char_interval interval) {
    return (c >= interval.begin && c <= interval.end);
}

char shift_generic_char(char c, int shift, char base, char length) {
    int pos = c - base;
    int delta  = (pos + shift) % length;

    if (delta < 0)
        delta += length;

    return(base + delta);
}

