#include <stdlib.h>
#include <sys/utsname.h>
#include <stdio.h>
#include "vega.h"

#ifdef __linux__
#include <sys/sysinfo.h>
#endif
#ifdef __APPLE__
#include <sys/sysctl.h>
#endif

char *get_username(){
    return getenv("USER");
}

char *get_hostname(){
    struct utsname basicinfo;
    uname(&basicinfo);
    return basicinfo.nodename;
}

char *get_osname();

char *get_hwname();

char *get_kernel(){
    struct utsname basicinfo;
    uname(&basicinfo);
    
    char *kernelname;
    asprintf(&kernelname, "%s %s", basicinfo.sysname, basicinfo.version);
    return kernelname;
}

long *get_uptime(){

    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return linuxinfo.uptime;
    #elif defined(__APPLE__)
        struct timespec uptime;
        clock_gettime(CLOCK_MONOTONIC_RAW, &uptime);

        return uptime.tv_sec;
    #endif
    // returns LONG_MIN if system is unsupported
    return LONG_MIN;
}

char *get_packages();

char *get_shell();

char *get_screenres();

char *get_de();

char *get_wm();

char *get_terminal();

char *get_termfont();

char *get_cpuname();

int *get_cpuusage(){
    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return (int) (100 * linuxinfo.loads[0]) / (get_nprocs() * SI_LOAD_SHIFT);
    #endif
}

char *get_gpuname();

char *get_vramused();

char *get_vramtotal();

long *get_ramused(){
    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return linuxinfo.totalram - linuxinfo.freeram;
    #endif
}

long *get_ramtotal(){
    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return linuxinfo.totalram - (linuxinfo.freeram + linuxinfo.bufferram);
    #endif
}

long *get_swapused(){
    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return linuxinfo.totalswap - linuxinfo.freeswap;
    #endif
}

long *get_swaptotal(){
    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return linuxinfo.totalswap;
    #endif
}

char *get_gpudriver();

char *get_df();

char *get_battery();

char *get_song();

char *get_localip();

char *get_ipv4();

char *get_ipv6();

