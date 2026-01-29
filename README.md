# 🗂️ Customised Virtual File System (CVFS)
A Linux-style in-memory virtual file system implemented in C, designed to simulate core operating system file system concepts such as inodes, file descriptors, permissions, and metadata management using custom data structures.

This project provides a command-line shell that allows users to create, manage, and manipulate virtual files entirely in RAM, closely mirroring how real file systems operate internally.

## 📌 Project Overview

The Customised Virtual File System (CVFS) is a monolithic C-based application that emulates the internal working of a UNIX/Linux file system.
It focuses on how file systems work beneath the hood, rather than on user-level APIs.

**All files, metadata, and structures exist in primary memory (RAM), allowing fast access while maintaining realistic file system behavior.**

This project is ideal for understanding:
- Operating System internals
- File system architecture
- Low-level resource management
- Data structure design in C

## 🎯 Key Objectives
- Simulate a Linux-like file system environment
- Implement inode-based file management
- Manage file descriptors and permissions
- Provide a custom shell interface for interaction
- Demonstrate core OS concepts using pure C and DSA

## 🚀 Features
- **Virtual Storage:** All files and metadata are stored in primary memory (RAM), simulating a disk.
- **Command Line Interface:** Custom shell to interact with the file system.
- **File Operations:** Support for standard operations like `creat`, `open`, `read`, `write`, and `unlink`.
- **Permission Management:** UNIX-style permissions:
  - Read (1)
  - Write (2)
  - Read + Write (3)
- **Metadata Management:** `stat` and `fstat` commands to view file details (inode number, size, permissions).
- **Persistence (Backup/Restore):** Ability to save the virtual file system state to a hard disk file `(CVFS_Backup.bin) and restore it later.
- **Resource Management:** Handles up to 20 open files and a configurable number of maximum inodes.

## 🧠 Internal Architecture
CVFS is designed around classic Linux file system structures.

### 🧩 Data Structures Used

| Data Structure | Description |
|:-------------|:-----------|
| **SuperBlock** | Maintains global file system metadata such as total inodes, free inodes, and file system status. |
| **Inode** | Stores file metadata including file name, inode number, file size, permissions, link count, and data buffer pointer. |
| **FileTable** | Maintains information about opened files such as read/write offsets, access mode, and reference count. |
| **UFDT (User File Descriptor Table)** | An array that maps file descriptors to their respective `FileTable` entries. |
| **DILB (Doubly Linked List)** | Maintains the Disk Inode List Block, linking all inodes in the file system. |
| **BootBlock** | Stores initial boot-time metadata and assists in file system initialization. |
| **Character Buffer** | Stores actual file data in memory (simulates disk data blocks). |

## 🗃️ Project Structure
```
CVFS/
│
├── cvfs.h
│   └── Macros, structures, constants, and function declarations
│
├── cvfs_helper.c
│   └── Core file system logic and operations
│
├── main.c
│   └── Entry point and command interpreter loop
│
└── CVFS_Backup.bin
    └── Persistent backup file (generated at runtime)
```
## 📖 Commands Reference

| Command | Usage | Description |
|:--------|:-------|:-------------|
| `man` | `man [command]` | Displays detailed manual for a specific command. |
| `help` | `help` | Lists all supported commands. |
| `creat` | `creat [filename] [permission]` | Creates a new file with given permissions (1: Read, 2: Write, 3: Read+Write). |
| `open` | `open [filename] [mode]` | Opens an existing file in specified mode. |
| `read` | `read [fd] [bytes]` | Reads specified number of bytes from an open file. |
| `write` | `write [fd]` | Writes data to an open file. |
| `ls` | `ls` | Lists all files present in the virtual file system. |
| `stat` | `stat [filename]` | Displays metadata of a file using its name. |
| `fstat` | `fstat [fd]` | Displays metadata of a file using its file descriptor. |
| `truncate` | `truncate [filename]` | Removes all data from a file without deleting it. |
| `rm` | `rm [filename]` | Deletes (unlinks) a file from the file system. |
| `cp` | `cp [source] [destination]` | Copies data from source file to destination file. |
| `rename` | `rename [oldname] [newname]` | Renames an existing file. |
| `backup` | `backup` | Saves the current file system state to disk. |
| `restore` | `restore` | Restores the file system state from disk. |
| `close` | `close [fd]` | Closes an open file descriptor. |
| `clear` | `clear` | Clears the console screen. |
| `exit` | `exit` | Terminates the CVFS application. |

## 🛠️ Tech Stack

| Layer | Technologies Used | Description |
|:----|:-----------------|:-----------|
| **Programming Language** | C | Core implementation language used for building the virtual file system and data structures. |
| **Compiler** | GCC (MinGW on Windows) | Compiles the C source code across platforms. |
| **Operating System Concepts** | Linux / UNIX File System | Inodes, file descriptors, permissions, UFDT, and superblock concepts. |
| **Data Structures** | Linked List, Arrays, Structs | Used to implement inodes, UFDT, file tables, and metadata handling. |
| **Memory Management** | Heap & Stack (RAM) | Entire file system is simulated in primary memory. |
| **CLI Interface** | Custom Shell (C-based) | Provides a UNIX-like command-line interface for interacting with CVFS. |
| **Persistence** | Binary File I/O | Backup and restore functionality using binary file operations. |
| **Development Tools** | VS Code / GCC Toolchain | Code development, debugging, and compilation. |
| **Version Control** | Git & GitHub | Source code management and project collaboration. |

> All commands are implemented using custom system-call–like functions written in C, simulating Linux file system behavior.

## ⚙️ Compilation & Execution

### 🔧 Prerequisites

* GCC Compiler
  * Linux/macOS: GCC
  * Windows: MinGW

### 🛠️ Build Instructions
```c
gcc main.c cvfs_helper.c -o cvfs
```

### ▶️ Run the Application
**Linux**
```
./cvfs
```
**Windows**
```
cvfs
```

## 🧪 Example Session
<img width="1919" height="973" alt="image" src="https://github.com/user-attachments/assets/b2e6d481-054b-4200-a4ed-ce6fad3f5628" />

<img width="1919" height="789" alt="image" src="https://github.com/user-attachments/assets/dbd014dd-e3ec-4fdf-b6a2-91909c4a2c3a" />

<img width="1919" height="983" alt="image" src="https://github.com/user-attachments/assets/113b4526-8528-4c9f-b67d-0543eae14912" />




## 👤 Author
**Ritesh Jillewad** <br>
This project is developed for academic learning purposes to understand core Operating System concepts such as file systems, inodes, file descriptors, and memory management.
