#include <stdlib.h>
#include <sys/utsname.h>
#include <stdio.h>
#include "vega.h"

char *get_username(){
    return getenv("USER");
}

char *get_hostname(){
    struct utsname buffer;
    uname(&buffer);
    return buffer.nodename;
}

char *get_osname();

char *get_hwname();

char *get_kernel(){
    struct utsname buffer;
    uname(&buffer);
    
    char *kernelname;
    asprintf(&kernelname, "%s %s", buffer.sysname, buffer.version);
    return kernelname;
}

char *get_uptime();

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

char *get_ram();

char *get_gpudriver();

char *get_df();

char *get_battery();

char *get_song();

char *get_localip();

char *get_ipv4();

char *get_ipv6();

