#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>
#include"sharedfile.h"

int main(int argc, char const *argv[])
{
    char *ptr = NULL;
    int ret = 0, fd = 0, count = 0;
    char command[4][80], str[80], arr[1024];

    void *p = NULL;
    VirtualFS *fptr = NULL;
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
        fptr = fp1();

        fptr->initialiseSuperBlock();


        fp2(fptr);
        dlclose(p);
    }
    return 0;
}
