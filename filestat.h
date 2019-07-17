char* date(time_t time);

void verbosePrint (char *pathname, struct stat fileStat);

void printStat (char *pathname, struct stat fileStat);

void writeOut (char *pathname, bool link, bool verb);

int recWrite (char *pathname, bool link, bool verb);

void noscan(char *pathname);
