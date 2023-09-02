#Project Overview:

Welcome to the "Exploring the 360fs File System" project. In this project, you will delve into the intricacies of the 360fs file system, a simplified file system, and develop three programs to interact with it. Each program serves a specific purpose, allowing you to gain practical experience in working with file systems.
360fs File System Overview:

Before delving into the details of the programs, let's familiarize ourselves with the key aspects of the 360fs file system:

    #File System Components:
        The 360fs file system comprises four main components: Superblock, File Allocation Table (FAT), Root Directory, and Data Blocks.

    #Superblock:
        The Superblock holds critical file system metadata.
        Metadata includes information such as the file system identifier, block size, file system size, FAT location, and more.

    #Root Directory:
        The Root Directory stores directory entries for files within the file system.
        Each directory entry contains various fields, including status, starting block, file size, timestamps, and filename.

    #File Allocation Table (FAT):
        The FAT resembles an array-based linked list, mapping blocks to files.
        It plays a crucial role in locating the blocks associated with a file.

    #Data Blocks:
        Data Blocks hold the actual file data.

#Program Tasks:

Now, let's explore the tasks you will undertake in this project:

#Program 1: stat360fs.c

The first program, stat360fs.c, aims to display detailed information about a 360fs file system image. This program will read the superblock of the disk image and utilize that information to retrieve entries in the FAT. The program's output will follow a specific format, providing valuable insights into the file system's structure and metadata.

#Program 2: ls360fs.c

The second program, ls360fs.c, is designed to list the contents of the root directory within a 360fs file system image. It will display the files present in the root directory along with their respective sizes. The output format adheres to predefined standards.

#Program 3: cat360fs.c

In the third program, cat360fs.c, created a utility that copies a file from a disk image to the standard output stream of the console. If the specified file is not found in the root directory, the program will gracefully handle the situation by printing a "file not found" message and exiting.

#Project Progress:

It is essential to gain a comprehensive understanding of the 360fs file system's structure before embarking on the implementation of the three programs. Take time to familiarize yourself with the file system's organization and the data representations specified in the project description.

#Testing and Validation:

Throughout the project, rigorously test your programs with various disk images to ensure their correctness and robustness. Debugging and validation are critical steps in delivering reliable software.
