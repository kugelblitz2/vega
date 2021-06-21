#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <pci/pci.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linux.h"

char *get_username(char *returnptr){
	strcpy(returnptr, getenv("USER"));
    return returnptr;
}

char *get_hostname(char *returnptr){
    struct utsname basicinfo;
    uname(&basicinfo);
    strcpy(returnptr, basicinfo.nodename);
    return returnptr;
}

char *get_osname(char *returnptr){
    FILE *os_release;
    os_release = fopen("/etc/os-release", "r");
	
    while(*returnptr != EOF){
        fgets(returnptr, 256, os_release);
        if(strncmp(returnptr, "PRETTY_NAME", 11) == 0){
            returnptr +=13;
            returnptr[strlen(returnptr)-2] = '\0';
            break;
        }
    }
	fclose(os_release);
    return returnptr;
}

char *get_hwname(char *returnptr, char bitmask){
    FILE *smbios;

    if((bitmask & 0b10000000) == 0b10000000){       // board_vendor
        char *read = malloc(256);
	
        smbios = fopen("/sys/devices/virtual/dmi/id/board_vendor", "r");
        fgets(read, 256, smbios);

        strcat(returnptr, read);
        strcat(returnptr, " ");

        returnptr[strlen(returnptr)-2] = ' ';
        returnptr[strlen(returnptr)-1] = '\0';
    }
    if((bitmask & 0b01000000) == 0b01000000){       // product_family
        char *read = malloc(256);

        smbios = fopen("/sys/devices/virtual/dmi/id/product_family", "r");
        fgets(read, 256, smbios);

        strcat(returnptr, read);
        strcat(returnptr, " ");
        
        returnptr[strlen(returnptr)-2] = ' ';
        returnptr[strlen(returnptr)-1] = '\0';
    }
    if((bitmask & 0b00100000) == 0b00100000){       // product_name
        char *read = malloc(256);

        smbios = fopen("/sys/devices/virtual/dmi/id/product_name", "r");
        fgets(read, 256, smbios);

        strcat(returnptr, read);
        strcat(returnptr, " ");
        
        returnptr[strlen(returnptr)-2] = ' ';
        returnptr[strlen(returnptr)-1] = '\0';
    }
    if((bitmask & 0b00010000) == 0b00010000){       // product_version
        char *read = malloc(256);

        smbios = fopen("/sys/devices/virtual/dmi/id/product_version", "r");
        fgets(read, 256, smbios);

        strcat(returnptr, read);
        strcat(returnptr, " ");
        
        returnptr[strlen(returnptr)-2] = ' ';
        returnptr[strlen(returnptr)-1] = '\0';
    }
    fclose(smbios);

    returnptr[strlen(returnptr)-1] = '\0';
    return returnptr;
}

char *get_kernel(char *returnptr){
    struct utsname basicinfo;
    uname(&basicinfo);

    strcpy(returnptr, basicinfo.sysname);
    returnptr[strlen(returnptr)] = ' ';
    strcpy(returnptr+strlen(returnptr), basicinfo.release);

    return returnptr;
}

long get_uptime(){
    struct sysinfo *linuxinfo;
    sysinfo(linuxinfo);

    return linuxinfo->uptime;
}

char *get_shell(char *returnptr){
    char *path = malloc(32);
    FILE *comm;

    // Get path for /proc/$$/comm (shell ppid)
    sprintf(path, "/proc/%d/comm", getppid());

    // Read /proc/$$/comm (shell ppid)
    comm = fopen(path, "r");
    fgets(returnptr, 256, comm);    
    fclose(comm);

    returnptr[strlen(returnptr)-1] = '\0';
    return returnptr;
}

char *get_screenres(char *returnptr){		// read /sys/class/drm
    char *screen_resolutions = malloc(256), *vcardpath = malloc(64), *res = malloc(64);
    DIR *drm;
    FILE *modesfile;
	
    drm = opendir("/sys/class/drm/");
    while(1){
        // Gets modes path
		struct dirent *directory = readdir(drm);
		if(directory == NULL) break;
		
		// Opens status file
		sprintf(vcardpath, "/sys/class/drm/%s/status", directory->d_name);
		modesfile = fopen(vcardpath, "r");
        if(modesfile == NULL) continue;
        
		// Checks if screen is connected
		fgets(res, 64, modesfile);
		fclose(modesfile);
		if(strncmp(res, "connected", 9)) continue;

		// Opens modes file
		sprintf(vcardpath, "/sys/class/drm/%s/modes", directory->d_name);
        modesfile = fopen(vcardpath, "r");
        if(modesfile == NULL) continue;
        
		// Reads screen resolutions
        fgets(res, 64, modesfile);
		if(*res == '\0') continue;
        
		strcat(returnptr, res);
		fclose(modesfile);
    }
	
    returnptr[strlen(returnptr)-1] = '\0';
    return returnptr;
}

char *get_de(char *returnptr){
	if(getenv("XDG_CURRENT_DESKTOP") == NULL) return NULL;

	strcpy(returnptr, getenv("XDG_CURRENT_DESKTOP"));
	return returnptr;
}

char *get_disp_protocol(char *returnptr){
	if(getenv("XDG_SESSION_TYPE") == NULL) return NULL;

	strcpy(returnptr, getenv("XDG_SESSION_TYPE"));
	return returnptr;
}

char *get_terminal(char *returnptr){
    char *term_pid = malloc(256), *path = malloc(32);
    FILE *comm;

    // Get path for /proc/$$/stat (shell ppid)
    sprintf(path, "/proc/%d/stat", getppid());

    // Read /proc/$$/comm (shell ppid/terminal pid)
    comm = fopen(path, "r");
    fgets(term_pid, 256, comm);
    fclose(comm);
    int counter = 0, start = 0, end = 0;
    for(int i = 0; i < 256; i++){
        if(term_pid[i] == ' ') counter++;
        if(counter == 3 && start == 0) start = i+1;
        if(counter == 4){
            end = i;
            break;
        }
    }
    term_pid[end] = '\0';
    term_pid += start;
    
    // Read terminal name
    sprintf(path, "/proc/%s/comm", term_pid);
    
    comm = fopen(path, "r");
    fgets(returnptr, 256, comm);
    fclose(comm);

    returnptr[strlen(returnptr)-1] = '\0';
    return returnptr;
}

char *get_cpuname(char *returnptr){
    FILE *cpuinfo;
    
    cpuinfo = fopen("/proc/cpuinfo", "r");
    while(*returnptr != EOF){
        fgets(returnptr, 256, cpuinfo);
        if(strncmp(returnptr, "model name", 10) == 0){
            returnptr += 13;
            returnptr[strlen(returnptr)-1] = '\0';
            break;
        }
    }
    
    fclose(cpuinfo);
    return returnptr;
}

char *get_gpuname(char *returnptr, char bitmask){
    struct pci_access *pciaccess;
    struct pci_dev *dev;

    pciaccess = pci_alloc();
    pci_init(pciaccess);            // Initialize the PCI library
    pci_scan_bus(pciaccess);        // Gets list of PCI devices

    for(struct pci_dev *device = pciaccess->devices; device; device=device->next){
        pci_fill_info(device, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);    // Reads PCI info
        
        // Checks if PCI device is a display controller
        if((device->device_class & 0b1111111100000000) == 0x0300){
            // pci_lookup_name needs a char buffer for some reason, I don't know what it does
            char *idkwhatthisdoes = malloc(100);
            
            // Reads vendor and/or device name and/or version
			if((bitmask & 0b10000000) == 0b10000000){    // Vendor
                strcat(returnptr, pci_lookup_name(pciaccess, idkwhatthisdoes, 100, PCI_LOOKUP_VENDOR, device->vendor_id));
                strcat(returnptr, " ");
            }
            if((bitmask & 0b01000000) == 0b01000000){    // Device Name
                strcat(returnptr, pci_lookup_name(pciaccess, idkwhatthisdoes, 100, PCI_LOOKUP_DEVICE, device->vendor_id, device->device_id));
                strcat(returnptr, " ");
            }
            returnptr[strlen(returnptr)-1] = '\0';
            strcat(returnptr, "\n");
        }
    }
    returnptr[strlen(returnptr)-1] = '\0';

    return returnptr;
}

long get_ramused(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.totalram - (linuxinfo.freeram + linuxinfo.bufferram);
}

long get_ramtotal(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.totalram;
}

long get_swapused(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.totalswap - linuxinfo.freeswap;
}

long get_swaptotal(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.totalswap;
}

unsigned long get_diskused(){
    struct statvfs diskinfo;
    statvfs("/", &diskinfo);

    // Disk space used in bytes
    return (diskinfo.f_blocks - diskinfo.f_bfree) * diskinfo.f_frsize;
}

unsigned long get_disktotal(){
    struct statvfs diskinfo;
    statvfs("/", &diskinfo);
	
    // Disk space used in bytes
    return diskinfo.f_blocks * diskinfo.f_frsize;
}

struct batteries get_battery(){
	FILE *battery_data;
	struct batteries batteries;
	batteries.num_of_batts = 0;

	for(int i = 0; true; i++){
		char *path = malloc(64), *temp = malloc(16);
		
		// Read battery status
		sprintf(path, "/sys/class/power_supply/BAT%d/status", i);
		battery_data = fopen(path, "r");
		if(battery_data == NULL) break;
		
		batteries.num_of_batts++;
		batteries.battery[i].charge_status = malloc(16);
		fgets(batteries.battery[i].charge_status, 16, battery_data);
		batteries.battery[i].charge_status[strlen(batteries.battery[i].charge_status)-1] = '\0';
		fclose(battery_data);

		// Read battery design capacity
		sprintf(path, "/sys/class/power_supply/BAT%d/energy_full_design", i);
        battery_data = fopen(path, "r");
		fgets(temp, 16, battery_data);
		batteries.battery[i].design_capacity = atoi(temp);
		fclose(battery_data);

		// Read battery capacity
		sprintf(path, "/sys/class/power_supply/BAT%d/energy_full", i);
		battery_data = fopen(path, "r");
		fgets(temp, 16, battery_data);
		batteries.battery[i].charge_full = atoi(temp);
        fclose(battery_data);

		// Read battery remaining
		sprintf(path, "/sys/class/power_supply/BAT%d/energy_now", i);
		battery_data = fopen(path, "r");
		fgets(temp, 16, battery_data);
		batteries.battery[i].charge_now = atoi(temp);
        fclose(battery_data);

		// Read battery percent
		sprintf(path, "/sys/class/power_supply/BAT%d/capacity", i);
		battery_data = fopen(path, "r");
		fgets(temp, 16, battery_data);
		batteries.battery[i].charge_percent = atoi(temp);
		fclose(battery_data);
	}

	return batteries;
}

char *get_ip(char *returnptr, char bitmask){		// Only returns one IP address at a time, currently incapable of public ipv4
    unsigned int addrtype = AF_INET;
    bool private_ip = false;
    
    // Read bitmask
    if((bitmask & 0b10000000) == 0b10000000) private_ip = true;
    if((bitmask & 0b01000000) == 0b01000000) addrtype = AF_INET6;

    struct ifaddrs *ifaddress;
    getifaddrs(&ifaddress);

    for(struct ifaddrs *i = ifaddress; i; i = i->ifa_next){
        if(i->ifa_addr->sa_family != addrtype) continue;        // Ignores the wrong type of address
        getnameinfo(i->ifa_addr, (addrtype == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), returnptr, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        
	if(!strncmp(returnptr, "127", 3) || !strncmp(returnptr, "169", 3) || !strncmp(returnptr, "fe80", 4) || !strncmp(returnptr, ":", 1)) continue;		// Ignore loopback and localhost

        // Check for local IPv4
        if(!strncmp(returnptr, "192", 3) || !strncmp(returnptr, "172", 3) || !strncmp(returnptr, "10", 2)){
            if(private_ip) break;       // Stop is private_ip is true
	    else continue;              // Ignore otherwise
        }
        // Check for local IPv6
        if(!strncmp(returnptr, "fd", 2)){
            if(private_ip) break;       // Stop if private_ip is true
	    else continue;              // Ignore otherwise
        }
        break;
    }

    return returnptr;
}

