struct terr {int fd; double biome; double prod; };
struct tile {int base; int layer;};
struct terr * terrbirth(int dif);
int terraingen(double biome, double prod);
int* myrandom(int n);
int abs(int x);
double choosebiome(int dif);