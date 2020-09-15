//Oliver Brandon Bravo Cedillo A01229273
// open(), close(), read() were used
// No get line was used
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define REPORT_FILE "packages_report.txt"
#define PACKAGES_SIZE 1000

//Struct with all the package Information
struct package{
	char name[200];
	char installDate[200];
	char lastUpdate[200];
	int  updates;
	char removalDate[200];
};

//Array of packages
struct package arr_package[PACKAGES_SIZE];

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {	
    if (argc == 5) {
	printf("Usage:./pacman-analizer.o \n");
	analizeLog(argv[2], argv[4]);
    }else{
	printf("Missing some of the parameters\n"); 
      	printf("\n");	
    }
   return 0;
}

void analizeLog(char *logFile, char *report) {
    int fd = open(logFile, O_RDONLY);
    //Size of the buffer we will ue to read file
    int buffSize = 4096;
    char buffer[buffSize];
    char bufferLine[buffSize];
    int bufferLineCounter = 0;
    int line;
    char *install;
    char *upgrade;
    char *remove;
    int installedAll = 0;
    int removedAll = 0;
    int upgradedAll = 0;
    int totalPackages = 0;
	int currentStructIndex = 0;
    if(fd == -1){
	printf("Error opening the logs file\n");
	printf("\n");
	return;
    }
    while(line=read(fd,buffer,buffSize)){
		//Move through the file to find end of line
		for(int i=0; i<sizeof(buffer); i++){
			bufferLine[bufferLineCounter] = buffer[i];
			bufferLineCounter ++;
			if(buffer[i] == '\n'){
				//Start to analize single line to identify package
				install = strstr(bufferLine,"installed");
				upgrade = strstr(bufferLine,"upgraded");
				remove = strstr(bufferLine,"removed");
				int emptySpace = 0;
				int firstEmptyIndex = 0;
				int wordLength = 0;
				int startingIndex = 0;
				//printf("%d\n",bufferLineCounter);
				//printf("%s",bufferLine);
				if(install != NULL){
					startingIndex = install - bufferLine;
					wordLength = 9;
					//printf("%d\n",install - bufferLine);
				}
				if(upgrade != NULL){
					startingIndex = upgrade - bufferLine;
					wordLength = 8;
				}
				if(remove != NULL){
					startingIndex = remove - bufferLine;
					wordLength = 7;
				}
				if(startingIndex != 0){
					for(int j=0; j<bufferLineCounter-startingIndex; j++){
						if(bufferLine[startingIndex+j] == ' '){
							emptySpace += 1;
							if(emptySpace == 2){
								int lengthOfPackage = j - firstEmptyIndex;
								if(lengthOfPackage > 3){
									//We got the package name
									char substr[lengthOfPackage];
									for(int k = 0; k<lengthOfPackage;k++){
										substr[k] = bufferLine[startingIndex+k+wordLength];
									}
									substr[lengthOfPackage] = '\0';
									//printf("%s\n", substr);	
									//We will search for the date now
									for(int j=0; j<bufferLineCounter; j++){
										if(bufferLine[j] == ']'){
											//We got the date
											char date[j];
											for(int k = 0; k<j;k++){
												date[k] = bufferLine[k+1];
											}
											date[j-1] = '\0';
											//printf("%s\n", date);
											//Check the condition and add remove or install the package
											
											if(install != NULL){
												//printf("%d\n",currentStructIndex);
												strcpy(arr_package[currentStructIndex].name, substr);
												strcpy(arr_package[currentStructIndex].installDate, date);
												strcpy(arr_package[currentStructIndex].removalDate, "-");
												strcpy(arr_package[currentStructIndex].lastUpdate, "-");
												arr_package[currentStructIndex].updates = 1;
												installedAll += 1;
												totalPackages += 1;
												currentStructIndex += 1;
											}
											if(upgrade != NULL){
												for(int j = 0; j<installedAll; j++){
													if(strcmp(arr_package[j].name,substr) == 0){
														arr_package[j].updates += 1;
														strcpy(arr_package[j].lastUpdate, date);
													}
												}
												upgradedAll += 1;
												break; 
											}
											if(remove != NULL){
												for(int j = 0; j<installedAll; j++){
													if(strcmp(arr_package[j].name,substr) == 0){
														strcpy(arr_package[j].removalDate, date);
													}
												}
												removedAll += 1;
												totalPackages -= 1;
												break; 
											}
											//write log file with all the results
											FILE *fpWrite;
											fpWrite = fopen(report,"w");
											//Write final report of the packages
											if(fpWrite != NULL){
												fprintf(fpWrite,"Pacman Package Report\n");
												fprintf(fpWrite, "---------------------\n");
												fprintf(fpWrite, "-Installed Packages:  %d\n", installedAll);
												fprintf(fpWrite, "-Removed Packages:    %d\n", removedAll);
												fprintf(fpWrite, "-Upgraded Packages:   %d\n", upgradedAll);
												fprintf(fpWrite, "-Current Installed:   %d\n", totalPackages);
												fprintf(fpWrite,"List Of Packages\n");
												fprintf(fpWrite, "---------------------\n");
												//Move through the array of structs (Packages)
												for(int k = 0; k<currentStructIndex; k++){
													fprintf(fpWrite,"- Package Name: %s\n", arr_package[k].name);
													fprintf(fpWrite," - Install Date: %s\n", arr_package[k].installDate);
													fprintf(fpWrite," - Last Update Date: %s\n", arr_package[k].lastUpdate);
													fprintf(fpWrite," - How Many Updates: %d\n", arr_package[k].updates);
													fprintf(fpWrite," - Removal Date : %s\n", arr_package[k].removalDate);
													fprintf(fpWrite,"\n");
												}
											}
											fclose(fpWrite);
											break; 
										}
									}
								}
								
							}
							firstEmptyIndex = j;
						}
					}
				}
			bufferLineCounter = 0;
			memset(bufferLine,buffSize,buffSize);	
			}
			
		}
    }
    printf("Generating Report from [%s] log file\n", logFile);
    printf("Report is generated at: [%s]\n", report);
    printf("\n");
    int close(int fd);
}
