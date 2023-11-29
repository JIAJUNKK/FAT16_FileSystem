#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4Ntask6.h"
#include "task5Ntask7.h"

BootSector * bootSector;

Directory * directoryArray; 
Directory * subdirectoryArray;
LongDirectory * longDirectoryArray;
uint16_t * fat_array;

Directory directory;
LongDirectory Ldirectory;


int main(){
    ssize_t readFile;
    int fd = open("/Users/JIAJUNKK_1/Desktop/211OperatingSystem/fat16.img", O_RDONLY);
    if (fd == -1){
        printf("There is an error opening the file. Check the directory name\n");
        exit(0);
    }
    printf("%d", fd);
//TASK 1
    size_t count = 1024; 
    char * buffer = malloc(count);
    loadMemory(fd, buffer, count, 2048);

    free(buffer);
    printf("\n");
//TASK 2
    lseek(fd, 0, SEEK_SET);
    bootSector = loadBootSector(fd);
    displayBootSector(bootSector);

//TASK 3
    fat_array = loadFATtoMemory(fd, fat_array, bootSector);

    /*
    for (int i = 0; i < 100; i++){
        printf("%d\n", fat_array[i]);
    }
    */
    
    displayOrderedClusterList(148, fat_array);

//Task 4
    lseek(fd, 0, SEEK_SET);
    directoryArray = loadRootDirectory(fd, bootSector, directoryArray);


    displayDirectoryDetails(bootSector, directoryArray);
    

//TASK 5 
    Volume *volume = malloc(sizeof(Volume));
    volume->bootSector = bootSector;
    File * file = openFile(volume, directoryArray);
    file -> fd = fd; 
    while (true){
        int rootChoice = choiceRootDirectory();
        if (rootChoice == -1){
            break;
        }
        displayRootDirectoryContent(file, rootChoice);
        printf("\n\n\n\n\n");
        displayDirectoryDetails(bootSector, directoryArray);
    }
    closeFile(file);


//TASK 6

    
    close(fd);
    return 0;

}





