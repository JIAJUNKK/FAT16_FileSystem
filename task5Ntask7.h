#ifndef TASK5NTASK7_H
#define TASK5NTASK7_H

#include "task1.h" 
#include "task2.h" 
#include "task3.h"
#include "task4Ntask6.h"



typedef struct{
    BootSector * bootSector;
}Volume;

typedef struct {
    Volume *volume;
    Directory * directoryArray;
    int fd; 
} File;

extern File * openFile(Volume *volume, Directory * directoryArray);
extern off_t seekFile (File * file, off_t offset, int whence, int startCluster);
extern size_t readFile(File *file, void *buffer, size_t length);
extern void closeFile(File *file);

off_t calculateSubdirectoryOffset(File * file, int clusterNumber);
char * getClusterBytes(File * file, int startCluster, size_t length, bool isSubDirectory);

int choiceRootDirectory();
int choiceSubDirectory();
void displayFileContent(File * file, size_t length, int clusterSize, int startCluster, bool isSubDirectory);
void displaySubDirectoryFileContent(File * file, Directory * parentDirectoryArray, int parentStartCluster, int selfStartCluster, bool isSubDirectory);
void displayRootDirectoryContent(File * file, int choice);





#endif