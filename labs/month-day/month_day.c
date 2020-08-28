#include <stdio.h>
#include <stdlib.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){
	
}

char *month_name(int n){
	static char *name[] = {
		"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
	};
	printf("%s ",name[n]);
	return (name[n]);
}

int day_of_year(int *day,int year){
	int daytab[2][13] = {{0,31,28,31,30,31,30,31,31,30,31,30,31},{0,31,29,31,30,31,30,31,31,30,31,30,31}};
	int i, leap, month_day;
	int tempDay = *day;
	char *month;
	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
	if((leap == 1 && *day > 366) || (leap == 0 && *day > 365)){
		printf("Error fecha no valida\n");
		return 0;
	}
	for (i = 1; i <13; i++){
		if(tempDay - daytab[leap][i] <= 0){
			month = month_name(i-1);
			month_day = tempDay;
			break;
			
		}else{
			tempDay -= daytab[leap][i];
		}
	}
	printf("%d, %d\n",month_day,year);
	return tempDay;
}


int main(int argc, char* argv[]) {

    if(argc == 3){
	int tempDay = atoi(argv[2]);
	int *day = &tempDay;
	*day = tempDay;
	int year =  atoi(argv[1]);
	day_of_year(day,year);
    }
    return 0;
}
