#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

char* date(time_t time){
    char *ctime_no_nl;
    // Numero di secondi in base dell'argomento passato
    time_t currentTime = time;
    // Conversione in una datta leggibile e troncatura dello \n a fine riga
    ctime_no_nl = strtok(ctime(&currentTime), "\n");
    return ctime_no_nl;
}

// Metodo per la scrittura del file di output
void writeOut (const char *pathname) {
    struct stat fileStat;
    stat(pathname,&fileStat);
    FILE* fout = fopen("/home/navi/workspace/Ulamogv2/filestat.db","a+");
    // Scrittura dei parametri richiesti
    fprintf(fout,"<%s> ",date(time(NULL)));
    fprintf(fout,"<%d> ",fileStat.st_uid);
    fprintf(fout,"<%d> ",fileStat.st_gid);
    fprintf(fout,"<%ld> ",fileStat.st_size);
    fprintf(fout,(S_ISDIR(fileStat.st_mode)) ? "<d" : "<-");
    fprintf(fout,(fileStat.st_mode & S_IRUSR) ? "r" : "-");
    fprintf(fout,(fileStat.st_mode & S_IWUSR) ? "w" : "-");
    fprintf(fout,(fileStat.st_mode & S_IXUSR) ? "x" : "-");
    fprintf(fout,(fileStat.st_mode & S_IRGRP) ? "r" : "-");
    fprintf(fout,(fileStat.st_mode & S_IWGRP) ? "w" : "-");
    fprintf(fout,(fileStat.st_mode & S_IXGRP) ? "x" : "-");
    fprintf(fout,(fileStat.st_mode & S_IROTH) ? "r" : "-");
    fprintf(fout,(fileStat.st_mode & S_IWOTH) ? "w" : "-");
    fprintf(fout,(fileStat.st_mode & S_IXOTH) ? "x>" : "->");
    fprintf(fout," <%s> ",date(fileStat.st_atime));
    fprintf(fout," <%s> ",date(fileStat.st_mtime));
    fprintf(fout," <%s> \n",date(fileStat.st_ctime));
    fprintf(fout,"###\n");
    fclose(fout);
}

// Metodo che cerca ricorsivamente i file all'interno delle cartelle
int recWrite (char *pathname) {
    DIR *dir;
    struct dirent *ent;
    writeOut(pathname);
    // Apre e scansiona la cartella
    if ((dir = opendir (pathname)) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            // Salta un ciclo se il nome è . o ..
            if (strcmp(".",ent->d_name) == 0 || strcmp("..",ent->d_name) == 0) {
                continue;
            }
            // Aggiornamento del pathname
            char *buffer = malloc(strlen(pathname)+strlen(ent->d_name)+1+sizeof("/"));
            strcpy(buffer,pathname);
            strcat(buffer, "/");
            strcat(buffer,ent->d_name);
            // Se trova una cartella, chiama il metodo ricorsivamente
            if (ent->d_type == 4) {
                recWrite(buffer);
                free(buffer);     
            } else {
                // Altrimenti scrive nel file di output e continua con la scansione
                writeOut(buffer);
                free(buffer);
            }
        }
        closedir (dir);
    } else {
        perror ("");
        return EXIT_FAILURE;
    }
}
