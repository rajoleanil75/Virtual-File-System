#define MAXINODE 50
#define READ 1
#define WRITE 2
#define MAXFILESIZE 1024
#define REGULAR 1
#define SPECIAL 2
#define START 0
#define CURRENT 1
#define END 2
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

typedef struct superblock
{
	int TotalInodes;
	int FreeInode;
}SUPERBLOCK, *PSUPERBLOCK;

typedef struct inode
{
	char FileName[50];
	int InodeNumber;
	int FileSize;
	int FileActualSize;
	int FileType;
	char *Buffer;
	int LinkCount;
	int ReferenceCount;
	int Permission;
	struct inode *next;
}INODE, *PINODE, **PPINODE;

typedef struct filetable
{
	int readoffset;
	int writeoffset;
	int count;
	int mode;
	PINODE ptrinode;
}FILETABLE, *PFILETABLE;

typedef struct ufdt
{
	PFILETABLE ptrfiletable;
}UFDT;

class VirtualFS
{
private:
	UFDT UFDTArr[50];
	SUPERBLOCK SUPERBLOCKobj;
	PINODE head;

public:
	VirtualFS();
	~VirtualFS();
	virtual void man(char *);
	virtual void initSuperBlock();
	virtual void initInodeTable();
	// virtual void displayHelp();
	// virtual int getFDFromName(char *);
	// virtual PINODE get_Inode(char *);
	// virtual int createFile(char *, int );
	// virtual int rmFile(char *);
	// virtual int readFile(int , char *, int );
	// virtual int writeFile(int , char *, int );
	// virtual int openFile(char *,int );
	// virtual void closeFileByFD(int );
	// virtual void closeAllFile();
	// virtual int lseekFile(int , int , int );
	// virtual void isFile();
	// virtual int fstatFile(int );
	// virtual int statFile(char *);
	// virtual int truncateFile(char *);
};