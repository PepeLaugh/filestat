// Metodo per il calcolo della data richiesta
char* date(time_t time);

// Scrittura su file di output con opzione --verbose
void verbosePrint (char *pathname, struct stat fileStat);

// Scrittura su file di output
void printStat (char *pathname, struct stat fileStat);

// Metodo che controlla se il file richiesto è già stato scansionato almeno una volta
int findMatch(char* search, char *file, struct stat fileStat, bool verb);

// Metodo che gestisce la corretta creazione della struct stat e del file di output
void writeOut (char *pathname, char *dest, bool link, bool verb);

// Cerca i file all'interno delle directory
int recWrite (char *pathname, char *dest, bool link, bool verb);

// Stampa del file di output
void noscan(char *pathname);

// Metodo che cerca il pathname indicato e ne stampa le statistiche
void history(char *pathname, char *dest);

// Scrive ### per la fine del file
void endOfFile(char *pathname);
