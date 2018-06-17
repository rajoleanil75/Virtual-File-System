#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>
#include"sharedfile.h"

int main(int argc, char const *argv[])
{
    void *p = NULL;
    VirtualFS *ptr = NULL;
    VirtualFS* (*fp1)() = NULL;
    void (*fp2)(VirtualFS *) = NULL;
    p = dlopen("/mnt/f/C Project/Virtual-File-System/VirtualFS.so",RTLD_LAZY);
    if(!p)
    {
        fprintf(stderr, "%s", dlerror());
        exit(1);
    }    
    else
    {
        fp1 = (VirtualFS*(*)()) dlsym(p,"create");
        fp2 = (void(*)(VirtualFS *)) dlsym(p,"destroy");
        ptr = fp1();

        char command[10] = "create";
        ptr->man(command);

        fp2(ptr);
        dlclose(p);
    }
    return 0;
}
