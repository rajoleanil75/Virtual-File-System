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

int VirtualFS::rmFile(char *name)
{
	int fd = getFDFromName(name);
	if(fd == -1)
		return -1;
	UFDTArr[fd].ptrfiletable->ptrinode->LinkCount -=1;
	if(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
	{
		UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
		free(UFDTArr[fd].ptrfiletable);
	}
	UFDTArr[fd].ptrfiletable = NULL;
	SUPERBLOCKobj.FreeInode +=1;
	return 0;
}

int VirtualFS::getFDFromName(char *name)
{
	int i = 0;
	while(i < 50)
	{
		if(UFDTArr[i].ptrfiletable != NULL)
		{
			if(strcmp(UFDTArr[i].ptrfiletable->ptrinode->FileName, name) == 0)
				break;
		}
		i++;
	}
	if(i == 50)
		return -1;
	return i;
}

int VirtualFS::closeFileByName(char *name)
{
	if(name == NULL)
		return -1;
	PINODE temp = head;
	while(temp != NULL)
	{
		if(strcmp(name, temp->FileName) == 0)
			break;
		temp = temp->next;
	}
	if(temp == NULL)
		return -2;
	int i = getFDFromName(name);
	if(i == -1)
		return -2;
	else
	{
		UFDTArr[i].ptrfiletable->readoffset = 0;
		UFDTArr[i].ptrfiletable->writeoffset = 0;
		UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount-=1;
		UFDTArr[i].ptrfiletable->ptrinode = NULL;
	}		
	return 0;
}

int VirtualFS::fstatFile(int fd)
{
	if(fd < 0)
		return -1;
	if(UFDTArr[fd].ptrfiletable == NULL)
		return -2;
	PINODE temp = UFDTArr[fd].ptrfiletable->ptrinode;
	printf("\n ------Statistical Information about file------");
	printf("\n File name: %s",temp->FileName);
	printf("\n Inode Number: %d",temp->InodeNumber);
	printf("\n File Size: %d",temp->FileSize);
	printf("\n Actual File Size: %d",temp->FileActualSize);
	printf("\n Link count: %d",temp->LinkCount);
	printf("\n Reference count: %d",temp->ReferenceCount);
	if(temp->Permission == 1)
		printf("\nFile permission: Read-only");
	else if(temp->Permission == 2)
		printf("\nFile permission: Write");
	else if(temp->Permission == 3)
		printf("\nFile permission: Read & Write");
	return 0;
}

int VirtualFS::statFile(char *name)
{
	if(name == NULL)
		return -1;
	PINODE temp = head;
	while(temp != NULL)
	{
		if(strcmp(name, temp->FileName) == 0)
			break;
		temp = temp->next;
	}
	if(temp == NULL)
		return -2;
	printf("\n ------Statistical Information about file------");
	printf("\n File name: %s",temp->FileName);
	printf("\n Inode Number: %d",temp->InodeNumber);
	printf("\n File Size: %d",temp->FileSize);
	printf("\n Actual File Size: %d",temp->FileActualSize);
	printf("\n Link count: %d",temp->LinkCount);
	printf("\n Reference count: %d",temp->ReferenceCount);
	if(temp->Permission == 1)
		printf("\nFile permission: Read-only");
	else if(temp->Permission == 2)
		printf("\nFile permission: Write");
	else if(temp->Permission == 3)
		printf("\nFile permission: Read & Write");
	return 0;
}

void VirtualFS::closeAllFile()
{
	int i = 0;
	while(i < 50)
	{
		if(UFDTArr[i].ptrfiletable != NULL)
		{
			UFDTArr[i].ptrfiletable->readoffset = 0;
			UFDTArr[i].ptrfiletable->writeoffset = 0;
			UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount-=1;
			UFDTArr[i].ptrfiletable->ptrinode = NULL;
		}
		i++;
	}
}

void VirtualFS::displayHelp()
{
	printf("\n ls: To list out all files");
	printf("\n clear: To clear console");
	printf("\n open: To open the file");
	printf("\n close: To close the file");
	printf("\n closeall: To close all opened file");
	printf("\n read: To read the contents of file");
	printf("\n write: To write contents to file");
	printf("\n exit: To terminate file system");
	printf("\n stat: To display information of file using name");
	printf("\n fstat: To display information of file using file descriptor");
	printf("\n truncate: To remove all data from file");
	printf("\n rm: To delete the file");
	printf("\n man: To display help for single command");
	printf("\n\n For more information about command type\n\t man command_name");
}

void VirtualFS::lsFile()
{
	PINODE temp = head;
	if(SUPERBLOCKobj.FreeInode == MAXINODE)
	{
		printf("\nERROR: There are no files");
		return;
	}
	else{
		printf("\nFile Name \t Inode Number \t File Size \t Link Count");
		printf("\n---------------------------------------------------------------------------");
		while(temp != NULL)
		{
			if(temp->FileType != 0)
			{
				printf("\n%s \t\t %d \t\t %d \t\t %d",temp->FileName, temp->InodeNumber, temp->FileActualSize, temp->LinkCount);
			}
			temp = temp->next;
		}
		printf("\n---------------------------------------------------------------------------");
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