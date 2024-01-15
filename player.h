#ifndef player_h
#define player_h
struct country* playersetup(char*name,int difficulty);
char * stats(struct country* c);
char* cmdhandler(char*cmd,int phase,struct country* c);
char* helper(int phase);
char* countryname();
int difficultyselect();
struct country* birth();
#endif