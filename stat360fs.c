#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "disk.h"


int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int i;
    char *imagename = NULL;
    FILE *f;
    int *fat_data;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i + 1 < argc) {
            imagename = argv[i + 1];
            i++;
        }
    }

    if (imagename == NULL) {
        fprintf(stderr, "usage: stat360fs --image <imagename>\n");
        exit(1);
    }

    f = fopen(imagename, "rb");
    if (f == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Read the superblock
    fread(&sb, sizeof(superblock_entry_t), 1, f);
    if (strncmp(sb.magic, FILE_SYSTEM_ID, FILE_SYSTEM_ID_LEN) != 0) {
        fprintf(stderr, "Invalid file system identifier\n");
        fclose(f);
        exit(1);
    }
    
    char *name = strrchr(imagename, '/');

    if (name) {
        name++; // Move the pointer to skip the '/'
        strcpy(imagename, name); // Copy the filename to imagename
    }

    printf("%s (%s)\n", sb.magic, imagename);
    printf("----------------------------------------------\n");
    printf("Bsz   Bcnt   FATst   FATcnt   DIRst   DIRcnt\n");
    printf("%-5u %-6u %-6u   %-7u   %-6u     %-6u\n",
           ntohs(sb.block_size), ntohl(sb.num_blocks),
           ntohl(sb.fat_start), ntohl(sb.fat_blocks),
           ntohl(sb.dir_start), ntohl(sb.dir_blocks));
    printf("----------------------------------------------\n");

    // Move to the FAT section
    fseek(f, ntohl(sb.fat_start) * ntohs(sb.block_size), SEEK_SET);

    // Read the FAT entries
    
    fat_data = (int *)malloc(ntohl(sb.fat_blocks) * ntohs(sb.block_size));
    fread(fat_data, ntohs(sb.block_size), ntohl(sb.fat_blocks), f);

    int free_blocks = 0;
    int reserved_blocks = 0;
    int allocated_blocks = 0;

    for (i = 0; i < ntohl(sb.num_blocks); i++) {
        int fat_entry = ntohl(fat_data[i]);
        if (fat_entry == FAT_AVAILABLE) {
            free_blocks++;
        } else if (fat_entry == FAT_RESERVED) {
            reserved_blocks++;
        } else{
            allocated_blocks++;
        }
    }

    printf("Free   Resv   Alloc\n");
    printf("%-6d %-6d %-6d\n", free_blocks, reserved_blocks, allocated_blocks);

    fclose(f);
    free(fat_data);
    return 0;
}
