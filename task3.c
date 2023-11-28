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
#include "task3.h"

//TASK 3
uint16_t * loadFATtoMemory(int fd, uint16_t * fat_array, BootSector * bootSector){
	int FATmemSize = bootSector -> BPB_FATSz16 * bootSector -> BPB_BytsPerSec;
	int reservedSize = bootSector -> BPB_RsvdSecCnt * bootSector -> BPB_BytsPerSec;
    
	fat_array = (uint16_t*) malloc(FATmemSize);
    
    for (int i = 0; i < FATmemSize; i++){
        loadMemory(fd, &fat_array[i], sizeof(uint16_t), reservedSize + (i * sizeof(uint16_t)));
    }

    return fat_array;
}
void displayOrderedClusterList(uint16_t startingCluster, uint16_t * fat_array){
    uint16_t currentCluster = startingCluster;
    while (currentCluster < 0xfff8){
        printf("Current Cluster: %d\n", currentCluster);
        currentCluster = fat_array[currentCluster];
    }
    printf("End of File\n");
    printf("\n");
}
