#include "fetch/linux.c"

int main(){
	printf("Vega Unformatted Output: \n");
	printf("username %s\n", get_username());
	printf("hostname %s\n", get_hostname());
	printf("osname %s\n", get_osname());
	printf("hwname %s\n", get_hwname(0b11111111));
	printf("kernel %s\n", get_kernel());

	printf("uptime %f", get_uptime());

	return 0;
}