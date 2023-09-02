#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "disk.h"

char *month_to_string(short m) {
    switch(m) {
    case 1: return "Jan";
    case 2: return "Feb";
    case 3: return "Mar";
    case 4: return "Apr";
    case 5: return "May";
    case 6: return "Jun";
    case 7: return "Jul";
    case 8: return "Aug";
    case 9: return "Sep";
    case 10: return "Oct";
    case 11: return "Nov";
    case 12: return "Dec";
    default: return "?!?";
    }
}


void unpack_datetime(unsigned char *time, short *year, short *month, 
    short *day, short *hour, short *minute, short *second)
{
    assert(time != NULL);

    memcpy(year, time, 2);
    *year = htons(*year);

    *month = (unsigned short)(time[2]);
    *day = (unsigned short)(time[3]);
    *hour = (unsigned short)(time[4]);
    *minute = (unsigned short)(time[5]);
    *second = (unsigned short)(time[6]);
}


int main(int argc, char *argv[]) {
    superblock_entry_t sb;
    int  i;
    char *imagename = NULL;
    FILE *f;
    
    directory_entry_t dir_entry;
    short year, month, day, hour, minute, second;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--image") == 0 && i+1 < argc) {
            imagename = argv[i+1];
            i++;
        }
    }
    
    
    f = fopen(imagename, "rb");
    if (f == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Read the superblock from the disk image
    fread(&sb, sizeof(superblock_entry_t), 1, f);
    fclose(f);

    // Open the disk image again for reading the root directory
    f = fopen(imagename, "rb");
    if (f == NULL) {
        perror("Error opening file");
        exit(1);
    }
    
    if((fread(&sb, sizeof(superblock_entry_t), 1, f)!=1)){
        fprintf(stderr, "Problems reading superblock\n");
        fclose(f);
        exit(1);
    }
    if(strncmp(sb.magic, FILE_SYSTEM_ID, FILE_SYSTEM_ID_LEN)!=0){
        fprintf(stderr,"%s is not in proper format\n", imagename);
        fclose(f);
        exit(1);
        
    }

    if (imagename == NULL)
    {
        fprintf(stderr, "usage: ls360fs --image <imagename>\n");
        exit(1);
    }

    // Move the file pointer to the start of the root directory
    fseek(f, ntohl(sb.dir_start) * ntohs(sb.block_size), SEEK_SET);

    // Read and display each entry in the root directory
    for (i = 0; i < MAX_DIR_ENTRIES; i++) {
        fread(&dir_entry, sizeof(directory_entry_t), 1, f);

        // Check if the status indicates an available entry
        if (dir_entry.status == DIR_ENTRY_AVAILABLE) {
            continue;
        }

        // Unpack date and time from the directory entry
        unpack_datetime(dir_entry.create_time, &year, &month, &day, &hour, &minute, &second);

        // Display the information for each file in the root directory
        printf("%8u %2d-%s-%2d %02d:%02d:%02d %s\n", 
               ntohl(dir_entry.file_size),
               year,
               month_to_string(month),
               day, hour, minute, second,
               dir_entry.filename);
    }

    fclose(f);

    return 0; 
}

