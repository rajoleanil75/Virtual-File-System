#include<stdio.h>
#include<stdlib.h>
#include<dlfcn.h>
#include<string.h>
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

        fptr->initSuperBlock();
        fptr->initInodeTable();
        while(1)
        {
            fflush(stdin);
            strcpy(str, "");
            printf("VFS:>");
            fgets(str, 80, stdin);
            count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);
            if(count == 1)
            {
                printf("\n 1");
                if(strcasecmp(command[0],"exit") == 0)
                {
                    break;
                }
            }
            else if(count == 2)
            {
                printf("\n 2");
            }
            else if(count == 3)
            {
                printf("\n 3");
            }
            else if(count == 4)
            {
                printf("\n 4\t %d\n",count);
            }
            else
            {
                printf("\nERROR: Command not found\n");
                continue;
            }
        }


        fp2(fptr);
        dlclose(p);
    }
    return 0;
}
