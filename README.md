# 360fs File System

## Introduction
The 360fs file system emulates a simple file system comprising a root directory, File Allocation Table (FAT), and block-based storage. This README provides an overview and guidance for tasks related to interacting with this file system.

## File System Components
The file system consists of four major components:

### Directory Entry Fields
Each directory entry contains:
- **Status**: Bit mask denoting file status, availability, and type.
- **Starting Block**: First block of the file associated with the entry.
- **Number of Blocks in File**: Total blocks occupied by the file.
- **File Size (in bytes)**: Size of the file in bytes.
- **Create Time/Modify Time**: Date and time info for file creation and modification.
- **Filename**: A string (max 30 chars) representing the file name.

### File Allocation Table (FAT)
The FAT is an array of 4-byte integers used to manage file block allocations.

## Tasks Overview
This project involves three main tasks:

### Task 1: stat360fs.c
Create `stat360fs.c` to extract and display information from a 360fs file system image. This includes reading the superblock and retrieving FAT entries.

### Task 2: ls360fs.c
Develop `ls360fs.c` to generate a listing of the root directory for a 360fs file system image in a specific format.

### Task 3: cat360fs.c
Write `cat360fs.c` to copy a file from the disk image to the console's standard output. If the file is not found in the root directory, display a "file not found" message and exit.

## Implementation Details
- **Disk Image Size**: Determined by block size (a power of 2) multiplied by the file system size (blocks).
- **File Retrieval from FAT**: Locate a file's starting block in the directory entry and navigate subsequent blocks using pointers in the FAT entries until the end of the file.

The tasks aim to create utilities for interacting with the 360fs file system, offering insights into file system structure, FAT navigation, and file metadata extraction.

