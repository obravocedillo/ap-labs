#include <stdio.h>
#include <string.h>
#define MAX_SIZE 100

void reverse(char arr[MAX_SIZE],int index){
	int i;
	int j;
	char tmp;
	for (i = 0; i<index/2; i++){
		tmp = arr[i];
		arr[i] = arr[index - i - 1];
		arr[index - i - 1] = tmp;
	}
	for(j = 0; j<index; j++){
		printf("%c",arr[j]);
	}
	printf("\n");
}

int main(){
    int c;
    char word[MAX_SIZE];
    int currentIndex = 0;
    while((c = getchar()) != EOF){
	    if(c == '\n'){
		reverse(word,currentIndex);
		currentIndex = 0;
		memset(word, 0, sizeof(word));
	    }else{
		word[currentIndex] = c;
		currentIndex ++;
	    }
    }
    return 0;
}
