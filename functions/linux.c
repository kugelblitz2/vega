#ifdef __linux__
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <pci/pci.h>        // man pcilib(7)
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

    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return linuxinfo.uptime;
    // returns LONG_MIN if system is unsupported
    return LONG_MIN;
}

char *get_packages();   // hard code by system

char *get_shell();      // read ps -p $$

char *get_screenres();  // read /sys/class/drm

char *get_de();         // read $XDG_CURRENT_DESKTOP then /usr/bin/*session 

char *get_wm();         // $XDG_SESSION_TYPE or $DISPLAY/$WAYLAND_DISPLAY

char *get_terminal();   // read ps -p $$ -o ppid

char *get_cpuname();    // read /proc/cpuinfo

int *get_cpuusage(){
    struct sysinfo linuxinfo;
    sysinfo(&linuxinfo);

    return (int) (100 * linuxinfo.loads[0]) / (get_nprocs() * SI_LOAD_SHIFT);

}

// Returns a pointer to an array of pointers each pointing to a GPU name
// Supports up to 4 GPUs
char **get_gpuname(){
    struct pci_access *pciaccess;
    struct pci_dev *dev;
    
    char *names[4];
    char index = 0;

    pciaccess = pci_alloc();
    pci_init(pciaccess);            // Initialize the PCI library
    pci_scan_bus(pciaccess);        // Gets list of PCI devices

    for(struct pci_dev *device = pciaccess->devices; device; device=device->next){
        pci_fill_info(device, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);    // Reads PCI info
        
        // Checks if PCI device is a display controller
        if((device->device_class & 0300) == 0300){
            // pci_lookup_name needs a char buffer for some reason, I don't know what it does
            *idkwhatthisdoes = malloc(100);
            
            // Reads vendor and device name
            names[index] = pci_lookup_name(pciaccess, idkwhatthisdoes, 100, PCI_LOOKUP_VENDOR | PCI_LOOKUP_DEVICE, device->vendor_id, device->device_id);
            index++;
        }
    }
    return &names;
}

char *get_vramused();

char *get_vramtotal();

long *get_ramused(){
    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return linuxinfo.totalram - linuxinfo.freeram;
    #endif
}

long *get_ramtotal(){
    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return linuxinfo.totalram - (linuxinfo.freeram + linuxinfo.bufferram);
    #endif
}

long *get_swapused(){
    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return linuxinfo.totalswap - linuxinfo.freeswap;
    #endif
}

long *get_swaptotal(){
    #ifdef __linux__
        struct sysinfo linuxinfo;
        sysinfo(&linuxinfo);

        return linuxinfo.totalswap;
    #endif
}

char *get_gpudriver();

unsigned long *get_diskused(){
    #ifdef __linux__
        struct statvfs diskinfo;
        statvfs(&diskinfo);

        // Disk space used in bytes
        return (diskinfo.f_blocks - diskinfo.f_bavail) / diskinfo.f_blocks;
    #endif
}

unsigned long *get_disktotal(){
    #ifdef __linux__
        struct statvfs diskinfo;
        statvfs(&diskinfo);

        // Disk space used in bytes
        return diskinfo.f_blocks / diskinfo.f_blocks;
    #endif 
}

char *get_battery();    // Read from /sys/class/power_supply/BATx/capacity

char *get_localip();

char *get_ipv4();

char *get_ipv6();

#endif