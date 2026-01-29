//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                          ENTRY POINT FUNCTION OF PROJECT
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "cvfs.h"

int main()
{
    char str[80] = {'\0'};
    char Command[5][80];                        // Buffer to store parsed command tokens
    char InputBuffer[MAXFILESIZE] = {'\0'};
    char * EmptyBuffer = NULL;

    int iCount = 0;
    int iRet = 0;

    // Initialize the auxilary data
    startAuxillaryDataInitialization();

    printf("\n");
    printf("----------------------------------------------------------------------------\n");
    printf("------------ Customised Virtual Filesystem Started Successfully ------------\n");
    printf("----------------------------------------------------------------------------\n");

    while(1)                                                                    /* Infinite listening loop */
    {
        fflush(stdin);
        strcpy(str, " ");

        printf("\nCVFS > ");
        fgets(str, sizeof(str), stdin);                                         /* Read input line */

        // Tokenize the command string
        iCount = sscanf(str, "%s %s %s %s %s", Command[0], Command[1], Command[2], Command[3], Command[4]);

        fflush(stdin);

        // Handle commands with up to 4 arguments
        

        // Single command (count = 1)
        if(iCount == 1)
        {
            /* exit command */
            /* CVFS > exit */
            if(strcmp("exit", Command[0]) == 0)
            {
                printf("Thank you for using CVFS.\n");
                printf("Releasing resources...\n");
                break;                                                         // End of infinite listening loop
            }// End of exit command

            /* ls command */
            /* CVFS > ls */
            else if(strcmp("ls", Command[0]) == 0)
            {
                lsFile();
            }// End of ls command

            /* single man command */
            /* CVFS > man */
            else if(strcmp("man", Command[0]) == 0)
            {
                printf("Which manual page do you want?\n");
                printf("Usage example: 'man man'.\n");
            }// End of sinlge man command

            /* help command */
            /* CVFS > help */
            else if(strcmp("help", Command[0]) == 0)
            {
                displayHelp();
            }// end of help command

            /* clear command */
            /* CVFS > clear */
            else if(strcmp("clear", Command[0]) == 0)
            {
                // conditional preprocessing
                #ifdef _WIN32                           // for windows
                    system("cls");     
                #else
                    system("clear");                    // for linux
                #endif
            } // end of clear command

            /* backup command */
            else if(strcmp("backup", Command[0]) == 0)
            {
                iRet = backupCVFS();
                if(iRet == EXECUTE_SUCCESS)
                {
                     printf("CVFS: Backup created successfully.\n");
                }
                else
                {
                     printf("CVFS: Error creating backup.\n");
                }
            }

            /* restore command */
            else if(strcmp("restore", Command[0]) == 0)
            {
                restoreCVFS();
            }

            else 
            {
                printf("ERROR: Command '%s' not recognized! Refer to 'help' for command info.\n", Command[0]);
            }

        }// End of iCount == 1

        
        // Two commands on terminal
        else if(iCount == 2)
        {
            /* man command */
            /* CVFS > man command_name */
            if(strcmp("man", Command[0]) == 0)
            {
                manPageDisplay(Command[1]);            // Command[1] contains the name of command
            }

            /* truncate command */
            /* CVFS > truncate demo.txt */
            else if(strcmp("truncate", Command[0]) == 0)
            {
                iRet = truncateFile(Command[1]);

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File data truncated successfully.\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error: File does not exist.\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error: Permission denied (File is Read-Only).\n");
                }
            }

            /* stat command */
            /* CVFS > stat filename */
            else if(strcmp("stat", Command[0]) == 0)
            {
                iRet = statFile(Command[1]);
                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("ERROR: File not found.\n");
                }
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("ERROR: Invalid parameters.\n");
                }
            }

            /* fstat command */
            /* CVFS > fstat file_descriptor */
            else if(strcmp("fstat", Command[0]) == 0)
            {
                iRet = fstatFile(atoi(Command[1]));
                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("ERROR: File descriptor not open/found.\n");
                }
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("ERROR: Invalid parameters.\n");
                }
            }

            /* CVFS :> unlink Demo.txt or rm Demo.txt */
            /* unlink/rm command  */
            else if(strcmp("unlink", Command[0]) == 0 || strcmp("rm", Command[0]) == 0)
            {
                /* man command_name */
                iRet = unlinkFile(Command[1]);
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("ERROR: Invalid parameters.\n");
                }

                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("ERROR: Deletion failed. File does not exist.\n");
                }

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File deleted successfully.\n");
                }
            }

            /* close command */
            /* CVFS > close fd */
            else if(strcmp("close", Command[0]) == 0)
            {
                iRet = closeFile(atoi(Command[1]));
                
                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File descriptor %d closed successfully.\n", atoi(Command[1]));
                }
                else
                {
                    printf("Error closing file.\n");
                }
            }

            /* write command */
            /* CVFS > write 2(fd) */
            else if(strcmp("write", Command[0]) == 0)
            {
                printf("Enter the data: \n");
                fgets(InputBuffer, MAXFILESIZE, stdin);

                iRet = writeFile(atoi(Command[1]), InputBuffer, strlen(InputBuffer) - 1);           // We sent the stirng excuding \0
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("ERROR: Invalid parameters.\n");
                }
                
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("ERROR: File does not exist.\n");
                }

                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("ERROR: Write failed. Permission denied.\n");
                }

                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("ERROR: Write failed. Insufficient space.\n");
                }

                else
                {
                    printf("%d bytes were successfully written.\n", iRet);
                }
            }

            // cat command
            // CVFS > cat filename
            else if(strcmp("cat", Command[0]) == 0)
            {
                iRet = catFile(Command[1]);
                if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error: File does not exist.\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error: Permission denied (File is Write-Only).\n");
                }
            }

            else 
            {
                printf("ERROR: Command '%s' not recognized! Refer to 'help' for command info.\n", Command[0]);
            }
        }// End of iCount = 2


        // Three commands on the terminal
        else if(iCount == 3)
        {

            /* creat command */
            /* CVFS > create hello.txt 3 */
            if(strcmp("creat", Command[0]) == 0)
            {
                iRet = createFile(Command[1], atoi(Command[2]));    // atoi will conver the ascii value to integer value, since command[2] is string, so it will return the ascii value

                /* Handling error macros/codes */

                // invalid file parameters
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("ERROR: Creation failed. Invalid parameters.\n");
                    printf("Please refer to the manual.\n");
                }

                // Inodes are finished
                if(iRet == ERR_NO_INODES)
                {
                    printf("ERROR: Creation failed. No free inodes available.\n");
                }

                if(iRet == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("ERROR: Creation failed. File already exists.\n");
                }

                if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("ERROR: Creation failed.\n");
                    printf("Maximum open files limit reached.\n");
                }

                printf("File created successfully. File Descriptor: %d\n", iRet);
            }// End of createFile()


            // open command
            // CVFS > open demo.txt 2
            else if(strcmp("open", Command[0]) == 0)
            {
                iRet = openFile(Command[1], atoi(Command[2]));

                if(iRet >= 0)
                {
                    printf("File opened successfully with FD : %d\n", iRet);
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error: File does not exist.\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error: Permission denied.\n");
                }
                else
                {
                    printf("Error opening file.\n");
                }
            }


            // read command
            // CVFS > read 3 10
            else if(strcmp("read", Command[0]) == 0)
            {
                EmptyBuffer = (char*)malloc(atoi(Command[2]) + 1);

                iRet = readFile(atoi(Command[1]), EmptyBuffer, atoi(Command[2]));
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("ERROR: Invalid parameters.\n");
                }

                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("ERROR: File does not exist.\n");
                }

                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("ERROR: Permission denied.\n");
                }

                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("ERROR: Read failed. Insufficient data in file.\n");
                }

                else
                {
                    printf("Read operation successful.\n");
                    EmptyBuffer[iRet] = '\0';
                    printf("Data read from file: %s\n", EmptyBuffer);

                    free(EmptyBuffer);
                }
            }

            // rename file command
            // CVFS > rename old.txt new.txt
            else if(strcmp("rename", Command[0]) == 0)
            {
                iRet = renameFile(Command[1], Command[2]);
                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File renamed successfully.\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error: The source file does not exist.\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("Error: A file with the new name already exists.\n");
                }
            }

            // copy command
            // CVFS > cp Demo.txt Hello.txt
            else if(strcmp("cp", Command[0]) == 0)
            {
                iRet = copyFile(Command[1], Command[2]);

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File copied successfully.\n");
                }
                else if(iRet == ERR_FILE_NOT_EXISTS)
                {
                    printf("Error: Source file does not exist.\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("Error: Destination file already exists.\n");
                }
                else if(iRet == ERR_NO_INODES)
                {
                    printf("Error: No free inodes to create destination file.\n");
                }
            }

            else 
            {
                printf("ERROR: Command '%s' not recognized! Refer to 'help' for command info.\n", Command[0]);
            }
        }// End of iCount == 3
        
        else if(iCount == 4)
        {

        }

        else 
        {
            printf("Command not found.\n");
            printf("Type 'help' for the list of commands.\n");
        }// End of else
    }// End of while

    return 0;
}// End of main