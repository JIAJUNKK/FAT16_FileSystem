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
#include "task4Ntask6.h"



//TASK 4
Directory * loadRootDirectory(int fd, BootSector * bootSector, Directory * directoryArray){
    int directoryMemSize = bootSector -> BPB_RootEntCnt * sizeof(Directory);
    directoryArray = (Directory *) malloc(directoryMemSize);
    off_t directoryReservedSize = (bootSector -> BPB_RsvdSecCnt + bootSector -> BPB_NumFATs * bootSector -> BPB_FATSz16) * bootSector -> BPB_BytsPerSec;
    loadMemory(fd, directoryArray, directoryMemSize, directoryReservedSize);
    return directoryArray;
}



Directory *loadSubdirectory(int fd, BootSector *bootSector, int clusterNumber) {
    int directoryMemSize = bootSector->BPB_SecPerClus * bootSector->BPB_BytsPerSec;
    Directory *subdirectoryArray = (Directory *)malloc(directoryMemSize); // Start with space for multiple clusters
    Directory *currentSubdirectory = subdirectoryArray;
    int currentCluster = clusterNumber;
    int offset = 1;
    int totalEntries = directoryMemSize / sizeof(Directory);
    int clustersLoaded = 1; 

    while (currentCluster <= 0xfff8 && currentCluster > 0) {
        int FirstDataSector = bootSector->BPB_RsvdSecCnt + (bootSector->BPB_NumFATs * bootSector->BPB_FATSz16) + ((bootSector->BPB_RootEntCnt * sizeof(Directory)) + (bootSector->BPB_BytsPerSec - 1)) / bootSector->BPB_BytsPerSec;
        int startingSector = FirstDataSector + (currentCluster - 2) * bootSector->BPB_SecPerClus;
        if (offset > clustersLoaded * totalEntries) {
            clustersLoaded += 3; 
            subdirectoryArray = (Directory *)realloc(subdirectoryArray, directoryMemSize * clustersLoaded);
            currentSubdirectory = subdirectoryArray + offset;
        }
        loadMemory(fd, currentSubdirectory, directoryMemSize, startingSector * bootSector->BPB_BytsPerSec);
        offset += totalEntries;
        currentCluster = fat_array[currentCluster];
    }
    return subdirectoryArray;
}




/*
Directory *loadSubdirectory(int fd, BootSector *bootSector, int clusterNumber) {
    int directoryMemSize = bootSector->BPB_SecPerClus * bootSector->BPB_BytsPerSec;
    Directory *subdirectoryArray = (Directory *)malloc(directoryMemSize);
    Directory *currentSubdirectory = subdirectoryArray;

    int currentCluster = clusterNumber;
    while (currentCluster <= 0xfff8 && currentCluster > 0) {
        int FirstDataSector = bootSector->BPB_RsvdSecCnt + (bootSector->BPB_NumFATs * bootSector->BPB_FATSz16) + ((bootSector->BPB_RootEntCnt * sizeof(Directory)) + (bootSector->BPB_BytsPerSec - 1)) / bootSector->BPB_BytsPerSec;
        int startingSector = FirstDataSector + (currentCluster - 2) * bootSector -> BPB_SecPerClus;
        printf("Debug: currentCluster: %d\n", currentCluster);
        printf("Debug: FirstDataSector: %d\n", FirstDataSector);
        printf("Debug: startingSector: %d\n", startingSector);
        loadMemory(fd, currentSubdirectory, directoryMemSize, startingSector * bootSector->BPB_BytsPerSec);
        currentCluster = fat_array[currentCluster];

        printf("Debug: After loadMemory - currentCluster: %d\n", currentCluster);

        if (currentCluster > 0xfff8) {
            printf("This is the end cluster: %d\n", currentCluster);
            break;
        }
        //currentSubdirectory ++;
        currentSubdirectory += directoryMemSize / sizeof(Directory); // Move by the size of loaded data
    }

    return subdirectoryArray;
}
*/



int getStartingCluster(uint16_t lowCluster, uint16_t highCluster) {
    int startingCluster = lowCluster | (highCluster << 16);
    if (startingCluster >= 0xfff8 || startingCluster < 0) {
        return -1; // Indicates an invalid cluster
    }
    return startingCluster;
}
bool validCluster(int cluster){
    if (cluster == -1){
        return false;
    }
    return true;
}
void displayTime(uint16_t time) {
    int hour = (time >> 11) & 0x1F;           
    int minute = (time >> 5) & 0x3F;         
    int second = (time & 0x1F) * 2;
    printf("%2s %02d:%02d:%02d %4s", "", hour, minute, second, "|");
}
void displayDate(uint16_t date) {
    int year = ((date >> 9) & 0x7F) + 1980;
    int month = (date >> 5) & 0x0F;           
    int day = date & 0x1F; 
    printf("%s %02d/%02d/%02d %4s", "", year, month, day, "|");        
}
void displayAttribute(uint16_t attribute){
    char array[7] = "-------";
    array[6] = (attribute & 0x01) ? 'R' : '-'; 
    array[5] = (attribute & 0x02) ? 'H' : '-'; 
    array[4] = (attribute & 0x04) ? 'S' : '-'; 
    array[3] = (attribute>>3 & 1) ? 'V' : '-'; 
    array[2] = (attribute & 0x10) ? 'D' : '-'; 
    array[1] = (attribute & 0x20) ? 'A' : '-'; 
    printf("%9s %2s", array, "|");
    
}
bool validFileName(uint8_t * directoryName){
    if (directoryName[0] == 0x00 || directoryName[0] == 0xE5){
        return false;
    }
    return true;
}

void displayDirectory(Directory * directory, int num, LongDirectory * longDirectoryArray, int *idx){
    char ** resArray = displayLongDirectory(longDirectoryArray);
    
    int startingCluster = getStartingCluster(directory -> DIR_FstClusLO, directory -> DIR_FstClusHI);
    if (validCluster(startingCluster) && validFileName(directory -> DIR_Name) && directory -> DIR_FileSize != -1){
        printf("%5d %4s", getStartingCluster(directory -> DIR_FstClusLO, directory -> DIR_FstClusHI), "|");
        displayTime(directory -> DIR_WrtTime);
        displayDate(directory -> DIR_WrtDate);
        displayAttribute(directory -> DIR_Attr);
        printf("%5d %3s", directory -> DIR_FileSize, "|");
        if (num == 0 || startingCluster == 0 || directory->DIR_Name[0] == '.'){
            printf("%3s %-10s", "", directory -> DIR_Name);
        }else{
            printf("%3s %-10s", "", resArray[*idx]);
            (*idx)++;
        }
        printf("\n");
        printf("----------------------------------------------------------------------------------------------------------------------------\n");
    }
}

void displayDirectoryDetails(BootSector * bootSector, Directory * directoryArray){
    LongDirectory * longDirectoryArray = loadLongDirectory(directoryArray, longDirectoryArray);
    int idx = 0;
    printf("\n\n\n\n\n");
    printf("----------------------------------------------------------------------------------------------------------------------------\n");
     printf("Starting | Last Modified | Last Modified | Attribute | File   |                          File Name                         \n"
            "Cluster  | Time          | Date          |           | Length |                                                             ");
    printf("\n----------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i <bootSector->BPB_SecPerClus * bootSector->BPB_BytsPerSec / sizeof(Directory); i ++){
        displayDirectory(&directoryArray[i], i, longDirectoryArray, &idx);
    }
    idx = 0;
    free(longDirectoryArray);
}


void displaySubDirectory(BootSector *bootSector, Directory *directoryArray, int entryNum) {
    int startingCluster = entryNum;
    while (startingCluster <= 0xfff8 && startingCluster > 0){
        Directory *subdirectoryArray = loadSubdirectory(3, bootSector, startingCluster);  
        startingCluster = fat_array[startingCluster];
        displayDirectoryDetails(bootSector, subdirectoryArray);
        free(subdirectoryArray);
    }
}

//TASK 6
LongDirectory *loadLongDirectory(Directory *directoryArray, LongDirectory * longDirectoryArray){
    int numDirEntries = bootSector->BPB_SecPerClus * bootSector->BPB_BytsPerSec;
    longDirectoryArray = (LongDirectory *)malloc(numDirEntries * sizeof(LongDirectory));
    int longDirIndex = 0;

    for (int i = 0; i < numDirEntries; i++) {
        if (directoryArray[i].DIR_Attr == 0x0f){
            memcpy(&longDirectoryArray[longDirIndex], &directoryArray[i], sizeof(LongDirectory));
            longDirIndex++;
        } 
    }
    return longDirectoryArray;
}

char ** displayLongDirectory(LongDirectory *longDirectoryArray) {
    int numDirEntries = bootSector->BPB_SecPerClus * bootSector->BPB_BytsPerSec;

    char **resArray = (char **)malloc(numDirEntries * sizeof(char *));
    for (int i = 0; i < numDirEntries; ++i) {
        resArray[i] = (char *)malloc(256 * sizeof(char)); 
        resArray[i][0] = '\0'; 
    }

    int resIndex = 0;
    char tempArray[256];
    int tempIndex = 0;

    for (int i = 0; i < numDirEntries; i++) {
        if (longDirectoryArray[i].LDIR_Ord & 0x40) {
            if (tempIndex > 0) {
                char reversedTempArray[256];
                int reversedIndex = 0;
                for (int j = tempIndex - 1; j >= 0; j--) {
                    reversedTempArray[reversedIndex++] = tempArray[j];
                }
                reversedTempArray[reversedIndex] = '\0';

                strcpy(resArray[resIndex], reversedTempArray);
                resIndex++;
                tempIndex = 0;
            }
        }

        char longFileName[256]; 
        int index = 0;
        for (int j = 0; j < 10; j += 2) {
            uint16_t character = (longDirectoryArray[i].LDIR_Name1[j + 1] << 8) | longDirectoryArray[i].LDIR_Name1[j];
            if (character == 0 || character == 0xFFFF) {
                break;  // End of the name
            }
            longFileName[index++] = (char)character;
        }
        for (int j = 0; j < 12; j += 2) {
            uint16_t character = (longDirectoryArray[i].LDIR_Name2[j + 1] << 8) | longDirectoryArray[i].LDIR_Name2[j];
            if (character == 0 || character == 0xFFFF) {
                break;  // End of the name
            }
            longFileName[index++] = (char)character;
        }
        for (int j = 0; j < 4; j += 2) {
            uint16_t character = (longDirectoryArray[i].LDIR_Name3[j + 1] << 8) | longDirectoryArray[i].LDIR_Name3[j];
            if (character == 0 || character == 0xFFFF) {
                break;
            }
            longFileName[index++] = (char)character;
        }
        longFileName[index] = '\0'; 

        if (index > 0) {
            for (int j = index - 1; j >= 0; j--) {
                tempArray[tempIndex++] = longFileName[j];
            }
        }
    }
    if (tempIndex > 0) {
        char reversedTempArray[256];
        int reversedIndex = 0;
        for (int j = tempIndex - 1; j >= 0; j--) {
            reversedTempArray[reversedIndex++] = tempArray[j];
        }
        reversedTempArray[reversedIndex] = '\0';

        strcpy(resArray[resIndex], reversedTempArray);
        resIndex++;
    }    
    return resArray;

}




