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

char *get_kernel(){
    struct utsname buffer;
    uname(&buffer);
    
    char *kernelname;
    asprintf(&kernelname, "%s %s", buffer.sysname, buffer.version);
    return kernelname;
}