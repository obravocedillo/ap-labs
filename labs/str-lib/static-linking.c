#include <stdio.h>
#include <string.h>

int mystrlen(char *);
char *mystradd(char *,char *);
char mystrfind(char *,char *);
int main(int argc, char* argv[]){
	if(argc == 4){
		if(strcmp(argv[1], "-add") == 0){
			char *result = mystradd(argv[2],argv[3]);
			printf("Initial Length: %d\n", mystrlen(argv[2]));
			printf("New String: %s\n", result);
			printf("New length: %d\n", mystrlen(result));
			printf("\n");
		}else if(strcmp(argv[1], "-find") == 0){
			int result = mystrfind(argv[2],argv[3]);
			if(result == -1){
				printf("String was not found");
				printf("\n");
			}else{
				printf("The string was found in position %d", result);
				printf("\n");
			}
			printf("\n");
		}
    		return 0;
	}else{
		printf("Error, missing parameters\n");
		printf("\n");
	}
}
