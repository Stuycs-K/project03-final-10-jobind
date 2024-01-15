//DONT FORGET TO COMMENT THIS FILE. THIS IS DENSE CODE, STUPID
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
char tiletochar(int t){
    if(t==0) return 'w';
    if(t==1) return '_';
    return '$';
}
char layertochar(int l){
    if(l==0) return '\0';
    if(l==1) return 'M';
    if(l==2) return 'C';
    return '$';
}
char * printer(struct country* c,int id){
    struct terr land = c->domain[id];
    char*outstring=calloc(1000,sizeof(char));
    if(land.fd==0){
        sprintf(outstring,"No land owned with id: %d\n", id);
        return outstring;
    }
    struct tile* tileset = malloc(30*30*sizeof(struct tile));
    //printf("FILE DESCRIPTOR: %d\n", land.fd);//TESTING
    //printf("BYTES ATTEMPTED: %ld\n",30*30*sizeof(struct tile));//TESTING
    int k = read(land.fd,tileset,30*30*sizeof(struct tile));
    //printf("BYTES READ SUCCESFULLY: %d\n",k);//TESTING
    outstring[0]='\0';
    for(int i=0; i<30; i++){
        for(int j=0; j<30; j++){
            char t = tiletochar(tileset[30*i+j].base);
            char l = layertochar(tileset[30*i+j].layer);
            printf("(%c,%c) ",t,l);//TESTING
            fflush(stdout);
            if(l=='\0'){
                strcat(outstring,&t);
            } else{
                strcat(outstring,&l);
            }
        }
        strcat(outstring,"\n");
    }
    return outstring;
}
int* myrandom(int n){
    int rd = open("/dev/random", O_RDONLY);
    int *rand = calloc(n,sizeof(int));
    read(rd,rand,n*sizeof(int));
    close(rd);
    return rand;
}
double myabs(double x){
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
    int bseed = myabs((double)(r[0]%5));
    return bseed / ((double)dif);
}
//There are 30 tiles per row, and per column. each tile is 2*8 bytes (2 ints)
int terraingen(double biome, double prod, char*fname){
    char * filename = malloc(200*sizeof(char));
    sprintf(filename,"%s.region",fname);
    //printf("AttemptedName: %s\n", fname);//TESTING
    int _fd = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0666);
    //printf("FDs FROM INIT: %d\n", _fd);//TESTING
    struct tile* tileset = malloc(30*30*sizeof(struct tile));
    for(int i=0; i<30*30; i++){
        double r = myabs(myrandomdouble());
        tileset[i].base=0;
        tileset[i].layer=0;
        //if(r<0.15){tileset[i].layer=1;}
        //if(r<0.05){tileset[i].layer=2;}
        r = myabs(myrandomdouble());
        //if(r<0.15){tileset[i].base=0;}
    }
    int k = write(_fd,tileset,30*30*sizeof(struct tile));
    //printf("BYTES WRITTEN SUCCESSFULLY: %d\n", k);//TESTING
    close(_fd);
    _fd = open(filename, O_RDWR, 0666);
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