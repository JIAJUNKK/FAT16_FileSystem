#ifndef TASK1_H
#define TASK1_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int loadMemory(int fd, void * destination, int size, int offset);

#endif 