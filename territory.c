#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include "player.h"
#include "nation.h"

struct tile {int base; int layer;};
struct terr {int fd; double biome; double prod; };
int* myrandom(int n){
    int rd = open("/dev/random", O_RDONLY);
    int *rand = calloc(n,sizeof(int));
    read(rd,rand,n*sizeof(int));
    return rand;
}
int abs(int x){
    if(x<0) return -x;
    return x;
}
double myrandomdouble(){
    int r = myrandom(1)[0];
    double rd = ((double)r )/ INT_MAX;
    return rd;
}
double choosebiome(int dif){
    int* r=myrandom(2);
    int bseed = abs(r[0]%5);
    return bseed / ((double)dif);
}
//There are 32 tiles per row, and per column. each tile is 2 bytes
int terraingen(double biome, double prod){
    int _fd = open("TESTNAME.region", O_RDWR | O_TRUNC | O_CREAT, 0666);
    return _fd;
}
struct terr * terrbirth(int dif, char * fname){
    struct terr * t = malloc(1*sizeof(struct terr));
    double _biome = choosebiome(dif);
    int _fd = terraingen(_biome,1/_biome);
    t->fd = _fd;
    t->biome = _biome;
    t->prod = 1 / _biome;
}