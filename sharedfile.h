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
	virtual void lsFile();
	virtual void closeAllFile();
	virtual void displayHelp();
	virtual int statFile(char *);
	virtual int fstatFile(int );
	virtual int closeFileByName(char *);
	virtual int getFDFromName(char *);
	virtual int rmFile(char *);
	virtual int truncateFile(char *);
	virtual int writeFile(int , char *, int );
	virtual int createFile(char *, int );
	virtual PINODE getInode(char *);
	// virtual int readFile(int , char *, int );
	// virtual int openFile(char *,int );
	// virtual int lseekFile(int , int , int );
};