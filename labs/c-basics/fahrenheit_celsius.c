#include <stdio.h>
#include <stdlib.h>

float calculateCelsius(int number){
	float result = ((number-32)*5)/(float)9;
	return result;
}

int main(int argc, char* argv[]){
	if(argc == 1){
		printf("Ingrese una cantidad valida");
	}else if(argc == 2){
		int fahrenheit = atoi(argv[1]);
		printf("Fahrenheit: %s, Celsius: %.1f",argv[1],calculateCelsius(fahrenheit));
	}else if(argc == 4){
		int start = atoi(argv[1]);
		int end = atoi(argv[2]);
		int increment = atoi(argv[3]);
		for(int i=start; i<=end; i += increment){
			printf("Fahrenheit: %d, Celcius: %.1f",i,calculateCelsius(i));
			printf("\n");
		}

	}
	printf("\n");
	return 0;
}
