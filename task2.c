#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "task1.h"
#include "task2.h"

BootSector * loadBootSector(int fd){
    BootSector * bootSector = (BootSector *) malloc (sizeof(BootSector));
    loadMemory(fd, bootSector, sizeof(BootSector), 0);
    return bootSector;
}
void displayBootSector(BootSector * bootSector){
    printf("Bytes per Sector: %hu\n", bootSector -> BPB_BytsPerSec);
    printf("Sectors per Cluster %hu\n", bootSector -> BPB_SecPerClus);
    printf("Reserved Sector Count: %hu\n", bootSector -> BPB_RsvdSecCnt);
    printf("Number of Copies of FAT: %hu\n", bootSector -> BPB_NumFATs);
    printf("FAT12/FAT16: size of root DIR: %hu\n", bootSector -> BPB_RootEntCnt);
    printf("Sectors, may be 0, see below: %hu\n", bootSector -> BPB_TotSec16);
    printf("Sectors in FAT (FAT12 or FAT16): %hu\n", bootSector -> BPB_FATSz16);
    printf("Sectors if BPB_TotSec16 == 0: %hu\n", bootSector -> BPB_TotSec32);
    printf("Non zero terminated string: %s\n", bootSector -> BS_VolLab);
    printf("\n");
}