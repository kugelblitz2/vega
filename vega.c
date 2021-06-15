#include "linux/linux.c"

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
	
	char *battery = malloc(256);
	char *ip = malloc(256);


	printf("Vega Unformatted Output: \n");
	printf("username %s\n", get_username(username));
	printf("hostname %s\n", get_hostname(hostname));
	printf("osname %s\n", get_osname(os_name));
	//printf("hwname %s\n", get_hwname(hwname, 0b10000000));
	printf("kernel %s\n", get_kernel(kernel));

	printf("uptime %ld\n", get_uptime());
	
	printf("shell %s\n", get_shell(shell));
	//printf("screenres %s\n", get_screenres(screenres));
	printf("de %s\n", get_de(de));
	printf("disp_protocol %s\n", get_disp_protocol(disp_protocol));
	//printf("terminal %s\n", get_terminal(terminal));
	printf("cpuname %s\n", get_cpuname(cpuname));

	printf("ramused %ld\n", get_ramused());
	printf("ramtotal %ld\n", get_ramtotal());
	printf("swapused %ld\n", get_swapused());
	printf("swaptotal %ld\n", get_swaptotal());
	printf("diskused %ld\n", get_diskused());
	printf("disktotal %ld\n", get_disktotal());
	
	//printf("battery %s\n", get_battery(battery));
	printf("public ipv4 %s\n", get_ip(ip, 0b00000000));
	printf("private ipv4 %s\n", get_ip(ip, 0b10000000));
	printf("public ipv6 %s\n", get_ip(ip, 0b01000000));
	printf("private ipv6 %s\n", get_ip(ip, 0b11000000));
	return 0;
}
