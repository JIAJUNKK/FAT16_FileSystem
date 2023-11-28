#include "task1.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int loadMemory(int fd, void * destination, int size, int offset){
    int jumpedLocation = lseek(fd, offset, SEEK_SET);
    int readFile = read(fd, destination, size);

    //printf("bytes read: %d\n", readFile);
    //printf("Location Jumped: %d\n", jumpedLocation);

    return readFile;
}