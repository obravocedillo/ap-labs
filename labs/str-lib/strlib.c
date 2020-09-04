#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int mystrlen(char *str){
    int i;
    for(i=0; str[i]!='\0'; i++);
    return i;
}

char *mystradd(char *origin, char *addition){
    char *result = malloc(mystrlen(origin) + mystrlen(addition) + 1);
    strcpy(result, origin);
    strcat(result, addition);
    return result; 
}

int mystrfind(char *origin, char *substr){
    int i;
    int j;
    int originl = mystrlen(origin);
    int substrl = mystrlen(substr);
    int difference = originl-substrl;
    for(i=0; i<=difference; i++){
	    for(j = 0; j<substrl;j++){
		if(origin[i + j] != substr[j]){
			break;
		}
	    }
	    if(j == substrl){
		return i;
	    }
    }
    return i;
}
