#ifndef TASK4NTASK6_H
#define TASK4NTASK6_H
#include <stdbool.h>

#include "task1.h" 
#include "task2.h" 
#include "task3.h"

extern BootSector *bootSector;
extern uint16_t *fat_array;


typedef struct __attribute__((__packed__)){
    uint8_t DIR_Name[ 11 ]; // Non zero terminated string
    uint8_t DIR_Attr; // File attributes
    uint8_t DIR_NTRes; // Used by Windows NT, ignore
    uint8_t DIR_CrtTimeTenth; // Tenths of sec. 0...199
    uint16_t DIR_CrtTime; // Creation Time in 2s intervals
    uint16_t DIR_CrtDate; // Date file created
    uint16_t DIR_LstAccDate; // Date of last read or write
    uint16_t DIR_FstClusHI; // Top 16 bits file's 1st cluster
    uint16_t DIR_WrtTime; // Time of last write
    uint16_t DIR_WrtDate; // Date of last write
    uint16_t DIR_FstClusLO; // Lower 16 bits file's 1st cluster
    uint32_t DIR_FileSize; // File size in bytes
} Directory;

typedef struct __attribute__((__packed__)){
    uint8_t LDIR_Ord; // Order/ position in sequence/ set
    uint8_t LDIR_Name1[ 10 ]; // First 5 UNICODE characters
    uint8_t LDIR_Attr; // = ATTR_LONG_NAME (xx001111)
    uint8_t LDIR_Type; // Should = 0
    uint8_t LDIR_Chksum; // Checksum of short name
    uint8_t LDIR_Name2[ 12 ]; // Middle 6 UNICODE characters
    uint16_t LDIR_FstClusLO; // MUST be zero
    uint8_t LDIR_Name3[ 4 ]; // Last 2 UNICODE characters
}LongDirectory;


Directory * loadRootDirectory(int fd, BootSector * bootSector, Directory * directoryArray);
Directory *loadSubdirectory(int fd, BootSector *bootSector, int clusterNumber);


int getStartingCluster(uint16_t lowCluster, uint16_t highCluster);
bool validCluster(int cluster);
void displayTime(uint16_t time);
void displayDate(uint16_t date);
void displayAttribute(uint16_t attribute);
bool validFileName(uint8_t * directoryName);
void displayDirectory(Directory * directory, int num, LongDirectory * longDirectoryArray, int * idx);
void displayDirectoryDetails(BootSector * bootSector, Directory * directoryArrays);
void displaySubDirectory(BootSector * bootSector, Directory * directoryArray, int entryNum);

int countEntriesInCluster(BootSector *bootSector, int clusterNumber);

LongDirectory *loadLongDirectory(Directory *directoryArray, LongDirectory * longDirectoryArray);
char ** displayLongDirectory(LongDirectory *longDirectoryArray);
char * extractLongFileName(LongDirectory * longDirectoryArray, int i, int index);

#endif 
