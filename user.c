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
            printf("\nVFS:>");
            fgets(str, 80, stdin);
            count = sscanf(str,"%s %s %s %s",command[0],command[1],command[2],command[3]);
            if(count == 1)
            {
                if(strcasecmp(command[0],"exit") == 0)
                {
                    break;
                }
                else if(strcasecmp(command[0],"ls") == 0)
                {
                    fptr->lsFile();
                    continue;
                }
                else if(strcasecmp(command[0],"closeall") == 0)
                {
                    fptr->closeAllFile();
                    printf("\nAll files closed successfully");
                    continue;
                }
                else if(strcasecmp(command[0],"clear") == 0)
                {
                    system("clear");
                    continue;
                }
                else if(strcasecmp(command[0],"help") == 0)
                {
                    fptr->displayHelp();
                    continue;
                }
                else
                {
                    printf("\n ERROR: Command Not Found\n");
                    continue;
                }
            }
            else if(count == 2)
            {
                if(strcasecmp(command[0],"stat") == 0)
                {
                    ret = fptr->statFile(command[1]);
                    if(ret == -1)
                        printf("\n ERROR: Incorrect parameters");
                    else if( ret == -2)
                        printf("\n ERROR: There is no such file");
                    else
                        continue;
                }
                else if(strcasecmp(command[0],"fstat") == 0)
                {
                    ret = fptr->fstatFile(atoi(command[1]));
                    if(ret == -1)
                        printf("\n ERROR: Incorrect parameters");
                    else if( ret == -2)
                        printf("\n ERROR: There is no such file");
                    else
                        continue;
                }
                else if(strcasecmp(command[0],"close") == 0)
                {
                    ret = fptr->closeFileByName(command[1]);
                    if(ret == -1)
                        printf("\n ERROR: Incorrect parameters");
                    else if( ret == -2)
                        printf("\n ERROR: There is no such file");
                    else
                        continue;
                }
                else if(strcasecmp(command[0],"rm") == 0)
                {
                    ret = fptr->rmFile(command[1]);
                    if(ret == -1)
                        printf("\n ERROR: There is no such file");
                    continue;
                }
                else if(strcasecmp(command[0],"man") == 0)
                {
                    fptr->man(command[1]);
                    continue;
                }
                else if(strcasecmp(command[0],"truncate") == 0)
                {
                    ret = fptr->truncateFile(command[1]);
                    if(ret == -1)
                        printf("\n ERROR: There is no such file");
                    continue;
                }
                else
                {
                    printf("\n ERROR: Command Not Found\n");
                    continue;
                }
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
