#ifndef _LINUX_H
#define _LINUX_H

struct battery{
	int design_capacity;
	int charge_full;
	int charge_now;
	int charge_percent;
	char *charge_status;
};

struct batteries{
	struct battery battery[4];
	int num_of_batts;
};

char *get_username(char *returnptr);
char *get_hostname(char *returnptr);
char *get_osname(char *returnptr);
char *get_hwname(char *returnptr, char bitmask);
char *get_kernel(char *returnptr);

long get_uptime();

char *get_shell(char *returnptr);
char *get_screenres(char *returnptr);
char *get_de(char *returnptr);
char *get_disp_protocol(char *returnptr);
char *get_terminal(char *returnptr);
char *get_cpuname(char *returnptr);
char *get_gpuname(char *returnptr);

long get_ramused();
long get_ramtotal();
long get_swapused();
long get_swaptotal();
unsigned long get_diskused();
unsigned long get_disktotal();

struct batteries get_battery();
char *get_ip(char *returnptr, char bitmask);

#endif
