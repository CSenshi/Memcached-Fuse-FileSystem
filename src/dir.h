#ifndef DIR_H
#define DIR_H

#define CHILD_C 100
#define MAX_FNAME 128

#include "memcached.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define ROOT_DIR_INODE 23

typedef struct dir
{
    char dir_name[MAX_FNAME];
    int inode;
    int par_inode;
    int chunk_inode;
    mode_t mode;
} dir;

int dir_create(const char *, mode_t, memcached *);
void dir_init(dir *, const char *, mode_t, memcached *);

#endif // !DIR_H
