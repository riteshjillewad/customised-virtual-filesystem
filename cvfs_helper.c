//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File Name:             cvfs_helper.c
//  Description:           Implementation of CVFS helper functions and logic
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "cvfs.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//     Global variables or objects used in the project
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Bootblock  bootobj;
struct Superblock superobj;
struct UAREA      uareaobj;

PINODE head = NULL;                                             /* Head pointer for the Inode Linked List */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         initialiseUAREA()
//  Description:           Initializes the User Area (UAREA) structure
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initialiseUAREA()
{
    int i = 0;

    strcpy(uareaobj.ProcessName, "Myexe");                      /* Set the process name */
    
    /* Initialize all UFDT pointers to NULL */
    for(i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }
    printf("CVFS: UAREA initialized successfully.\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         initialiseSuperBlock()
//  Description:           Initializes the Superblock structure
//  Author:                Ritesh Jillewad
//  Date:                  13/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes  = MAXINODE;                            /* Initially, all inodes are free */

    printf("CVFS: Superblock initialized successfully.\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         createDILB()
//  Description:           Creates the Disk Inode List Block (Linked List of Inodes)
//  Author:                Ritesh Jillewad
//  Date:                  13/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void createDILB()                                               
{
    PINODE newnode = NULL;
    PINODE temp = NULL;
    int i = 0;                                                  /* Counter for inode numbers */

    temp = head;                                                /* Preserve the current head pointer */
    
    /* Iterate to create inodes */
    for(i = 1; i <= MAXINODE; i++)
    {
        /* Allocate memory for a new inode */
        newnode = (PINODE)malloc(sizeof(INODE));

        /* Initialize inode members */
        strcpy(newnode -> FileName, "\0");
        newnode -> InodeNumber = i;
        newnode -> FileSize = 0;
        newnode -> ActualFileSize = 0;
        newnode -> FileType = 0;
        newnode -> ReferenceCount = 0;
        newnode -> Permission = 0;
        newnode -> Buffer = NULL;
        newnode -> next = NULL;

        /* Link the new inode to the list */
        if(temp == NULL)                                        /* Case 1: Linked List is empty */
        {
            head = newnode;
            temp = head;
        }
        else                                                    /* Case 2: Linked List contains at least one node */
        {
            temp -> next = newnode;
            temp = temp -> next;        
        }
    }

    printf("CVFS: DILB created successfully.\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         startAuxillaryDataInitialization()
//  Description:           Initializes all auxiliary data structures
//  Author:                Ritesh Jillewad
//  Date:                  13/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void startAuxillaryDataInitialization()
{
    strcpy(bootobj.Information, "CVFS booting process completed.\n");
    printf("%s\n", bootobj.Information);

    initialiseSuperBlock();
    createDILB();
    initialiseUAREA();

    printf("CVFS: Auxiliary data initialized successfully.\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         displayHelp()
//  Description:           Displays the help menu with available commands
//  Author:                Ritesh Jillewad
//  Date:                  14/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void displayHelp()
{
    printf("\n-----------------------------------------------------------------------------\n");
    printf("--------------------------- CVFS Commands Reference -------------------------\n");
    printf("-----------------------------------------------------------------------------\n");

    printf("\n[ GENERAL COMMANDS ]\n");
    printf("man     : Display the manual page for a specific command.\n");
    printf("help    : Display this help manual.\n");
    printf("clear   : Clear the terminal screen.\n");
    printf("exit    : Terminate the CVFS application.\n");
    printf("backup  : Backup filesystem to disk.\n");
    printf("restore : Restore filesystem from disk.\n");

    printf("\n[ FILE OPERATIONS ]\n");
    printf("ls      : List all files currently in the system.\n");
    printf("creat   : Create a new file.\n");
    printf("open    : Open an existing file for reading or writing.\n");
    printf("close   : Close an opened file.\n");
    printf("read    : Read data from an open file.\n");
    printf("write   : Write data into an open file.\n");
    printf("rm      : Delete a file from the system.\n");
    printf("cp      : Copy contents from source to destination.\n");
    printf("mv      : Rename a file (usage: rename old new).\n");
    printf("cat     : Display file contents.\n");
    printf("truncate: Remove all data from a file.\n");
    printf("lseek   : Change the read/write offset position.\n");

    printf("\n[ INFORMATION ]\n");
    printf("stat    : Display statistical information of a file by name.\n");
    printf("fstat   : Display statistical information of a file by descriptor.\n");
    printf("----------------------------------------------------------------------------\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         manPageDisplay()
//  Description:           Displays the manual page for a specific command
//  Author:                Ritesh Jillewad
//  Date:                  14/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void manPageDisplay(char Name[])                             /* Name stores the command (e.g., ls, stat) */
{
    if(Name == NULL)
    {
        return;
    }

    printf("\n----------------------------------------------------------------------------\n");
    printf("------------------------- System Calls Manual ------------------------------\n");
    printf("----------------------------------------------------------------------------\n");


    /* Manual page for ls command */
    if(strcmp("ls", Name) == 0)
    {
        printf("NAME        : ls\n");
        printf("DESCRIPTION : List information about all the files in current directory.\n");                            
        printf("USAGE       : ls\n");                                                                     
    }


    /* Manual page for creat command */
    else if(strcmp("creat", Name) == 0)
    {
        printf("NAME        : creat\n");
        printf("DESCRIPTION : Create a new regular file in the filesystem.\n");
        printf("USAGE       : creat <file_name> <permission>\n");
        printf("ARGUMENTS   : file_name   (Name of the file to be created)\n");
        printf("              permission  (1:Read, 2:Write, 3:Read & Write)\n");
    }


    /* Manual page for open command */
    else if(strcmp("open", Name) == 0)
    {
        printf("NAME        : open\n");
        printf("DESCRIPTION : Open an existing file for reading/writing.\n");
        printf("USAGE       : open <file_name> <mode>\n");
        printf("ARGUMENTS   : file_name (Name of the file to open)\n");
        printf("              mode      (1:Read, 2:Write, 3:Read & Write)\n");
    }


    /* Manual page for read command */
    else if(strcmp("read", Name) == 0)
    {
        printf("NAME        : read\n");
        printf("DESCRIPTION : Read data from an opened file into a buffer.\n");
        printf("USAGE       : read <file_name> <number_of_bytes>\n");
        printf("ARGUMENTS   : file_name (Name of the file to read from)\n");
        printf("              bytes     (Quantity of data to read)\n");
    }


    /* Manual page for write command */
    else if(strcmp("write", Name) == 0)
    {
        printf("NAME        : write\n");
        printf("DESCRIPTION : Write data into an opened file.\n");
        printf("USAGE       : write <file_name>\n");
        printf("NOTE        : Follow the command by entering the data text.\n");
    }

    
    /* Manual page for stat command */
    else if(strcmp("stat", Name) == 0)
    {
        printf("NAME        : stat\n");
        printf("DESCRIPTION : Display the status and information of a file.\n");
        printf("USAGE       : stat <file_name>\n");
        printf("ARGUMENTS   : file_name (Name of the file to inspect)\n");
    }

    /* Manual page for fstat command */
    else if(strcmp("fstat", Name) == 0)
    {
        printf("NAME        : fstat\n");
        printf("DESCRIPTION : Display the status and information of a file by file descriptor.\n");
        printf("USAGE       : fstat <file_descriptor>\n");
        printf("ARGUMENTS   : file_descriptor (The integer returned by open/creat)\n");
    }

    /* Manual page for truncate command */
    else if(strcmp("truncate", Name) == 0)
    {
        printf("NAME        : truncate\n");
        printf("DESCRIPTION : Remove all data from a file (sets size to 0).\n");
        printf("USAGE       : truncate <file_name>\n");
        printf("ARGUMENTS   : file_name (Name of the file to truncate)\n");
    }

    /* Manual page for man command */
    else if(strcmp("man", Name) == 0)
    {
        printf("NAME        : man\n");
        printf("DESCRIPTION : Display the manual page for a given command.\n");
        printf("USAGE       : man <command_name>\n");
        printf("ARGUMENTS   : command_name (The utility you want to learn about).\n");
    }  


    /* Manual page for help command */
    else if(strcmp("help", Name) == 0)
    {
        printf("NAME        : help\n");
        printf("DESCRIPTION : Display a summary of all available commands in CVFS.\n");
        printf("USAGE       : help\n");
    }


    /* Manual page for clear command */
    else if(strcmp("clear", Name) == 0)
    {
        printf("NAME        : clear\n");
        printf("DESCRIPTION : Clear the current terminal screen.\n");
        printf("USAGE       : clear\n");
    }


    /* Manual page for exit command */
    else if(strcmp("exit", Name) == 0)
    {
        printf("NAME        : exit\n");
        printf("DESCRIPTION : Terminate the CVFS shell and deallocate resources.\n");
        printf("USAGE       : exit\n");
    }

    /* Manual page for rm command */
    else if(strcmp("rm", Name) == 0)
    {
        printf("NAME        : rm\n");
        printf("DESCRIPTION : Delete a file.\n");
        printf("USAGE       : rm <file_name>\n");
        printf("ARGUMENTS   : file_name (Name of the file to delete)\n");
    }

    /* Manual page for cp command */
    else if(strcmp("cp", Name) == 0)
    {
        printf("NAME        : cp\n");
        printf("DESCRIPTION : Copy content from source file to destination file.\n");
        printf("USAGE       : cp <source> <destination>\n");
    }

    /* Manual page for cat command */
    else if(strcmp("cat", Name) == 0)
    {
        printf("NAME        : cat\n");
        printf("DESCRIPTION : Display contents of a file.\n");
        printf("USAGE       : cat <filename>\n");
    }

    /* Manual page for rename command */
    else if(strcmp("rename", Name) == 0)
    {
        printf("NAME        : rename\n");
        printf("DESCRIPTION : Rename an existing file.\n");
        printf("USAGE       : rename <old_name> <new_name>\n");
    }

    /* Manual page for backup command */
    else if(strcmp("backup", Name) == 0)
    {
        printf("NAME        : backup\n");
        printf("DESCRIPTION : Backup all files to a local binary file.\n");
        printf("USAGE       : backup\n");
    }

    /* Manual page for restore command */
    else if(strcmp("restore", Name) == 0)
    {
        printf("NAME        : restore\n");
        printf("DESCRIPTION : Restore files from local backup.\n");
        printf("USAGE       : restore\n");
    }

    /* Invalid manual page command */
    else
    {
        printf("ERROR       : No manual entry found for command '%s'.\n", Name);
    }

    printf("----------------------------------------------------------------------------\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         isFileExists()
//  Description:           Checks if a file already exists
//  Input:                 Filename
//  Output:                True or false
//  Author:                Ritesh Jillewad
//  Date:                  16/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool isFileExists(const char* name)                     /* Filename to search for in the inode list */
{
    PINODE temp = NULL;
    bool bFlag = false;

    temp = head;                            
    while(temp != NULL)                                 /* Iterate through the inode list */
    {
        /* Check if filename matches and it is a regular file */
        if((strcmp(name, temp -> FileName) == 0) && (temp -> FileType) == REGULARFILE)
        {
            /* File found */
            bFlag = true;
            break;
        }

        temp = temp -> next;
    }

    return bFlag;
}// End of isFileExists()

/* Note: The && condition ensures that deleted files (FileType 0) are ignored. */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         createFile()
//  Description:           Creates a new regular file
//  Input:                 Filename and permissions
//  Output:                File descriptor (Integer)
//  Author:                Ritesh Jillewad
//  Date:                  16/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int createFile(char *name, int permission)
{
    PINODE temp = NULL;
    int i = 0;

    temp = head;
    // printf("Remaining inodes: %d\n", superobj.FreeInodes);

    /* Input Validation */

    // Check if filename is NULL
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Validate permissions
    // Permission = 1 -> READ
    // Permission = 2 -> WRITE
    // Permission = 3 -> READ + WRITE
    if((permission < 1) || (permission > 3))
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check if there are free inodes available
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // Check if the file already exists
    if(isFileExists(name) == true)
    {
        return ERR_FILE_ALREADY_EXISTS;
    }

    /* Validation Passed */

    // 1. Search for a free Inode
    while(temp != NULL)
    {
        if((temp -> FileType) == 0)
        {
            // Found a free inode
            break;
        }
        temp = temp -> next;
    }

    if(temp == NULL)
    {
        return ERR_NO_INODES;
    }

    // 2. Search for a free UFDT (User File Descriptor Table) slot
    // Start from 3, as 0, 1, 2 are reserved (stdin, stdout, stderr)
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            // Found an empty slot
            break;
        }
    }

    // UFDT is full; maximum open files limit reached
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // 3. Allocate memory and initialize file structure
    
    // Allocate memory for the file table entry
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialize file table properties
    uareaobj.UFDT[i] -> ReadOffset = 0;
    uareaobj.UFDT[i] -> WriteOffset = 0;
    uareaobj.UFDT[i] -> Mode = permission;

    // Link the file table to the inode
    uareaobj.UFDT[i] -> ptrinode = temp;

    // Initialize inode properties
    strcpy(uareaobj.UFDT[i] -> ptrinode -> FileName, name);
    uareaobj.UFDT[i] -> ptrinode -> FileSize = MAXFILESIZE;
    uareaobj.UFDT[i] -> ptrinode -> ActualFileSize = 0;
    uareaobj.UFDT[i] -> ptrinode -> FileType = REGULARFILE;
    uareaobj.UFDT[i] -> ptrinode -> ReferenceCount = 1;
    uareaobj.UFDT[i] -> ptrinode -> Permission = permission;

    // Allocate memory for file data buffer
    uareaobj.UFDT[i] -> ptrinode -> Buffer = (char *)malloc(MAXFILESIZE);
    
    // Safety: Initialize buffer
    memset(uareaobj.UFDT[i] -> ptrinode -> Buffer, 0, MAXFILESIZE);

    // Decrement the free inode count
    superobj.FreeInodes--;

    // Return the file descriptor
    return i;

}// End of createFile()


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         lsFile()
//  Description:           Lists all available files
//  Input:                 void
//  Output:                void
//  Author:                Ritesh Jillewad
//  Date:                  16/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void lsFile()
{
    PINODE temp = head;
    printf("----------------------------------------------------------------------------\n");
    printf("%-8s%-20s%-10s%-10s\n", "Inode", "File Name", "Size", "Actual Size");
    printf("----------------------------------------------------------------------------\n");
    while(temp != NULL)
    {
        if(temp -> FileType != 0)
        {
            printf("%-8d%-20s%-10d%-10d\n", temp->InodeNumber, temp->FileName, temp->FileSize, temp->ActualFileSize);
        }
        temp = temp -> next;
    }
    printf("----------------------------------------------------------------------------\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         unlinkFile()
//  Description:           Deletes a file
//  Input:                 Filename
//  Output:                Status Code (Integer)
//  Author:                Ritesh Jillewad
//  Date:                  22/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int unlinkFile(char *name)
{
    int i = 0;
    PINODE temp = head;

    // Validate filename
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check if file exists
    if(isFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // 1. Locate the file in the Inode list
    while(temp != NULL)
    {
        if((temp -> FileType != 0) && (strcmp(temp -> FileName, name) == 0)) 
        {
            break;
        }
        temp = temp -> next;
    }

    // 2. If the file is open, close it (release UFDT entry)
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(uareaobj.UFDT[i] -> ptrinode == temp)
            {
                 free(uareaobj.UFDT[i]);
                 uareaobj.UFDT[i] = NULL;
            }
        }
    }

    // 3. Release Inode resources
    if(temp -> Buffer != NULL)
    {
        free(temp -> Buffer);
        temp -> Buffer = NULL;
    }

    temp -> FileSize = 0;
    temp -> ActualFileSize = 0;
    temp -> FileType = 0;
    temp -> ReferenceCount = 0;
    temp -> Permission = 0;
    memset(temp -> FileName, 0, 20); 

    // Increment the free inodes count
    superobj.FreeInodes++;

    return EXECUTE_SUCCESS;
}// End of unlinkFile()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         writeFile()
//  Description:           Writes data into a file
//  Input:                 File Descriptor, Data Buffer, Size of Data
//  Output:                Number of bytes written or Error Code
//  Author:                Ritesh Jillewad
//  Date:                  22/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int writeFile(int fd, char *data, int size)
{
    /*
    printf("File descriptor: %d\n", fd);
    printf("Data to write: %s\n", data);
    printf("Bytes to write: %d\n", size);
    */

    // Validate file descriptor
    if((fd < 0) || (fd > MAXOPENFILES))
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check if file descriptor is valid (file is open)
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Check for write permission
    if(uareaobj.UFDT[fd] -> ptrinode -> Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Check for sufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd] -> WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Perform write operation
    strncpy(uareaobj.UFDT[fd] -> ptrinode -> Buffer + uareaobj.UFDT[fd] -> WriteOffset, data, size);   //  strncpy(uareaobj.UFDT[fd] -> ptrinode -> buffer This is the base address

    // Update write offset
    uareaobj.UFDT[fd] -> WriteOffset = uareaobj.UFDT[fd] -> WriteOffset + size;

    // Update actual file size
    uareaobj.UFDT[fd] -> ptrinode -> ActualFileSize = uareaobj.UFDT[fd] -> ptrinode -> ActualFileSize + size;

    // Return the number of bytes written
    return size;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         readFile()
//  Description:           Reads data from a file
//  Input:                 File Descriptor, Output Buffer, Size of Data
//  Output:                Number of bytes read or Error Code
//  Author:                Ritesh Jillewad
//  Date:                  22/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int readFile(int fd, char *data, int size)
{
    // Validate file descriptor
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Validate buffer
    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check if file descriptor is valid
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Check for read permission
    if(uareaobj.UFDT[fd] -> ptrinode -> Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Check if requested data size exceeds available data
    if((uareaobj.UFDT[fd] -> ptrinode -> ActualFileSize - uareaobj.UFDT[fd] -> ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Perform read operation
    strncpy(data, uareaobj.UFDT[fd] -> ptrinode -> Buffer + uareaobj.UFDT[fd] -> ReadOffset, size);

    // Update the read offset
    uareaobj.UFDT[fd] -> ReadOffset = uareaobj.UFDT[fd] -> ReadOffset + size;

    // Return the number of bytes read
    return size;
}// End of readFile()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         statFile()
//  Description:           Displays information about a given file
//  Input:                 Filename
//  Output:                Status Code (Integer)
//  Author:                Ritesh Jillewad
//  Date:                  26/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int statFile(char *name)
{
    PINODE temp = NULL;
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    temp = head;
    while(temp != NULL)
    {
        if(strcmp(name, temp -> FileName) == 0 && (temp -> FileType) == REGULARFILE)
        {
            break;
        }
        
        temp = temp -> next;
    }

    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    printf("\n----------------------------------------------------------------------------\n");
    printf("-------------------- Statistical Information of File -----------------------\n");
    printf("----------------------------------------------------------------------------\n");
    printf("File Name           : %s\n", temp -> FileName);
    printf("Inode Number        : %d\n", temp -> InodeNumber);
    printf("File Size           : %d\n", temp -> FileSize);
    printf("Actual File Size    : %d\n", temp -> ActualFileSize);
    printf("Link Count          : %d\n", temp -> ReferenceCount);
    printf("Reference Count     : %d\n", temp -> ReferenceCount);

    if(temp -> Permission == 1)
    {
        printf("File Permission     : Read only\n");
    }
    else if(temp -> Permission == 2)
    {
        printf("File Permission     : Write\n");
    }
    else if(temp -> Permission == 3)
    {
        printf("File Permission     : Read & Write\n");
    }
    
    printf("----------------------------------------------------------------------------\n\n");

    return EXECUTE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         fstatFile()
//  Description:           Displays information about a file using File Descriptor
//  Input:                 File Descriptor
//  Output:                Status Code (Integer)
//  Author:                Ritesh Jillewad
//  Date:                  26/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fstatFile(int fd)
{
    PINODE temp = NULL;

    // Validate file descriptor
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check if file is closed/invalid
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    temp = uareaobj.UFDT[fd]->ptrinode;

    printf("\n----------------------------------------------------------------------------\n");
    printf("-------------------- Statistical Information of File -----------------------\n");
    printf("----------------------------------------------------------------------------\n");
    printf("File Name           : %s\n", temp -> FileName);
    printf("Inode Number        : %d\n", temp -> InodeNumber);
    printf("File Size           : %d\n", temp -> FileSize);
    printf("Actual File Size    : %d\n", temp -> ActualFileSize);
    printf("Link Count          : %d\n", temp -> ReferenceCount);
    printf("Reference Count     : %d\n", temp -> ReferenceCount);

    if(temp -> Permission == 1)
        printf("File Permission     : Read only\n");
    else if(temp -> Permission == 2)
        printf("File Permission     : Write\n");
    else if(temp -> Permission == 3)
        printf("File Permission     : Read & Write\n");
    
    printf("----------------------------------------------------------------------------\n\n");

    return EXECUTE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         openFile()
//  Description:           Opens an existing file
//  Input:                 Filename, Mode (1=Read, 2=Write, 3=Read+Write)
//  Output:                File Descriptor (Integer)
//  Author:                Ritesh Jillewad
//  Date:                  26/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int openFile(char *name, int mode)
{
    PINODE temp = NULL;
    int i = 0;

    // Validation: Check parameters
    if(name == NULL || mode <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Now we need to find the file
    temp = head;
    while(temp != NULL)
    {
        if(strcmp(name, temp -> FileName) == 0 && (temp -> FileType) == REGULARFILE)
        {
            // This means we found the file we want to open
            break;
        }

        temp = temp -> next;
    }

    // If temp is NULL, we reached the end without finding the file
    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Check Permissions
    // Strict Check: Ensure the requested mode matches or is allowed by the file's permission
    if (temp->Permission != mode && temp->Permission != (READ + WRITE)) 
    {
        return ERR_PERMISSION_DENIED;
    }

    // Find a free User File Descriptor Table (UFDT) slot
    // Start from 0 (standard) or 3 if you wish to reserve stdin/out/err like create
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // If i reached max, the table is full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate memory for FileTable
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));
    if(uareaobj.UFDT[i] == NULL)
    {
        return -1; // Memory allocation failed
    }

    // Initialize FileTable
    uareaobj.UFDT[i] -> ReadOffset = 0;
    uareaobj.UFDT[i] -> WriteOffset = 0;
    uareaobj.UFDT[i] -> Mode = mode;
    
    // Link to the Inode
    uareaobj.UFDT[i] -> ptrinode = temp;

    // Update Inode Metadata
    uareaobj.UFDT[i] -> ptrinode->ReferenceCount++;

    return i; // Return the File Descriptor
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         closeFile()
//  Description:           Closes an existing open file
//  Input:                 File Descriptor (Integer)
//  Output:                Status Code (0 for success, negative for error)
//  Author:                Ritesh Jillewad
//  Date:                  26/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int closeFile(int fd)
{
    // Validation: Check if FD is correct
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // File we want to close, does not exists, or is not opened
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // We need to decrement the reference count, as the we are closing the file
    uareaobj.UFDT[fd] -> ptrinode -> ReferenceCount--;

    // Free the memory of the FileTable structure
    free(uareaobj.UFDT[fd]);

    // Reset the UFDT entry to NULL so this FD can be reused
    uareaobj.UFDT[fd] = NULL;
    
    return EXECUTE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         truncateFile()
//  Description:           Removes all data from a file (size becomes 0)
//  Input:                 Filename
//  Output:                Status Code
//  Author:                Ritesh Jillewad
//  Date:                  26/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int truncateFile(char *name)
{
    PINODE temp = NULL;
    int i = 0;

    // if name field is null/missing
    if(name == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    temp = head;
    while(temp != NULL)
    {
        if(strcmp(name, temp -> FileName) == 0 && (temp -> FileType) == REGULARFILE)
        {
            // File to truncate is found
            break;
        }

        temp = temp -> next;
    }

    // File not found
    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Check Permissions (Must have WRITE permission to modify data)
    if(temp -> Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Wipe the data
    // We verify the buffer exists, then fill it with 0
    if(temp -> Buffer != NULL)
    {
        memset(temp -> Buffer, '\0', MAXFILESIZE);
    }

    // Now we reset the actual file size
    temp -> ActualFileSize = 0;

    // Reset Offsets for Open Files
    // If this file is currently open in any slot of the UFDT, we must reset 
    // the cursor (offsets) back to 0, otherwise the cursor will point to nowhere.
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(uareaobj.UFDT[i] -> ptrinode == temp)
            {
                uareaobj.UFDT[i] -> ReadOffset = 0;
                uareaobj.UFDT[i] -> WriteOffset = 0;
            }
        }
    }

    return EXECUTE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         renameFile()
//  Description:           Renames an existing file
//  Input:                 Old Filename, New Filename
//  Output:                Status Code
//  Author:                Ritesh Jillewad
//  Date:                  27/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int renameFile(char *oldName, char *newName)
{
    PINODE temp = NULL;

    // If new name and old name parameters are null
    if(oldName == NULL || newName == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check if the old file exists
    if(isFileExists(oldName) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Check if the new name is already taken by another file
    if(isFileExists(newName) == true)
    {
        return ERR_FILE_ALREADY_EXISTS;
    }

    // Now we need to find the inode
    temp = head;
    while(temp != NULL)
    {
        if(strcmp(oldName, temp -> FileName) == 0)
        {
            // We found the file
            break;
        }

        temp = temp -> next;
    }

    // safety check
    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Now we simply update the old filename with new filename
    strcpy(temp -> FileName, newName);

    return EXECUTE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         catFile()
//  Description:           Display entire file content to standard output
//  Input:                 Filename
//  Output:                Status Code
//  Author:                Ritesh Jillewad
//  Date:                  27/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int catFile(char *name)
{
    PINODE temp = NULL;

    // If name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Finding the file
    temp = head;
    while(temp != NULL)
    {
        if(strcmp(name, temp -> FileName) == 0)
        {
            // Found the file
            break;
        }

        temp = temp -> next;
    }

    // File not found
    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // If read permission is not given
    if(temp -> Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Check if file is empty
    if(temp -> ActualFileSize == 0)
    {
        printf("File is empty!\n");
        return EXECUTE_SUCCESS;
    }

    // There is data, we will print it
    printf("File contents: \n");
    printf("%s\n", temp -> Buffer);

    return EXECUTE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         copyFile()
//  Description:           Copies the content from source file to destination file
//  Input:                 Source filename, Destination filename
//  Output:                Status Code
//  Author:                Ritesh Jillewad
//  Date:                  27/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int copyFile(char *src, char *dest)
{
    PINODE tempSrc = NULL;
    PINODE tempDest = NULL;
    int fd = 0;

    // Name validation
    if(src == NULL || dest == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Check if Source exists
    if(isFileExists(src) == false)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Check if Destination already exists (if it already exists we cannot copy it as)
    if(isFileExists(dest) == true)
    {
        return ERR_FILE_ALREADY_EXISTS;
    }

    // Create the Destination File
    // We create it with READ+WRITE permissions (3) by default
    fd = createFile(dest, 3);
    if(fd < 0)
    {
        return fd; // Return the error code from createFile (e.g., ERR_NO_INODES)
    }

    // Finding the source node
    tempSrc = head;
    while(tempSrc != NULL)
    {
        if(strcmp(src, tempSrc -> FileName) == 0)
        {
            // Found the source file
            break;
        }
        tempSrc = tempSrc -> next;
    }

    // Get Destination Inode from the FD we just created
    // uareaobj.UFDT[fd] points to the FileTable, which points to the Inode
    tempDest = uareaobj.UFDT[fd] -> ptrinode;

    // Perform the Copy
    // Copy the actual data buffer
    memcpy(tempDest -> Buffer, tempSrc -> Buffer, MAXFILESIZE);
    
    // Copy the metadata (Size)
    tempDest -> ActualFileSize = tempSrc -> ActualFileSize;
    
    return EXECUTE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         backupCVFS()
//  Description:           Saves data to disk
//  Input:                 void
//  Output:                void
//  Author:                Ritesh Jillewad
//  Date:                  28/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int backupCVFS()
{
    PINODE temp = NULL;
    int fd = 0;

    // Open the file 
    fd = open(BACKUP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if(fd == -1)
    {
        printf("Error: Unable to open backup file.\n");
        return -1;
    }

    // Traverse the list
    temp = head;
    while(temp != NULL)
    {
        // If filetype is 0, it means it's regualr file, but the backup file is binary
        if(temp -> FileType != 0)
        {
            // Write data
            write(fd, temp -> FileName, sizeof(temp -> FileName));
            write(fd, &temp -> InodeNumber, sizeof(temp -> InodeNumber));
            write(fd, &temp -> ActualFileSize, sizeof(temp -> ActualFileSize));
            write(fd, &temp -> Permission, sizeof(temp -> Permission));
            
            // Write the buffer content
            write(fd, temp -> Buffer, MAXFILESIZE);
        }
        temp = temp -> next;
    }

    // Close the file descriptor
    close(fd);
    
    return EXECUTE_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         restoreCVFS()
//  Description:           Restores the data from disk
//  Input:                 void
//  Output:                void
//  Author:                Ritesh Jillewad
//  Date:                  28/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void restoreCVFS()
{
    PINODE temp = head;
    int fd = 0;
    int iRet = 0;

    // Temporary variables
    char name[20] = {'\0'};
    int inodeNum = 0;
    int fileSize = 0;
    int permission = 0;

    // Open the backup file
    fd = open(BACKUP_FILE, O_RDONLY);

    if(fd == -1)
    {
        printf("CVFS: No backup file found. Starting fresh.\n");
        return;
    }

    // Read the file
    // read() returns the number of bytes read. If it returns 0, it means End of File (EOF).
    while((iRet = read(fd, name, sizeof(name))) > 0)
    {
        if(temp == NULL) break;

        // Read the rest of the metadata
        read(fd, &inodeNum, sizeof(int));
        read(fd, &fileSize, sizeof(int));
        read(fd, &permission, sizeof(int));
        
        // Read the data buffer
        read(fd, temp -> Buffer, MAXFILESIZE);

        // 3. Restore to Inode
        strcpy(temp -> FileName, name);
        temp -> ActualFileSize = fileSize;
        temp -> Permission = permission;
        temp -> FileType = REGULARFILE; 

        // Update Superblock
        superobj.FreeInodes--;

        temp = temp -> next;
    }

    // Close the file descriptor
    close(fd);
    printf("CVFS: System restored successfully.\n");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name:         chmodFile()
//  Description:           Changes the permissions of the file
//  Input:                 Filename, new permission
//  Output:                void
//  Author:                Ritesh Jillewad
//  Date:                  28/01/2026
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int chmodFile(char *name, int new_permission)
{
    PINODE temp = NULL;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Validating the permission range
    // 1 = READ
    // 2 = WRITE
    // 3 = READ + WRITE
    if(new_permission < 1 || new_permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // Now we need to find the file
    temp = head;
    while(temp != NULL)
    {
        if(strcmp(name, temp -> FileName) == 0)
        {
            // We found the file
            break;
        }

        temp = temp -> next;
    }

    // File not found
    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXISTS;
    }

    // Now we will update the permission
    temp -> Permission = new_permission;

    return EXECUTE_SUCCESS;
}
