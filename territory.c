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
//There are 32 tiles per row, and per column. each tile is 2*8 bytes (2 ints)
int terraingen(double biome, double prod, char*fname){
    char * filename = malloc(200*sizeof(char));
    sprintf(filename,"regions/%s.region",fname);
    int _fd = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0666);
    struct tile* tileset = malloc(32*32*sizeof(struct tile));
    for(int i=0; i<32*32; i++){
        tileset[i].base=1;
        tileset[i].layer=0;
    }
    write(_fd,tileset,32*32*sizeof(struct tile));
    return _fd;
}
struct terr * terrbirth(int dif, char * fname){
    struct terr * t = malloc(1*sizeof(struct terr));
    double _biome = choosebiome(dif);
    int _fd = terraingen(_biome,1/_biome,fname);
    t->fd = _fd;
    t->biome = _biome;
    t->prod = 1 / _biome;
}