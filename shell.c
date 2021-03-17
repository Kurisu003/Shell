#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "commands.h"

#ifdef __linux__
	#define BETRIEBSSYSTEM "Linux"
	
	#define RED   "\x1B[31m"
	#define GRN   "\x1B[32m"
	#define YEL   "\x1B[33m"
	#define BLU   "\x1B[34m"
	#define MAG   "\x1B[35m"
	#define CYN   "\x1B[36m"
	#define WHT   "\x1B[37m"
	#define RESET "\x1B[0m"	
#else
	#include <windows.h>
	#define BETRIEBSSYSTEM "Windows"
#endif
int main() {
	
		int hicounter = 0;
		#ifdef __linux__
			char back[15] = "\x1B[0m"; 
		#else
			int farbe = 7;
			char eingabe[200];
		#endif
		while (1) {
			char command[300], trennung[] = " ", history[300][1000], command2[300];
			char *ptr, *str;
			char pfad[200];
			GETDIR(pfad, 200);
			
			//Farbe
			#ifdef __linux__
				printf(RESET);
				printf("%s", back);
			#else
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), farbe);
			#endif
			
			printf("%s ~$:", pfad);
			#ifdef __linux__
				fgets(command, 300, stdin);
			#else
				char befehle[24][20] = {"echo", "clear", "exit", "touch", "rm", "cat", "ls", "color", "help", "mv", "cp", "os", "wc", "mkdir", "rmdir", "chmod", "more", "less", "pwd", "cd", "whoami", "nano", "color", "date"};
				int i, true = 0;
				char c, x;
				
				i = 0;
				while ((c = getch()) != '\r'){
					
					printf("\r                                                                                                          ");
					if (c == '\t') {
							
						struct dirent *de;
						DIR *dr = opendir(".");
						if (dr == NULL) {
							printf("Ordner nicht gefunden.\n" );
						
						}
						while ((de = readdir(dr)) != NULL) {
								char vergleich[200];
								if (strstr(eingabe, " ") != NULL) {
									int u;
									for (u = 0; eingabe[u] != ' '; u++);
									u++;
									int p = 0;
									for (p = 0; eingabe[u] != '\0'; u++){
											vergleich[p] = eingabe[u];
											p++;
									}
									vergleich[p] = '\0';
									
								}
								else
									strcpy(vergleich, eingabe);
								for (int n = 0; n < strlen(vergleich); n++) {
									
									if (vergleich[n] != de->d_name[n]) {
										true = 0;
										break;
									}
									else
										true = 1;
								}
								
								if (true == 1) {
								
										for (int j = 0; vergleich[j] = '\0'; j++)
											eingabe[j] = '\0';
										strcpy(vergleich, de->d_name);
										
								
								
										for (int n = strlen(eingabe); eingabe[n] != ' '; n--)
											eingabe[n] = '\0';
										strcat(eingabe, vergleich);
										i = strlen(eingabe);
										break;
								}
							
						}
						closedir(dr);	
				
						
				
						for (x = 0; x < 24; x++) {
							for (int z = 0; z < i ; z++) {
								if (eingabe[z] != befehle[x][z]) {
									true = 0;
									break;
								}
								else 
									true = 1;
							}
							if (true == 1) {
								for (int q = 0; eingabe[q] = '\0'; q++) 
									eingabe[q] = '\0';
								strcpy(eingabe, befehle[x]);
								i = strlen(eingabe);
								break;
							}
						}
				
				
					}
					else if (c == '\b') {
						if (i != 0) {
							i--;
							eingabe[i] = '\0';
						}
					}
					else {
						eingabe[i] = c;
						i++;
						eingabe[i] = '\0';
					}
					printf("\r%s ~$:", pfad);
					printf("%s", eingabe);
				}
				printf("\n");
				strcpy(command, eingabe);
				
				for (int x = 0; x < 200; x++) {
					eingabe[x] = '\0';
				}
				
				
			#endif
		
			strcpy(command2, command);
		
			#ifdef __linux__
				chomp(command2);
			#endif
			str = &command[0];
			#ifdef __linux__
				chomp(str);
			#endif

			strcpy(history[hicounter], command);
			hicounter++;

			// String wird aufgeteilt falls er aus mehreren Wörtern besteht
			if (strpbrk(command, " ") != 0){
				ptr = strtok(command, trennung);
			}
			else{
				//strcpy(*ptr, command);
				ptr = &command[0];
			}
			
			// Alle Commands
			
			if (strcmp(ptr, "exit") == 0) 
				return 0;
			else if (strcmp(ptr, "color") == 0) {
				#ifdef __linux__
						ptr = strtok(NULL, " ");
						int farbe = atoi(ptr);
						switch (farbe) {
							case 1: strcpy(back, BLU); break;
							case 2: strcpy(back, GRN); break;
							case 3: strcpy(back, CYN); break;
							case 4: strcpy(back, RED); break;
							case 5: strcpy(back, MAG); break;
							case 6: strcpy(back, YEL); break;
							case 7: strcpy(back, RESET); break;
						}
						printf("%s", back);
				#else
					farbe = color(ptr);
				#endif
			}
			else if (strcmp(ptr, "os") == 0) {
				printf(BETRIEBSSYSTEM);
				printf("\n");
			}
			else if (strcmp(ptr, "help") == 0)
				help();
			else if (strcmp(ptr, "echo") == 0) {
				int schreiben = 0;
				if (strstr(command2, ">>") != NULL)
					schreiben = 3;
				else if (strstr(command2, ">") != NULL)
					schreiben = 2;
				echo(ptr, schreiben, command2);
			}
			else if (strcmp(ptr, "clear") == 0)
				clear();
			else if (strcmp(ptr, "touch") == 0)
				touch(ptr);
			else if (strcmp(ptr, "cat") == 0) {
				if (strcmp(ptr, "cat") == 0) {
					int schreiben = 0;
						for (int i = 0; command2[i] != '\0'; i++) {
					
							if (command2[i] == '>') {
								schreiben = 1;
								break;
							}
							else 
								schreiben = 0;
						}
	
						cat(ptr, schreiben);
				}
				
			
			}
			else if (strcmp(ptr, "pwd") == 0)
				pwd();
			else if (strcmp(ptr, "cd") == 0)
				cd(ptr);
			else if (strcmp(ptr, "rm") == 0)
				rm(ptr);
			else if (strcmp(ptr, "cp") == 0)
				cp(ptr, 0);
			else if (strcmp(ptr, "mv") == 0)
				cp(ptr, 1);
			else if (strcmp(ptr, "wc") == 0)
				wc(ptr);
			else if (strcmp(ptr, "mkdir") == 0)
				ordni(ptr);
			else if (strcmp(ptr, "rmdir") == 0){
				if ((ptr = strtok(NULL, " ")) == NULL)
					printf("Ordner eingeben!\n");
				else
					rmtree(ptr);
			}
			else if (strcmp(ptr, "less") == 0)
				less(ptr);
			else if (strcmp(ptr, "whoami") == 0)
				whoami();
			else if ((strcmp(ptr, "ls") == 0) || (strcmp(ptr, "dir") == 0)) {
				int list;
				if ((command2[3] == '-') && (command[4] == 'l')) {
					list = 1;
				}
				else
					list = 0;
				ls(ptr, list);
			}
			else if (strcmp(ptr, "chmod") == 0)
				rechte(ptr);
			else if (strcmp(ptr, "more") == 0)
				more(ptr);
			else if (strcmp(ptr, "nano") == 0)
				nano(ptr);
			else if ((strcmp(ptr, "date") == 0) || (strcmp(ptr, "time") == 0))
				date();
			else if (strcmp(ptr, "history") == 0) { 
				if (strstr(command2, "-c") != NULL) {
					ptr = strtok(NULL, " ");	
				}
				if (strcmp(ptr, "-c") == 0) {
					for (int i = 0; i < hicounter; i++) {
						for (int x = 0; x < 100; x++) {
								history[i][x] = '\0';
						}
					}
					hicounter = 0;
				}	
				else {
					for (int i = 0; i < hicounter; i++) {
								printf("%s\n", history[i]);
					}
				}
			}
			
			// Muss zum Schluss stehen
			else if (strcmp(ptr, " ") == 1)
				printf("Unknown command\n");
			
			
		}
}
