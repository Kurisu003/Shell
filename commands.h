#ifndef COMMANDS_H
#define COMMANDS_H
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#ifdef __linux__
	#define GETDIR getcwd
#else
	#include <direct.h>
	#include <conio.h>
	#include <windows.h>
	#define GETDIR _getcwd
#endif

#define SIZE 300

//Funktion um das \n eines Strings zu entfernen, das bei fgets entsteht
void chomp(char *str){
	size_t p = strlen(str);
	str[p-1] = '\0';
}

//echo Funktion zum Ausgeben und in Dateien schreiben
//übergeben wird die Eingabe, eine Variable zum Überprüfen ob ausgeben oder in Datei schreiben und eine Kopie der Eingabe
void echo(char *ptr, int schreiben, char command2[SIZE]){
	
	//Zum Ausgeben in die Konsole
	//Einfach strtok und ausgeben
	if (schreiben == 0) {
		while (ptr != NULL) {
			ptr = strtok(NULL, " ");
			if (ptr != NULL){
				printf("%s", ptr);
				printf(" ");
			}
		}
		printf("\n");
	}
	//Zum Schreiben in eine Datei
	else  {
		int i = 5, x = 0;
		char text[SIZE], datei[SIZE];
		
		//In den String text wird der Text gegeben, der dann in die Datei geschrieben werden soll
		while (1) {
			//Falls man beim > angekommen ist, wird die Schleife unterbrochen
			if (command2[i] == '>') {
				//Der String wird noch mit \0 terminiert
				text[x] = '\0';
				break;
			}
			text[x] = command2[i];
			i++;
			x++;
		}
		x=0;
		i += schreiben;
		//Das selbe wird gemacht, um den Damen der Datei herauszufinden
		while (1) {
			if (command2[i] == '\0') {
				datei[x] = '\0';
				break;
			}
			datei[x] = command2[i];
			i++;
			x++;
		}
		FILE* dateizeiger;
		if (schreiben == 2)
			dateizeiger = fopen(datei, "w");
		else 
			dateizeiger = fopen(datei, "a");
		if (dateizeiger == NULL) {
				printf("Datei konnte nicht geoeffnet werden\n");
				return;
		}
		
		fprintf(dateizeiger, text);
		fclose(dateizeiger);
	}
}
void clear() {
	#ifdef __linux__
		printf("\033c"); //Cleart screen in Linux
	#else
		
		//VON MICROSOFT SEITE
		HANDLE                     hStdOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD                      count;
		DWORD                      cellCount;
		COORD                      homeCoords = { 0, 0 };

		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hStdOut == INVALID_HANDLE_VALUE) return;

		/* Get the number of cells in the current buffer */
		if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
		cellCount = csbi.dwSize.X * csbi.dwSize.Y;

		/* Fill the entire buffer with spaces */
		if (!FillConsoleOutputCharacter(
			hStdOut,
			(TCHAR)' ',
			cellCount,
			homeCoords,
			&count
		)) return;

		/* Fill the entire buffer with the current colors and attributes */
		if (!FillConsoleOutputAttribute(
			hStdOut,
			csbi.wAttributes,
			cellCount,
			homeCoords,
			&count
		)) return;

		/* Move the cursor home */
		SetConsoleCursorPosition(hStdOut, homeCoords);
	#endif
}
void touch(char *ptr) {

	char name[SIZE] = "";
	int i = 0;
	while (ptr != NULL) {
			ptr = strtok(NULL, " ");
			if (ptr != NULL) {
				if (i != 0)
					strcat(name, " ");
				i++;
				
				strcat(name, ptr);
			}
	}
	FILE *dateizeiger;
	dateizeiger = fopen(name, "w");
	fclose(dateizeiger);
}




void cat(char *ptr, int schreiben) {
	
	
	int abbruch = 0;
	while ((ptr != NULL) && (abbruch == 0)) {
	
		if (schreiben == 0) {
			ptr = strtok(NULL, " ");
			FILE *fp = fopen(ptr, "r");
			int c;
			while ((c = getc(fp)) != EOF) 
				printf("%c", c);
			printf("\n");
			fclose(fp);
		}
		else {
			char text[100000];
			int n = 0;
			
			while ((strcmp(ptr, ">") != 0) && (abbruch == 0)) {
				ptr = strtok(NULL, " ");
				if (strcmp(ptr, ">") == 0)
					abbruch = 1;
				FILE *fp = fopen(ptr, "r");
				int c;
				while ((c = getc(fp)) != EOF) {
					text[n] = c;
					n++;
				}
				fclose(fp);
			}
			text[n] = '\0';
			ptr = strtok(NULL, " ");
		
			FILE *fp2 = fopen(ptr, "w");
			
			for (int i = 0; text[i] != '\0'; i++) {
				fputc(text[i], fp2);
			}
			fclose(fp2);
		}
	}
}






void pwd() {
	char pfad[SIZE];
	GETDIR(pfad, 300);
	printf("%s\n", pfad);
}



void cd(char *ptr) {
	char pfad[SIZE];
	ptr = strtok(NULL, " ");
	//printf("----> %s\n", ptr);
	chdir(ptr);
	GETDIR(pfad, 300);
	if ((strstr(pfad, ptr) == 0) && (strcmp(ptr, "..")) != 0)
		printf("Verzeichnis nicht gefunden\n");
}




void rm(char *ptr) {
		ptr = strtok(NULL, " ");
		if (remove(ptr) != 0)
			printf("Datei nicht gefunden\n");
}




void ordni(char *ptr) {
	#ifdef __linux__
		char name[SIZE] = "";
		while (ptr != NULL) {
			ptr = strtok(NULL, " ");
			if (ptr != NULL)
				strcat(name, ptr);
		}
		mkdir(name, 0700);

	#else
	char name[SIZE] = "";
	while (ptr != NULL) {
		ptr = strtok(NULL, " ");
		if (ptr != NULL)
			strcat(name, ptr);
	}
	CreateDirectory(name, NULL);
	#endif
}




int pathexists(char folder[]) {

    DIR* dir = opendir(folder);
    if (dir) //Pfad vorhanden
        return 1;
    else if (ENOENT == errno)  //Pfad nicht vorhanden
        return 0;
    else //andere Fehler
        return 0;
}





long long int findSize(char file_name[])
{
    FILE* fp = fopen(file_name, "r");

    fseek(fp, 0L, SEEK_END);

    // calculating the size of the file 
    long long int size = ftell(fp);

    if (size > 1000)
        size /= 1000;
    else if (size > 1)
        size = 1;
    else
        size = 0;

    fclose(fp);

    return size;
}




void ls(char *ptr, int list) {
	
	int i = 0;
	struct dirent *de;
	DIR *dr = opendir(".");
	
	if (list == 1) {
	
			printf("%-33s", "Name");
			printf("%-13s", "Size in KB");
			
			#ifdef __unix__
				printf("%-20s", "Last Modified");
				printf("Permissions\n");
				while ((de = readdir(dr)) != NULL) {
					if (i >= 2) {
						struct stat stvariable;
						stat(de->d_name, &stvariable);
						printf("%-33s", de->d_name); //Name Datei
						
						long int size = stvariable.st_size;
						if (size > 1000)
							size /= 1000;
						else if (size > 1)
							size = 1;
						else
							size = 0;

						printf("%-13ld", size); //Größe Datei
						printf("%-20.10s", ctime(&stvariable.st_atime)); //Zuletzt geändert Datei
						printf((S_ISDIR(stvariable.st_mode)) ? "d" : "-"); //Art Ordner/file
						printf((stvariable.st_mode & S_IRUSR) ? "r" : "-"); //Rechte...
						printf((stvariable.st_mode & S_IWUSR) ? "w" : "-");
						printf((stvariable.st_mode & S_IXUSR) ? "x" : "-");
						printf((stvariable.st_mode & S_IRGRP) ? "r" : "-");
						printf((stvariable.st_mode & S_IWGRP) ? "w" : "-");
						printf((stvariable.st_mode & S_IXGRP) ? "x" : "-");
						printf((stvariable.st_mode & S_IROTH) ? "r" : "-");
						printf((stvariable.st_mode & S_IWOTH) ? "w" : "-");
						printf((stvariable.st_mode & S_IXOTH) ? "x\n" : "-\n");
					}
					else
						i++;
				}
			#else
				printf("Type\n");
				while ((de = readdir(dr)) != NULL) {
					if (i >= 2) {
						printf("%-33s", de->d_name);  //Name Datei
						if (!pathexists(de->d_name)) {
							long long int size = findSize(de->d_name);
							printf("%-13lld", size);
						}
						else 
							printf("%-13s", "-");

						pathexists(de->d_name) ? printf("Folder\n") : printf("File\n");
					}
					else
						i++;
			}
			#endif
	}

	if (dr == NULL) {
        printf("Ordner nicht gefunden.\n" );
        return;
    }
    while ((de = readdir(dr)) != NULL)
            printf("%s\n", de->d_name);

    closedir(dr);
}




void more(char *ptr) {

	FILE *dateizeiger;
	int i, temp;
	ptr = strtok(NULL, " ");
	dateizeiger = fopen(ptr, "r");

	if(dateizeiger == NULL) {
	printf("Datei konnte nicht geoeffnet werden.\n");
	}
	else {
	// komplette Datei zeichenweise ausgeben
		while((temp = fgetc(dateizeiger))!=EOF) {
			printf("%c", temp);
		}
		fclose(dateizeiger);
	}	
	printf("\n");
}
void less(char *ptr) {
		FILE* dateizeiger;
		int i = 2, temp;
		ptr = strtok(NULL, " ");
		dateizeiger = fopen(ptr, "r");
		
	if(dateizeiger == NULL) {
	printf("Datei konnte nicht geoeffnet werden.\n");
	}
	else {
	// komplette Datei zeichenweise ausgeben
		clear();
		printf("||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
		printf("1)  ");
		while((temp = fgetc(dateizeiger))!=EOF) {
			if (temp == '\n') {
					printf("\n");
					printf("%d)  ", i);
					i++;
			}
			if (temp != '\n')
				printf("%c", temp);
		}
		fclose(dateizeiger);
		printf("\n||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
	}	
	while (1) {
			char abbruch;
			scanf(" %c", &abbruch);
			if (abbruch == 'q') {
				clear();
				return;
			}
	}
	getchar();
}

void wc(char* ptr) {
	
	ptr = strtok(NULL, " ");
	
	if (strcmp(ptr, "-l") == 0) {
		FILE* dateizeiger;
		int temp, anzahl = 0;
		ptr = strtok(NULL, " ");
		dateizeiger = fopen(ptr, "r");
		if (dateizeiger == NULL) {
			printf("Datei konnte nicht geoeffnet werden\n");
			return;
		}
		while ((temp = fgetc(dateizeiger)) != EOF) {
			
			if (temp == '\n')
				anzahl++;
		}
		fclose(dateizeiger);
		printf("%d\n", anzahl + 1);	
	}
	else if (strcmp(ptr, "-w") == 0) {	
		FILE* dateizeiger;
		int temp, anzahl = 0;
		ptr = strtok(NULL, " ");
		dateizeiger = fopen(ptr, "r");
		while ((temp = fgetc(dateizeiger)) != EOF) {
			
			if (temp == ' ')
				anzahl++;
		}
		fclose(dateizeiger);
		printf("%d\n", anzahl + 1);
	}
	else if (strcmp(ptr, "-c") == 0){
		
		FILE* dateizeiger;
		int temp, anzahl = 0;
		ptr = strtok(NULL, " ");
		dateizeiger = fopen(ptr, "r");
		while ((temp = fgetc(dateizeiger)) != EOF) {
			
			anzahl++;
		}
		fclose(dateizeiger);
		printf("%d\n", anzahl);
		
	}
	else {
		printf("Nur die Parameter -l -w -c sind erlaubt!\n");
		printf("Example: wc -l test.txt\n");
		
	}
}

void whoami() {
		#ifdef __linux__
			char name[SIZE];
			getlogin_r(name, 300);
			printf("%s\n", name);
		#else
			printf("%s\n", getenv("USERNAME"));
		#endif
}

void rechte(char* ptr) {
	
		#ifdef __linux__
		char mode[4], datei[SIZE];
		ptr = strtok(NULL, " ");
		strcpy(mode, ptr);
		ptr = strtok(NULL, " ");
		strcpy(datei, ptr);
		int rechte = strtol(mode, 0, 8);
		if (chmod(datei, rechte) < 0)
			printf("Ein Fehler ist aufgetreten\n");
		#else
			printf("Dieser Befehl ist nur in Linux Systemen möglich\n");
		#endif
	
	
}

void nano(char* ptr) {
	
	#ifdef __linux__
		printf("geat no net");
	#else
		if ((ptr = strtok(NULL, " ")) == NULL) {
			printf("Es muss eine Datei uebergeben werden\n");
			return;
		}
		char *array;
		int temp, i = 0;
		FILE* dateizeiger;
		dateizeiger = fopen(ptr, "r");
		array = malloc(sizeof(char));
		while((temp = fgetc(dateizeiger))!=EOF) {
			if ((array = realloc(array, sizeof(char))) == NULL) {
				printf("Es ist eine Fehler bei der Speicherreservierung aufgetreten\n");
				return;
			}
			printf("xd");
			array[i] = temp;
			i++;
		}
		array = realloc(array, sizeof(char));
		array[i] = '\0';
		clear();
		printf("\t GNU NANO\n\n");
		for (int x = 0; array[x] != '\0'; x++) {
			printf("%c", array[x]);
		}
		char input;
		while (array[i-1] != '|') {
			if ((array = realloc(array, sizeof(char))) == NULL) {
				printf("Es ist ein Fehler bei der Speicherreservierung aufgetreten\n");
				return;
			}
		
			fflush(stdin);
			input = getch();
			if (input == '\b') {
					i--;
					array[i] = '\0';
			}
			else {
				array[i] = input;
				i++;
			}
			if ((array = realloc(array, sizeof(char))) == NULL) {
					printf("Es ist ein Fehler bei der Speicherreservierung aufgetreten\n");
					return;
			}
			array[i] = '\0';
			clear();
			printf("\t GNU NANO\n\n");
			for (int x = 0; array[x] != '\0'; x++) {
				if (array[x] == '\r') 
					printf("\n");
				else
					printf("%c", array[x]);	
			}
		}
		array[i-1] = '\0';
		printf("\n");
		fclose(dateizeiger);
		dateizeiger = fopen(ptr, "w");
		fprintf(dateizeiger, array);
		fclose(dateizeiger);
		free(array);
	#endif	
}
int color(char *ptr) {
	
		#ifdef __linux__ 
		#else
			ptr = strtok(NULL, "ptr");
			return atoi(ptr);
		#endif
}


void rmtree(const char path[])
{
    size_t path_len;
    char *full_path;
    DIR *dir;
    struct stat stat_path, stat_entry;
    struct dirent *entry;

    // stat for the path
    stat(path, &stat_path);

    // if path does not exists or is not dir - exit with status -1
    if (S_ISDIR(stat_path.st_mode) == 0) {
        fprintf(stderr, "%s: %s\n", "Is not directory", path);
        exit(-1);
    }

    // if not possible to read the directory for this user
    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "%s: %s\n", "Can`t open directory", path);
        exit(-1);
    }

    // the length of the path
    path_len = strlen(path);

    // iteration through entries in the directory
    while ((entry = readdir(dir)) != NULL) {

        // skip entries "." and ".."
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        // determinate a full path of an entry
        full_path = calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, entry->d_name);

        // stat for the entry
        stat(full_path, &stat_entry);

        // recursively remove a nested directory
        if (S_ISDIR(stat_entry.st_mode) != 0) {
            rmtree(full_path);
            continue;
        }

        // remove a file object
        if (unlink(full_path) != 0)
            printf("Can`t remove a file: %s\n", full_path);
        free(full_path);
    }

    // remove the devastated directory and close the object of it
    if (rmdir(path) != 0)
        printf("Can`t remove a directory: %s\n", path);
    closedir(dir);
}

void cp(char *ptr, int del) {
		
		FILE *lesen;
		FILE *schreiben;
		ptr = strtok(NULL, " ");
		char ldatei[50] = "";
		strcpy(ldatei, ptr);
		char aktuellerpfad[SIZE];
		GETDIR(aktuellerpfad, SIZE);
		
		int anzahl = 0, i;
		ptr = strtok(NULL, " ");
		for (i = 0; ptr[i] != '\0'; i++) {
			if (ptr[i] == '\\')
				anzahl++;
		}

		if (ptr[i-1] == '\\')
			anzahl--;
		char pfad[SIZE];
		i = 0;
		int x = 0;
		while (1) {
			if (x == anzahl)
				break;
			if (ptr[i] == '\\') 
				x++;
			pfad[i] = ptr[i];
			i++;
		}
		pfad[i] = '\0';
		char datei[50];
		for (int x = 0; ptr[i] != '\0'; x++) {
				datei[x] = ptr[i];
				i++;
		}
		
		chdir(pfad);
		char aktuellerpfad2[SIZE];
		strcpy(aktuellerpfad2, aktuellerpfad);
		strcat(aktuellerpfad2, "\\");
		strcat(aktuellerpfad2, ldatei);
		if ((lesen = fopen(aktuellerpfad2, "r")) == NULL) {
			printf("Die Datei konnte nicht gefunden werden\n");
			chdir(aktuellerpfad);
			return;
		}
		schreiben = fopen(datei, "w");
		int temp;
		while ((temp = fgetc(lesen)) != EOF) {
			fputc(temp, schreiben);
		}
		fclose(lesen);
		fclose(schreiben);
		chdir(aktuellerpfad);
		if (del)
			remove(ldatei);
	
}

void date() {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("Jetzt gerade: %02d-%02d-%d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);	
}




void help() {
	
		printf("--------------------------------------------------------------------------------------------\n\n");
		printf("help             Zeigt diese Liste aller Befehle\n\n");
		printf("echo             Gibt Text in Konsole oder Datei aus\n");
		printf("                 \"echo hallo\"   ||    \"echo hallo > datei.txt\"\n\n");
		printf("date || time     Zeigt das aktuelle Datum und Uhrzeit\n\n");
		printf("color            Zum Ver%cndern der Farbe in der Konsole\n", 132);
		printf("                 \"color 3\"\n");
		printf("                 1 = blau; 2 = gr%cn; 3 = cyant; 4 = rot; 5 = magenta; 6 = gelb; 7 = weiss...\n\n", 129);
		printf("cp               Kopiert eine Datei in ein anderes Verzeichnis\n");
		printf("                 \"cp datei.txt C:\\pfad\\kopie.txt\"\n\n");
		printf("mv               Verschiebt eine Datei in ein anderes Verzeichnis, kann aber auch zum Umbennenen verwendet werden\n");
		printf("                 \"mv datei.txt C:\\pfad\\datei.txt\"\n\n");
		printf("pwd              Zeigt das aktuelle Verzeichnis\n\n");
		printf("nano             Editor um in Dateien zu schreiben. Wird mit | beendet.\n");
		printf("                 Es muss eine bereits existierende oder der Name einer neuen Datei %cbergeben werden\n", 129);
		printf("                 \"nano beispiel.txt\"\n\n");
		printf("cd               Wechselt das Verzeichnis\n\n");
		printf("clear            Cleart die Konsole\n\n");
		printf("history          Zeigt die letzten eingegebenen Befehle. Kann mit -c gel%cscht werden\n\n", 148);
		printf("cat              Gibt Inhalt einer Datei aus oder f%chrt zwei Dateien zusammen\n", 129);
		printf("                 Mit > kann der Inhalt in eine Datei exportiert werden\n\n");
		printf("touch            Erstellt eine Datei\n\n");
		printf("rm               L%cscht eine Datei\n\n", 148);
		printf("ls               Zeigt alle Dateien im Verzeichnis\n");
		printf("ls -l            Zeigt alle Dateien im Verzeichnis mit Rechten\n\n");
		printf("chmod(UNIX only) Ver%cndert die Rechte einer Datei oder eines Ordners\n\n", 132);
		printf("less             Zeigt den Inhalt einer Datei, wird mit q beendet\n\n");
		printf("more             Zeigt den Inhalt einer Datei\n\n");
		printf("mkdir            Erstellt einen Ordner\n\n");
		printf("rmdir            L%cscht einen Ordner\n\n", 148);
		printf("os               Gibt das aktuelle Betriebssystem aus\n\n");
		printf("wc               Gibt die Zeilen, W%crter oder Buchstaben einer Datei aus\n", 148);
		printf("                 \"wc -l datei.txt\"\n\n");
		printf("whoami           Zeigt den aktuellen Benutzer\n\n");
		printf("exit             Beendet die Shell\n\n");
		printf("--------------------------------------------------------------------------------------------\n\n");
}



