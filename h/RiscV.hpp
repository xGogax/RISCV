//
// Created by os on 8/24/26.
//

#ifndef PROJECT_BASE_V1_1_RISCV_HPP
#define PROJECT_BASE_V1_1_RISCV_HPP

#include "../lib/hw.h"

class RiscV {
public:

    // pop sstatus.spp and status.spie bits (has to be a non inline function)
    static void popSppSpie();

    // read register scause
    static uint64 r_scause();

    // write register scause
    static void w_scause(uint64 scause);

    // read register sepc
    static uint64 r_sepc();

    // write register sepc
    static void w_sepc(uint64 sepc);

    // read register stvec
    static uint64 r_stvec();

    // write register stvec
    static void w_stvec(uint64 stvec);

    // read register stval
    static uint64 r_stval();

    // write register stval
    static void w_stval(uint64 stval);

    enum BitMaskSip {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5),
        SIP_SEIP = (1 << 9),
    };

    // mask set register sip
    static void ms_sip(uint64 mask);

    // mask clear register sip
    static void mc_sip(uint64 mask);

    // read register sip
    static uint64 r_sip();

    // write register sip
    static void w_sip(uint64 sip);

    enum BitMaskStatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_STIE = (1 << 5),
        SSTATUS_SEIE = (1 << 8),
    };

    // mask set register sstatus
    static void ms_sstatus(uint64 mask);

    // mask clear register sstatus
    static void mc_sstatus(uint64 mask);

    // read register sstatus
    static uint64 r_sstatus();

    // write register sstatus
    static void w_sstatus(uint64 sstatus);

    // supervisor trap
    static void supervisorTrap();

    // system codes
    enum sysCode {
        // Memory management
        SYS_MEM_ALLOC           = 0x01,
        SYS_MEM_FREE            = 0x02,

        // Thread management
        SYS_THREAD_CREATE       = 0x11,
        SYS_THREAD_EXIT         = 0x12,
        SYS_THREAD_DISPATCH     = 0x13,
        SYS_TIME_SLEEP          = 0x31,

        // Semaphore management
        SYS_SEM_OPEN            = 0x21,
        SYS_SEM_CLOSE           = 0x22,
        SYS_SEM_WAIT            = 0x23,
        SYS_SEM_SIGNAL          = 0x24,
        SYS_SEM_WAIT_N          = 0x25,
        SYS_SEM_SIGNAL_N        = 0x26,

        // Console management
        SYS_GET_C               = 0x41,
        SYS_PUT_C               = 0x42
    };
private:

    // supervisor trap handler
    static void handleSupervisorTrap();
};

inline uint64 RiscV::r_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void RiscV::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 RiscV::r_sepc() {
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void RiscV::w_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 RiscV::r_stvec() {
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void RiscV::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 RiscV::r_stval() {
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void RiscV::w_stval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void RiscV::ms_sip(uint64 mask) {
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask) {
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sip() {
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void RiscV::w_sip(uint64 sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void RiscV::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sstatus(uint64 mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void RiscV::w_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

#endif //PROJECT_BASE_V1_1_RISCV_HPP
