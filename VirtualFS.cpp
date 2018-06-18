using namespace std;
#include<string.h>
#include"sharedfile.h"

VirtualFS::VirtualFS()
{
	head = NULL;
}
VirtualFS::~VirtualFS()
{
	if(head == NULL)
		return;
	PINODE temp;
	while(head != NULL)
	{
		temp = head;
		head = head->next;
		delete temp;
	}
}

void VirtualFS::initInodeTable()
{
	PINODE nn = NULL;
	PINODE temp = head;
	int i = 1;
	while(i<=MAXINODE)
	{
		nn = new INODE;
		nn->LinkCount = nn->ReferenceCount = 0;
		nn->FileType = nn->FileSize = 0;
		nn->Buffer = NULL;
		nn->next = NULL;
		nn->InodeNumber = i;
		if(temp == NULL)
		{
			head = nn;
			temp = head;
		}
		else
		{
			temp->next = nn;
			temp = temp->next;
		}
		i++;
	}
}

void VirtualFS::initSuperBlock()
{
	//Initialise Super block and set total inode to MAXINODE and make UFDTArr NULL
	int i = 0;
	while(i<MAXINODE)
	{
		UFDTArr[i].ptrfiletable = NULL;
		i++;
	}
	SUPERBLOCKobj.TotalInodes = MAXINODE;
	SUPERBLOCKobj.FreeInode = MAXINODE;
}

void VirtualFS::man(char *name)
{
	if(name == NULL)
		return;
	if(strcasecmp(name, "create") == 0)
	{
		printf("\n Description: Used to create new regular file");
		printf("\n Usage: create File_name permission");
	}
	else if(strcasecmp(name, "read") == 0)
	{
		printf("\n Description: Used to read data from regular file");
		printf("\n Usage: read File_name No_of_Bytes_To_Read");
	}
	else if(strcasecmp(name, "write") == 0)
	{
		printf("\n Description: Used to write into regular file");
		printf("\n Usage: write File_name\n After this enter the data that you want to write");
	}
	else if(strcasecmp(name, "ls") == 0)
	{
		printf("\n Description: Used to list all information of files in Directory");
		printf("\n Usage: ls");
	}
	else if(strcasecmp(name, "stat") == 0)
	{
		printf("\n Description: Used to display information of file");
		printf("\n Usage: stat File_name");
	}
	else if(strcasecmp(name, "fstat") == 0)
	{
		printf("\n Description: Used to display information of file");
		printf("\n Usage: fstat File_Descriptor");
	}
	else if(strcasecmp(name, "truncate") == 0)
	{
		printf("\n Description: Used to remove data from file");
		printf("\n Usage: truncate File_name");
	}
	else if(strcasecmp(name, "open") == 0)
	{
		printf("\n Description: Used to open existing file");
		printf("\n Usage: open File_name mode");
	}
	else if(strcasecmp(name, "close") == 0)
	{
		printf("\n Description: Used to close opened file");
		printf("\n Usage: close File_name");
	}
	else if(strcasecmp(name, "closeall") == 0)
	{
		printf("\n Description: Used to close all opened file");
		printf("\n Usage: closeall");
	}
	else if(strcasecmp(name, "lseek") == 0)
	{
		printf("\n Description: Used to change file offset");
		printf("\n Usage: lseek File_name ChangeInOffset StartPoint");
	}
	else if(strcasecmp(name, "rm") == 0)
	{
		printf("\n Description: Used to delete the file");
		printf("\n Usage: rm File_name");
	}
	else
		printf("\n ERROR: No manual entry available");
}

extern "C"
{
	VirtualFS* create()
	{
		return new VirtualFS;
	}
	void destroy(VirtualFS* p)
	{
		delete p;
	}
}