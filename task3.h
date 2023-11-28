#ifndef TASK3_H
#define TASK3_H

#include "task1.h" 
#include "task2.h" 

uint16_t * loadFATtoMemory(int fd, uint16_t * fat_array, BootSector * bootSector);
void displayOrderedClusterList(uint16_t startingCluster, uint16_t * fat_array);

#endif 
