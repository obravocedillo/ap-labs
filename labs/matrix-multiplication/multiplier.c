#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "logger.h"
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>


//Number of buffers, received form command line
int NUM_BUFFERS;
pthread_mutex_t *mutexes;
//result to be written in file
long *result;
//Threads used to calculate multiplication of matrix
pthread_t threads[2000];
long **buffers;

void *threadFunc(void *arg);

//Matric used to help in the multiplication of matrix
struct MatMul {
  long *A;
  long *B;
  size_t row;
  size_t col;
  long *result;
};

int openFile(char* filename){
    int fd = open(filename, O_RDONLY);
    return fd;
}

off_t getSize(int fd){
  //CurrentOffSet
    off_t currentOffSet = lseek(fd, (size_t)0, SEEK_CUR);
    off_t sizeOfFile = lseek(fd, (size_t)0, SEEK_END); 
    lseek(fd, currentOffSet, SEEK_SET);
    return sizeOfFile;
}

long *readMatrix(char *filename) {
  //Open File
  int fd = openFile(filename);
  //File Error
  if(fd == -1){
        infof("Invalid file, cant be opened\n");
        exit(-1);
  }
  off_t sizeOfFile = getSize(fd);
  long* matrix = (long*)malloc(sizeof(long)*sizeOfFile);
  read(fd,matrix,sizeOfFile);
  close(fd);
  return matrix;
}

long *getColumn(int col, long *matrix) {
  size_t currentOffSet = col - 1;
  long *mycol;
  //column is 2000 long
  mycol = (long *)malloc(2000 * sizeof(long));

  for (int i = 0; i < 2000; i++) {
    mycol[i] = matrix[currentOffSet];
    currentOffSet += 2000;
  }
  return mycol;
}

long *getRow(int row, long *matrix) {
  size_t currentOffSet = ((2 * row) - 2) * 1000;
  long *myrow;
  //column is 2000 long
  myrow = (long *)malloc(2000 * sizeof(long));

  for (int i = 0; i < 2000; i++) {
    myrow[i] = matrix[currentOffSet++];
  }
  return myrow;
}

int getLock(void) {
  //Gets current lock
  for (int i = 0; i < NUM_BUFFERS; i++) {
    if (pthread_mutex_trylock(&mutexes[i]) == 0)
      return i;
  }
  return -1;
}

int releaseLock(int lock) {
  //Releases lock
  if (pthread_mutex_unlock(&mutexes[lock]) == 0) {
    return 0;
  }
  return -1;
}

long dotProduct(long *vec1, long *vec2) {
  long result = 0;
  for (int i = 0; i < 2000; i++)
    result += vec1[i] * vec2[i];
  return result;
}

long *multiply(long *matA, long *matB) {
  infof("Calculating multiplication, columns and rows were set to 200 in order to allow the program to finish");
  result = (long *)malloc(40000 * sizeof(long));
  for (size_t i = 0; i < 200; i++) {
    for (size_t j = 0; j < 200; j++) {
      struct MatMul *multiplier;
      multiplier =
          (struct MatMul *)
          malloc(sizeof(struct MatMul));
      multiplier->A = matA;
      multiplier->B = matB;
      multiplier->row = i + 1;
      multiplier->col = j + 1;
      multiplier->result = result;
      pthread_create(&threads[j], NULL, threadFunc,
               (void *)multiplier);
    }

    for (size_t j = 0; j < 200; j++)
      pthread_join(threads[j], NULL);
    fflush(stdout);
  }
  infof("\n");
  return result;
}

int saveResultMatrix(long *result, char file[]) {
  FILE *f = fopen(file, "w");
  if (f == NULL) {
    errorf("Couldn't write results\n");
    return -1;
  }

  for (size_t i = 0; i < 40000; i++)
    //fprintf used to save data in file
    fprintf(f, "%ld\n", result[i]);

  fclose(f);
  return 0;
}

void *threadFunc(void *arg) {
  struct MatMul *data = (struct MatMul *)arg;
  long index;

  int lock1, lock2;
  while ((lock1 = getLock()) == -1);
  while ((lock2 = getLock()) == -1);
  buffers[lock1] = getRow(data->row, data->A);
  buffers[lock2] = getColumn(data->col, data->B);

  index = ((((data->row - 1) * 2000) + data->col) - 1);
  data->result[index] = dotProduct(buffers[lock1], buffers[lock2]);

  free(buffers[lock1]);
  free(buffers[lock2]);
  free(arg);
  while (releaseLock(lock1) != 0);
  while (releaseLock(lock2) != 0);
  return NULL;
}


int main(int argc, char **argv) {
  // Place your magic here
  if (argc != 5) {
    errorf("You are missing some parameters\n");
    return 1;
  }

  if(strcmp(argv[1],"-n")){
    errorf("Wrong use of paremeters\n");
    return -1;
  }
  infof("%s \n",argv[2]);
  NUM_BUFFERS = strtol(argv[2], NULL, 10);
  buffers = (long **)malloc(NUM_BUFFERS * sizeof(long *));
  mutexes = (pthread_mutex_t *) malloc(NUM_BUFFERS * sizeof(pthread_mutex_t));

  for (int i = 0; i < NUM_BUFFERS; i++) {
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    mutexes[i] = mutex;
    pthread_mutex_init(&mutexes[i], NULL);
  }

  long *matrixA, *matrixB;
  matrixA = readMatrix("matA.dat");
  matrixB = readMatrix("matB.dat");

  result = multiply(matrixA, matrixB);
  saveResultMatrix(result,argv[4]);
  return 0;
}

