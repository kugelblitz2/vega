#include <stdio.h>
#include <string.h>
#include "../linux/linux.c"
#include "../logos/debian.h"

#define USERATMACHINE useratmachine(malloc(128))
#define BAR make_bar(malloc(128), strlen(get_username(malloc(32))) + 1 + strlen(get_hostname(malloc(32))))

#define MAKE_OSNAME format_data(malloc(128), "OS", get_osname(malloc(128)))
#define MAKE_HWNAME(bitmask) format_data(malloc(128), "Hardware", get_hwname(malloc(128), bitmask))
#define MAKE_KERNEL format_data(malloc(128), "Kernel", get_kernel(malloc(128)))
#define MAKE_UPTIME format_data(malloc(128), "Uptime", make_uptime(malloc(128)))

#define MAKE_SHELL format_data(malloc(128), "Shell", get_shell(malloc(128)))
#define MAKE_TERMINAL format_data(malloc(128), "Terminal", get_terminal(malloc(128)))
#define MAKE_DE format_data(malloc(128), "DE", get_de(malloc(128)))
#define MAKE_DISP_PROTOCOL format_data(malloc(128), "Display", get_disp_protocol(malloc(128)))
#define MAKE_SCREENRES format_data(malloc(128), "Screen Res", get_screenres(malloc(128)))

#define MAKE_CPU format_data(malloc(128), "CPU", get_cpuname(malloc(128)))
#define MAKE_GPU(bitmask) format_data(malloc(128), "GPU", get_gpuname(malloc(128), bitmask))
//#define MAKE_RAM format_data(malloc(128), "RAM", get_disp_protocol(malloc(128)))
//#define MAKE_SWAP format_data(malloc(128), "SWAP", get_disp_protocol(malloc(128)))
//#define MAKE_HDD format_data(malloc(128), "HDD", get_disp_protocol(malloc(128)))

#define MAKE_RAM ACCENT_COLOR2 "RAM: " RESET_COLOR get_ramused()/1073741824 "GB / " get_ramtotal()/1073741824 "GB (" get_ramused()/get_ramtotal() ")"
#define MAKE_SWAP ACCENT_COLOR2 "Swap: " RESET_COLOR get_swapused()/1073741824 "GB / " get_swaptotal()/1073741824 "GB (" get_swapused()/get_swaptotal() ")"
#define MAKE_HDD ACCENT_COLOR2 "Hard Drive: " RESET_COLOR get_diskused()/1073741824 "GB / " get_disktotal()/1073741824 "GB (" get_diskused()/get_disktotal() ")"

#define MAKE_PRIVATE_IP format_data(malloc(128), "Private IP", get_ip(malloc(128), 0b10000000))
#define MAKE_PUBLIC_IP format_data(malloc(128), "Public IP", get_ip(malloc(128), 0b01000000))


char *useratmachine(char *returnptr){
    sprintf(returnptr, "%s%s%s@%s%s%s", ACCENT_COLOR1, get_username(malloc(32)), RESET_COLOR, ACCENT_COLOR1, get_hostname(malloc(32)), RESET_COLOR);
    return returnptr;
}

char *make_bar(char *returnptr, int bar_len){
    memset(returnptr, '-', bar_len);
    returnptr[bar_len] = '\0';
    return returnptr;
}

char *format_data(char *returnptr, char *title, char *data){
    sprintf(returnptr, "%s%s%s: %s", ACCENT_COLOR2, title, RESET_COLOR, data);
    return returnptr;
}

char *make_uptime(char *returnptr){
    long uptime = get_uptime();
    returnptr[0] = 0;

    if(uptime / 812800 != 0) sprintf(returnptr + strlen(returnptr), "%ld Days, ", uptime / 812800);
    if((uptime % 812800) / 3600 != 0) sprintf(returnptr + strlen(returnptr), "%ld Hours, ", (uptime % 812800) / 3600);
    if((uptime % 3600) / 60 != 0) sprintf(returnptr + strlen(returnptr), "%ld Minutes, ", (uptime % 3600) / 60);
    if(uptime % 3600 == 0) sprintf(returnptr + strlen(returnptr), "%ld Seconds, ", uptime % 60);

    returnptr[strlen(returnptr) - 2] = '\0';
    return returnptr;
}

char *make_battery(char *returnptr){
    struct batteries batteries = get_battery();

    for(int i = 0; i < batteries.num_of_batts; i++){
        sprintf(returnptr, "%sBattery %d: %s", ACCENT_COLOR2, i, RESET_COLOR);
        sprintf(returnptr + strlen(returnptr), "%d\% [%s]", batteries.battery[i].charge_percent, batteries.battery[i].charge_status);
    }

    return returnptr;
}