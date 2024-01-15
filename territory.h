struct terr {int fd; double biome; double prod; };
struct tile {int base; int layer;};
struct terr * terrbirth(int dif, char* fname);
int terraingen(double biome, double prod, char* fname);
int* myrandom(int n);
int abs(int x);
double choosebiome(int dif);
double myrandomdouble();