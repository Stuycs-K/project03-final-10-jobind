//STRUCTS
/*Notes on country:
dif is the difficulty rating, used for calculations.
Higher dif means worse odds WRT the nation.
military is troop count.
GDP is economic wellbeing. (wealth/yr)
wealth is accumulated GDP.
the struct domain* points to all of the nation's territories.
*/
struct country {int dif; int GDP; int wealth; int military; char name[100]; struct domain* _domain; };
/*Notes on domain:
keeps track of a nation's territory.
MAY BE MOVED TO territory.h IN THE FUTURE
*/
struct domain {int size; /*struct terr*[40];TERRITORY STRUCT*/};


//COMMENTS ON FUNCTIONS IN nation.c FILE
int rivalcount();
struct country** rivalbirth();