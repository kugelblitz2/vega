#include <stdio.h>
#include <string.h>
#include "../linux/linux.c"
#include "../logos/debian.h"

#define USERATMACHINE ACCENT_COLOR1 get_username(malloc(64)) RESET_COLOR "@" ACCENT_COLOR1 get_hostname(malloc(64)) RESET_COLOR

#define MAKE_OSNAME ACCENT_COLOR2 "OS: " RESET_COLOR get_osname(malloc(64))
#define MAKE_HWNAME(bitmask) ACCENT_COLOR2 "Hardware: " RESET_COLOR get_hwname(malloc(64), bitmask)
#define MAKE_KERNEL ACCENT_COLOR2 "Kernel: " RESET_COLOR get_kernel(malloc(64))

#define MAKE_SHELL ACCENT_COLOR2 "Shell: " RESET_COLOR get_shell(malloc(64))
#define MAKE_TERMINAL ACCENT_COLOR2 "Terminal: " RESET_COLOR get_terminal(malloc(64))
#define MAKE_DE ACCENT_COLOR2 "DE: " RESET_COLOR get_de(malloc(64))
#define MAKE_DISP_PROTOCOL ACCENT_COLOR2 "Display: " RESET_COLOR get_disp_protocol(malloc(64))

#define MAKE_CPU ACCENT_COLOR2 "CPU: " RESET_COLOR get_cpuname(malloc(64))
#define MAKE_GPU(bitmask) ACCENT_COLOR2 "GPU: " RESET_COLOR get_gpuname(malloc(64), bitmask)
#define MAKE_RAM ACCENT_COLOR2 "RAM: " RESET_COLOR get_ramused()/1073741824 "GB / " get_ramtotal()/1073741824 "GB (" get_ramused()/get_ramtotal() ")"
#define MAKE_SWAP ACCENT_COLOR2 "Swap: " RESET_COLOR get_swapused()/1073741824 "GB / " get_swaptotal()/1073741824 "GB (" get_swapused()/get_swaptotal() ")"
#define MAKE_HDD ACCENT_COLOR2 "Hard Drive: " RESET_COLOR get_diskused()/1073741824 "GB / " get_disktotal()/1073741824 "GB (" get_diskused()/get_disktotal() ")"

char *make_bar(char *returnptr, int bar_len){
    memset(returnptr, '-', bar_len);
    returnptr[bar_len] = '\n';
    returnptr[bar_len+1] = '\0';
    return returnptr;
}

char *make_uptime(char *returnptr){
    long uptime = get_uptime;
    sprintf(returnptr, "%sUptime: %s", ACCENT_COLOR2, RESET_COLOR);

    if(uptime % 86400 != 0) sprintf(returnptr + strlen(returnptr), "Days: %ld, ", uptime / 86400);
    if((uptime % 86400) % 3600 != 0) sprintf(returnptr + strlen(returnptr), "Hours: %ld, ", (uptime % 86400) / 3600);
    if((uptime % 3600) % 60 != 0) sprintf(returnptr + strlen(returnptr), "Minutes: %ld, ", (uptime % 3600) / 60);
    if(uptime % 3600 == 0) sprintf(returnptr + strlen(returnptr), "Seconds: %ld, ", uptime % 60);

    returnptr[strlen(returnptr) - 2] = '\0';
    return returnptr;
}