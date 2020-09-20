#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define MAXCHAR 1000

void merge(int array[], int start, int middle, int last) {
    int firstHalf = middle - start + 1;
    int secondHalf = last - middle;

    int arrOne[firstHalf];
    int arrTwo[secondHalf];
   
    for (int i = 0; i < firstHalf; i++){
        arrOne[i] = array[start + i];
    }
    
    for (int j = 0; j < secondHalf; j++){
        arrTwo[j] = array[middle + 1 + j];
    }


    int counterOne = 0;
    int counterTwo = 0;
    int counterThree = start;

    while (counterOne < firstHalf && counterTwo < secondHalf) {
        if (arrOne[counterOne] <= arrTwo[counterTwo]) {
            array[counterThree] = arrOne[counterOne];
            counterOne++;
        } else {
            array[counterThree] = arrTwo[counterTwo];
            counterTwo++;
        }
        counterThree++;
    }

    while (counterOne < firstHalf) {
        array[counterThree] = arrOne[counterOne];
        counterOne++;
        counterThree++;
    }

    while (counterTwo < secondHalf) {
        array[counterThree] = arrTwo[counterTwo];
        counterTwo++;
        counterThree++;
    }
}

void mergeInt(int arr[], int first, int last){
	if (first < last){
		int mid = (first + last) / 2;
		mergeInt(arr, first, mid);
		mergeInt(arr, mid + 1, last);
		merge(arr, first, mid, last);
	}
}

int main(int argc, char *argv[]){
    FILE *fp;
    char str[MAXCHAR];
    if(argc > 1){
        if(strcmp(argv[1], "-n") == 0){
            if(argc > 2){
                int counterLines = 0;
                int numbers[MAXCHAR];
                fp = fopen(argv[2], "r");
                while (fgets(str, MAXCHAR, fp)!=NULL){
                    numbers[counterLines] = atoi(str);
                    counterLines ++;
                }
                mergeInt(numbers,0,counterLines-1);
                for(int i=0; i<counterLines; i++){
                    printf("%d\n",numbers[i]);
                }
                fclose(fp);
            }else{
                printf("Error incomplete arguments\n");
                printf("\n");
            }
        }else{
            fp = fopen(argv[1], "r");
            int counterLines = 0;
            int numbers[MAXCHAR];
            char strings[MAXCHAR][MAXCHAR];
            while (fgets(str, MAXCHAR, fp)!=NULL){
                int tempSum = 0;
                for(int i=0; i<strlen(str);i++){
                    tempSum += str[i];
                }
                strcpy(strings[counterLines], str);
                numbers[counterLines] = tempSum;
                counterLines ++;
            }
            //for string we will sum all characters, sort them as ints and then return them to char
            mergeInt(numbers,0,counterLines-1);
            for(int i=0; i<counterLines; i++){
                for(int j=0; j<counterLines; j++){
                    int tempSum = 0;
                    for(int k=0; k<strlen(strings[j]);k++){
                        tempSum += strings[j][k];
                    }
                    if(numbers[i] == tempSum){
                        printf("%s",strings[j]);
                    }
                }
            }
            fclose(fp);
        }
    }else{
        printf("Error missing arguments\n");
        printf("\n");
    }
    return 0;
}
