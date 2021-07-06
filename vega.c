#include "common/corefuncs.c"

int main(){
	
	char *username = malloc(256);
	char *hostname = malloc(256);
	char *os_name = malloc(256);
	char *hwname = malloc(256);
	char *kernel = malloc(256);
	
	char *shell = malloc(256);
	char *screenres = malloc(256);
	char *de = malloc(256);
	char *disp_protocol = malloc(256);
	char *terminal = malloc(256);
	char *cpuname = malloc(256);
	char *gpuname = malloc(256);
	
	char *ip = malloc(256);

	
	printf("%s\n", USERATMACHINE);
	printf("%s\n", BAR);

	printf("%s\n", MAKE_OSNAME);
	printf("%s\n", MAKE_HWNAME(0b11110000));
	printf("%s\n", MAKE_KERNEL);
	printf("%s\n", MAKE_UPTIME);

	printf("%s\n", MAKE_SHELL);
	printf("%s\n", MAKE_TERMINAL);
	printf("%s\n", MAKE_DE);
	printf("%s\n", MAKE_DISP_PROTOCOL);
	printf("%s\n", MAKE_SCREENRES);

	printf("%s\n", MAKE_CPU);
	printf("%s\n", MAKE_GPU(0b11000000));

	printf("ramused %ld\n", get_ramused());
	printf("ramtotal %ld\n", get_ramtotal());
	printf("swapused %ld\n", get_swapused());
	printf("swaptotal %ld\n", get_swaptotal());
	printf("diskused %ld\n", get_diskused());
	printf("disktotal %ld\n", get_disktotal());
	
	struct batteries batteries = get_battery();
	
	for(int i = 0; i < batteries.num_of_batts; i++){
		printf("battery%d %d %d %d %d %s\n", i, batteries.battery[i].design_capacity, 
			batteries.battery[i].charge_full, batteries.battery[i].charge_now, 
			batteries.battery[i].charge_percent, batteries.battery[i].charge_status);
	}
	
	printf("%s\n", MAKE_PRIVATE_IP);
	printf("%s\n", MAKE_PUBLIC_IP);
	return 0;
}
