//
// Created by os on 8/20/26.
//

#ifndef MICKO_VEZBE7_PRINT_H
#define MICKO_VEZBE7_PRINT_H

#include "RiscV.hpp"
#include "../lib/hw.h"
#include "../lib/console.h"

inline void printStringK(char const *string) {
    uint64 sstatus = RiscV::r_sstatus();
    RiscV::mc_sstatus(RiscV::SSTATUS_SIE);

    while (*string != '\0') {
        __putc(*string);
        string++;
    }

    RiscV::ms_sstatus(sstatus & RiscV::SSTATUS_SIE ? RiscV::SSTATUS_SIE : 0);
}

inline void printIntegerK(uint64 integer) {
    uint64 sstatus = RiscV::r_sstatus();
    RiscV::mc_sstatus(RiscV::SSTATUS_SIE);

    static char digits[] = "0123456789";
    char buf[16];
    int i, neg;
    uint x;

    neg = 0;
    if (integer < 0) {
        neg = 1;
        x = -integer;
    } else {
        x = integer;
    }

    i = 0;
    do {
        buf[i++] = digits[x % 10];
    } while ((x /= 10) != 0);
    if (neg) buf[i++] = '-';

    while (--i >= 0) __putc(buf[i]);

    RiscV::ms_sstatus(sstatus & RiscV::SSTATUS_SIE ? RiscV::SSTATUS_SIE : 0);
};

#endif //MICKO_VEZBE7_PRINT_H
