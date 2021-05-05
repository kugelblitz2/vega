#ifdef __APPLE__
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#include <sys/sysctl.h>
#include <stdio.h>

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
    struct timespec uptime;
    clock_gettime(CLOCK_MONOTONIC_RAW, &uptime);

    return uptime.tv_sec;

    // returns LONG_MIN if system is unsupported
    return LONG_MIN;
}

char *get_packages();

char *get_shell();

char *get_screenres();

char *get_de();

char *get_wm();

char *get_terminal();

char *get_cpuname();

int *get_cpuusage(){
}

char *get_gpuname();

char *get_vramused();

char *get_vramtotal();

long *get_ramused(){

}

long *get_ramtotal(){
}

long *get_swapused(){
}

long *get_swaptotal(){
}

char *get_gpudriver();

unsigned long *get_diskused(){
}

unsigned long *get_disktotal(){
}

char *get_battery();

char *get_localip();

char *get_ipv4();

char *get_ipv6();

#endif