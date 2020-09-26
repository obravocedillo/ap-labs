
#include <stdio.h>

int infof(char *, ...);
int warnf(char *, ...);
int errorf(char *, ...);
int panicf(char *, ...);

int main() {
    infof("Info %d\n",5);
    warnf("Warning\n");
    errorf("Error\n");
    panicf("Panic\n");
    return 0;
}
