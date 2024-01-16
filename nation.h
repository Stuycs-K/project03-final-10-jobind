//STRUCTS
/*Notes on country:
dif is the difficulty rating, used for calculations.
Higher dif means worse odds WRT the nation.
military is troop count.
GDP is economic wellbeing. (wealth/yr)
wealth is accumulated GDP.
the struct domain* points to all of the nation's territories.
*/
struct country {int dif; int GDP; int wealth; int military; char name[100]; struct terr* domain; };
/*Notes on domain:
keeps track of a nation's territory.
MAY BE MOVED TO territory.h IN THE FUTURE
*/


//COMMENTS ON FUNCTIONS IN nation.c FILE
int rivalcount();
struct country** rivalbirth();
char* aicountryname(int d);
double smoothinate(double r);
int invest(struct country* c, int d);
int train(struct country* c, int d);
int findc(struct country**w,int size,char*cname);