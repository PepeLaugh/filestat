#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include "filestat.h"

long start;
long end;
int nfile;
long maxSize;

char* date(time_t time){
    char *ctime_no_nl;
    // Numero di secondi in base dell'argomento passato
    time_t currentTime = time;
    // Conversione in una datta leggibile e troncatura dello \n a fine riga
    ctime_no_nl = strtok(ctime(&currentTime), "\n");
    return ctime_no_nl;
}

// Metodo per la scrittura su file con l'opzione verbose
void verbosePrint (char *pathname, struct stat fileStat) {
    FILE* fout = fopen(pathname,"a+");
    fprintf(fout,"<%s> ",date(time(NULL)));
    printf("%s ",date(time(NULL)));
    fprintf(fout,"<%d> ",fileStat.st_uid);
    printf("%d ",fileStat.st_uid);
    fprintf(fout,"<%d> ",fileStat.st_gid);
    printf("%d ",fileStat.st_gid);
    fprintf(fout,"<%ld> ",fileStat.st_size);
    printf("%ld ",fileStat.st_size);
    fprintf(fout,(S_ISDIR(fileStat.st_mode)) ? "<d" : "<-");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    fprintf(fout,(fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    fprintf(fout,(fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    fprintf(fout,(fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    fprintf(fout,(fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    fprintf(fout,(fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    fprintf(fout,(fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    fprintf(fout,(fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    fprintf(fout,(fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    fprintf(fout,(fileStat.st_mode & S_IXOTH) ? "x> " : "-> ");
    printf((fileStat.st_mode & S_IXOTH) ? "x " : "- ");
    fprintf(fout,"<%s> ",date(fileStat.st_atime));
    printf("%s ",date(fileStat.st_atime));
    fprintf(fout,"<%s> ",date(fileStat.st_mtime));
    printf("%s ",date(fileStat.st_mtime));
    fprintf(fout,"<%s> ",date(fileStat.st_ctime));
    printf("%s ",date(fileStat.st_ctime));
    fprintf(fout,"<%ld> \n",fileStat.st_nlink);
    printf("%ld\n\n",fileStat.st_nlink);
    fprintf(fout,"###\n");
    fclose(fout);
}

// Metodo per la scrittura su file
void printStat (char *pathname, struct stat fileStat) {
    FILE* fout = fopen(pathname,"a+");
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
    fprintf(fout,(fileStat.st_mode & S_IXOTH) ? "x> " : "-> ");
    fprintf(fout,"<%s> ",date(fileStat.st_atime));
    fprintf(fout,"<%s> ",date(fileStat.st_mtime));
    fprintf(fout,"<%s> ",date(fileStat.st_ctime));
    fprintf(fout,"<%ld> \n",fileStat.st_nlink);
    fprintf(fout,"###\n");
    fclose(fout);
}
// Metodo per la scrittura del file di output
void writeOut (char *pathname, bool link, bool verb) {
    struct stat fileStat;
    if (link) {
        // Stats del link
        lstat(pathname,&fileStat);
    } else {
        // Stats del file a cui fa riferimento il link
        stat(pathname,&fileStat);
    }
    FILE* fout = fopen("filestat.db","a+");
    char temp[512];
    // Creazione di una copia del file di output
    FILE* copy = fopen("copy.db","a+");
    // Flag per l'eventuale presenza del file richiesto
	int find_result = 0;
    // Modifica del pathname per facilitare la scansione
    char *buffer = malloc(strlen(pathname)+sizeof("<"));
    strcpy(buffer,pathname);
    strcat(buffer, ">");
	while(fgets(temp, 512, fout) != NULL) {
        // Ciclo il file di output copiando ogni riga
        fputs(temp,copy);
		if((strstr(temp, buffer)) != NULL) {
            // Se trova una stringa uguale al pathname, aggiorno il flag
            find_result = 1;
            while(fgets(temp, 512, fout) != NULL){
                // Scansiono ogni riga finchè non trovo la fine del file desiderato
                if (strstr(temp,"###") != NULL) {
                    fclose(copy);
                    // Aggiungo una nuova entry
                    if (verb){
                        printf("%s\n",pathname);
                        verbosePrint("copy.db",fileStat);
                    } else {
                        printStat("copy.db",fileStat);
                    }
                    fopen("copy.db","a+");
                    // Copio il resto del file
                    while(fgets(temp, 512, fout) != NULL) {
                        fputs(temp,copy);
                    }
                    break;
                }
                // Copio finchè non trovo la stringa desiderata
                fputs(temp,copy);
            }
		}
        if (find_result == 1) {
            break;
        }
	}
    free(buffer);
    fclose(copy);
    if (find_result == 0){
        // Se non è stato trovato un match, aggiungo il pathname del nuovo file e scansiono
        fprintf(fout,"# <%s>\n",pathname);
        if (verb) {
            printf("%s\n",pathname);
        }
        fclose(fout);
        remove("copy.db");
        if (verb) {
            verbosePrint("filestat.db",fileStat);
        } else {
            printStat("filestat.db",fileStat);
        }

    } else {
        // Se è stato trovato un match, scambio la copia aggiornata con il vecchio file
        fclose(fout);
        remove("filestat.db");
        rename("copy.db","filestat.db");
    }
}

// Metodo che cerca ricorsivamente i file all'interno delle cartelle
int recWrite (char *pathname, bool link, bool verb) {
    DIR *dir;
    struct dirent *ent;
    writeOut(pathname,link,verb);
    // Apre e scansiona la cartella
    if ((dir = opendir (pathname)) != NULL) {
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
                recWrite(buffer,link,verb);
                free(buffer);
            } else {
                // Altrimenti scrive nel file di output e continua con la scansione
                writeOut(buffer,link,verb);
                free(buffer);
            }
        }
        closedir (dir);
    } else {
        perror ("");
        return EXIT_FAILURE;
    }
}

void noscan(char *pathname){
    char temp[512];
    FILE* f = fopen(pathname,"r");
    while (fgets(temp,512,f) != NULL) {
        printf("%s", temp);
    }
    fclose(f);
}