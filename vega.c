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
}

char *get_packages();

char *get_shell();

char *get_screenres();

char *get_de();

char *get_wm();

char *get_terminal();

char *get_termfont();

char *get_cpu();

char *get_gpu();

char *get_vram();

char *get_ramused();

char *get_ramtotal();

char *get_gpudriver();

char *get_df();

char *get_battery();

char *get_song();

char *get_localip();

char *get_ipv4();

char *get_ipv6();

