#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "disk.h"

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int i;
    char *imagename = NULL;
    char *filename = NULL;
    FILE *f;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i + 1 < argc) {
            imagename = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
            filename = argv[i + 1];
            i++;
        }
    }

    if (imagename == NULL || filename == NULL) {
        fprintf(stderr, "usage: cat360fs --image <imagename> --file <filename in image>\n");
        exit(1);
    }

    f = fopen(imagename, "rb");
    if (f == NULL) {
        fprintf(stderr, "cat360fs: unable to open image %s\n", imagename);
        exit(1);
    }

    fread(&sb, sizeof(superblock_entry_t), 1, f);
    if (memcmp(sb.magic, FILE_SYSTEM_ID, FILE_SYSTEM_ID_LEN) != 0) {
        fprintf(stderr, "cat360fs: %s is not in the proper format\n", imagename);
        exit(1);
    }

    int block_size = ntohs(sb.block_size);
    int dir_start = ntohl(sb.dir_start);
    int dir_blocks = ntohl(sb.dir_blocks);

    int entry_per_block = block_size / sizeof(directory_entry_t);
    int num_entries = dir_blocks * entry_per_block;

    directory_entry_t *dir = (directory_entry_t *)malloc(dir_blocks * block_size);
    if (dir == NULL) {
        fprintf(stderr, "cat360fs: problems malloc memory for dir\n");
        fclose(f);
        exit(1);
    }

    fseek(f, dir_start * block_size, SEEK_SET);
    if (fread(dir, block_size, dir_blocks, f) != dir_blocks) {
        fprintf(stderr, "cat360fs: problems reading directory from image\n");
        fclose(f);
        free(dir);
        exit(1);
    }

    int file_found = 0;
    int file_block_start = 0;
    int file_size = 0;

    for (i = 0; i < num_entries; i++) {
        if (dir[i].status == DIR_ENTRY_NORMALFILE && strcmp(dir[i].filename, filename) == 0) {
            file_found = 1;
            file_block_start = ntohl(dir[i].start_block);
            file_size = ntohl(dir[i].file_size);
            break;
        }
    }

    if (!file_found) {
        fprintf(stderr, "cat360fs: file not found (%s)\n", filename);
        fclose(f);
        free(dir);
        exit(1);
    }

    int *FAT = (int *)malloc(ntohl(sb.fat_blocks) * block_size);
    if (FAT == NULL) {
        fprintf(stderr, "cat360fs: problems malloc memory for FAT\n");
        fclose(f);
        free(dir);
        exit(1);
    }

    fseek(f, ntohl(sb.fat_start) * block_size, SEEK_SET);
    if (fread(FAT, block_size, ntohl(sb.fat_blocks), f) != ntohl(sb.fat_blocks)) {
        fprintf(stderr, "cat360fs: fewer FAT entries than expected\n");
        fclose(f);
        free(dir);
        free(FAT);
        exit(1);
    }

    char *buffer = (char *)malloc(block_size);
    if (buffer == NULL) {
        fprintf(stderr, "cat360fs: problems malloc memory during file read\n");
        fclose(f);
        free(dir);
        free(FAT);
        exit(1);
    }

    while (1) {
        fseek(f, file_block_start * block_size, SEEK_SET);
        fread(buffer, sizeof(char), block_size, f);

        int bytes_to_read = (file_size > block_size) ? block_size : file_size;
        fwrite(buffer, sizeof(char), bytes_to_read, stdout);

        file_block_start = ntohl(FAT[file_block_start]);
        if (file_block_start == FAT_LASTBLOCK) {
            break;
        }

        file_size -= bytes_to_read;
    }

    free(buffer);
    free(dir);
    free(FAT);
    fclose(f);
    return 0;
}
