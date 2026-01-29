//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File Name:             cvfs.h
//  Description:           Header file containing macro definitions, structures, and function prototypes
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CVFS_H
#define CVFS_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          USER DEFINED MACROS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAXFILESIZE     1024
#define MAXOPENFILES    20
#define MAXINODE        5

#define READ            1
#define WRITE           2
#define EXECUTE         4

#define START           0
#define CURRENT         1
#define END             2

#define EXECUTE_SUCCESS 0

#define REGULARFILE     1
#define SPECIALFILE     2

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  MACROS FOR ERROR HANDLING
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER     -1
#define ERR_NO_INODES             -2
#define ERR_FILE_ALREADY_EXISTS   -3
#define ERR_FILE_NOT_EXISTS       -4
#define ERR_PERMISSION_DENIED     -5
#define ERR_INSUFFICIENT_SPACE    -6
#define ERR_INSUFFICIENT_DATA     -7
#define ERR_MAX_FILES_OPEN        -8

#define BACKUP_FILE "CVFS_Backup.bin"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      STRUCTURE DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Bootblock
{
    char Information[100];
};

struct Superblock
{
    int TotalInodes;
    int FreeInodes;
};

// Use #pragma pack(1) to avoid padding
#pragma pack(1)
struct Inode
{
    char   FileName[20];
    int    InodeNumber;
    int    FileSize;
    int    ActualFileSize;
    int    FileType;
    int    ReferenceCount;
    int    Permission;
    char   *Buffer;
    struct Inode *next;
};

typedef struct Inode   INODE;
typedef struct Inode* PINODE;
typedef struct Inode** PPINODE;

struct Filetable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef struct Filetable  FILETABLE;
typedef struct Filetable* PFILETABLE;

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  GLOBAL VARIABLE DECLARATIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Declared extern so they can be accessed in implementation files but defined only once
extern struct Bootblock  bootobj;
extern struct Superblock superobj;
extern struct UAREA      uareaobj;
extern PINODE head;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initialiseUAREA();
void initialiseSuperBlock();
void createDILB();
void startAuxillaryDataInitialization();
void displayHelp();
void manPageDisplay(char Name[]);
bool isFileExists(const char* name);
int createFile(char *name, int permission);
void lsFile();
int unlinkFile(char *name);
int writeFile(int fd, char *data, int size);
int readFile(int fd, char *data, int size);
int statFile(char *name);
int fstatFile(int fd);
int openFile(char *name, int mode);
int closeFile(int fd);
int truncateFile(char *name);
int renameFile(char *oldName, char *newName);
int catFile(char *name);
int copyFile(char *src, char *dest);
int backupCVFS();
void restoreCVFS();

#endif // CVFS_H