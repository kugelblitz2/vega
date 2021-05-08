#ifdef __linux__
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <pci/pci.h>        // man pcilib(7)
#include <ifaddrs.h>
#include <string.h>
#include <stdio.h>

char *get_username(){
    return getenv("USER");
}

char *get_hostname(){
    struct utsname basicinfo;
    uname(&basicinfo);
    return basicinfo.nodename;
}

char *get_osname();                 // read /etc/os-release

char *get_hwname(char bitmask);     // read /sys/devices/virtual/dmi/id/*

char *get_kernel(){
    struct utsname basicinfo;
    uname(&basicinfo);
    
    char *kernelname;
    asprintf(&kernelname, "%s %s", basicinfo.sysname, basicinfo.version);
    return kernelname;
}

long *get_uptime(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.uptime;
}

char *get_packages();   // hard code by system

char *get_shell();      // read /proc/$$/comm

char *get_screenres();  // read /sys/class/drm

char *get_de();         // read $XDG_CURRENT_DESKTOP then /usr/bin/*session 

char *get_wm();         // $XDG_SESSION_TYPE or $DISPLAY/$WAYLAND_DISPLAY

char *get_terminal();   // read 4th value in /proc/$$/stat for PPID

char *get_cpuname();    // read /proc/cpuinfo

int *get_cpuusage(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return (int) (100 * linuxinfo.loads[0]) / (get_nprocs() * SI_LOAD_SHIFT);
}

char *get_gpuname(char bitmask){
    struct pci_access *pciaccess;
    struct pci_dev *dev;

    char *names;

    pciaccess = pci_alloc();
    pci_init(pciaccess);            // Initialize the PCI library
    pci_scan_bus(pciaccess);        // Gets list of PCI devices

    for(struct pci_dev *device = pciaccess->devices; device; device=device->next){
        pci_fill_info(device, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);    // Reads PCI info
        
        // Checks if PCI device is a display controller
        if((device->device_class & 0300) == 0300){
            // pci_lookup_name needs a char buffer for some reason, I don't know what it does
            *idkwhatthisdoes = malloc(100);
            
            // Reads vendor and/or device name and/or version
            if((bitmask & 0b10000000) = 0b10000000){    // Vendor
                strcat(names, pci_lookup_name(pciaccess, idkwhatthisdoes, 100, PCI_LOOKUP_VENDOR, device->vendor_id));
                strcat(names, ' ');
            }
            if((bitmask & 0b01000000) = 0b01000000){    // Device Name
                strcat(names, pci_lookup_name(pciaccess, idkwhatthisdoes, 100, PCI_LOOKUP_DEVICE, device->vendor_id, device->device_id));
                strcat(names, ' ');
            }
            strcat(names, '\n');
        }
    }
    return &names;
}

long *get_ramused(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.totalram - linuxinfo.freeram;
}

long *get_ramtotal(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.totalram - (linuxinfo.freeram + linuxinfo.bufferram);
}

long *get_swapused(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.totalswap - linuxinfo.freeswap;
}

long *get_swaptotal(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.totalswap;
}

char *get_gpudriver();

unsigned long *get_diskused(){
    struct statvfs diskinfo;
    statvfs(&diskinfo);

    // Disk space used in bytes
    return (diskinfo.f_blocks - diskinfo.f_bavail) / diskinfo.f_blocks;

unsigned long *get_disktotal(){
    struct statvfs diskinfo;
    statvfs(&diskinfo);

    // Disk space used in bytes
    return diskinfo.f_blocks / diskinfo.f_blocks;
}

char *get_battery();    // Read from /sys/class/power_supply/BATx/capacity

char *get_ip(char bitmask){
    unsigned int addrtype = AF_INET;
    bool private_ip = false;
    
    // Read bitmask
    if((bitmask & 0b10000000) == 0b10000000) private_ip = true;
    if((bitmask & 0b01000000) == 0b01000000) addrtype = AF_INET6;

    char *ipaddr;
    struct ifaddrs *ifaddress;
    getifaddrs(&ifaddress);

    for(struct ifaddrs *i = ifaddress; i; i = i->ifa_next){
        if(i->ifa_addr->sa_family != addrtype) continue;        // Ignores the wrong type of address
        getnameinfo(i->ifa_addr, (addrtype == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), ipaddr, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if(strncmp(ipaddr, '127', 3) || strncmp(ipaddr, '169', 3) || strncmp(ipaddr, 'fe80', 4)) continue;      // Ignore loopback and localhost

        // Check for local IPv4
        if(strncmp(ipaddr, '192', 3) || strncmp(ipaddr, '172', 3) || strncmp(ipaddr, '10', 2)){
            if(private_ip) break;       // Stop is private_ip is true
            else continue;              // Ignore otherwise
        }
        // Check for local IPv6
        if(strncmp(ipaddr, 'fd', 2)){
            if(private_ip) break;       // Stop if private_ip is true
            else continue;              // Ignore otherwise
        }
        
        break;
    }

    return ipaddr;
}

#endif