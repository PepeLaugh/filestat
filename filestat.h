char* date(time_t time);

void verbosePrint (char *pathname, struct stat fileStat);

void printStat (char *pathname, struct stat fileStat);

void writeOut (char *pathname, char *dest, bool link, bool verb);

int recWrite (char *pathname, char *dest, bool link, bool verb);

void noscan(char *pathname);

void findaMatch(char* search, char *file, struct stat fileStat, bool verb);

void history(char *pathname, char *dest);
