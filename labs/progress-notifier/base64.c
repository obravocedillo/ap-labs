#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "logger.h"

#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66

static const unsigned char d[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};

//progress of the current process
int progress = 0;
//The number of pads is used to track progress
int currentPads = 0;

//The current value in decode
int currentDecode = 0;

//Function to encode, https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C_2
//link provided from the lab
int base64encode(const void* data_buf, size_t dataLength, char* result, size_t resultSize){
   const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
   const uint8_t *data = (const uint8_t *)data_buf;
   size_t resultIndex = 0;
   size_t x;
   uint32_t n = 0;
   int padCount = dataLength % 3;

   uint8_t n0, n1, n2, n3;

   /* increment over the length of the string, three characters at a time */
   for (x = 0; x < dataLength; x += 3) 
   {
      /* these three 8-bit (ASCII) characters become one 24-bit number */
      n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+1) < dataLength)
         n += ((uint32_t)data[x+1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0
      
      if((x+2) < dataLength)
         n += data[x+2];

      /* this 24-bit number gets separated into four 6-bit numbers */
      n0 = (uint8_t)(n >> 18) & 63;
      n1 = (uint8_t)(n >> 12) & 63;
      n2 = (uint8_t)(n >> 6) & 63;
      n3 = (uint8_t)n & 63;
            
      /*
       * if we have one byte available, then its encoding is spread
       * out over two characters
       */
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n0];
      if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
      result[resultIndex++] = base64chars[n1];

      /*
       * if we have only two bytes available, then their encoding is
       * spread out over three chars
       */
      if((x+1) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n2];
      }

      /*
       * if we have all three bytes available, then their encoding is spread
       * out over four characters
       */
      if((x+2) < dataLength)
      {
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = base64chars[n3];
      }
        
        /*
            Rule of three to get progress
            dataLength 100
            currentPads   ?
            
        */
        currentPads += 3;
        progress = (currentPads*100.0)/dataLength;
        //Debug progress infof("Progreso: %d\n",progress);
   }

  

   /*
    * create and add padding that is required if we did not have a multiple of 3
    * number of characters available
    */
   if (padCount > 0) 
   { 
      for (; padCount < 3; padCount++) 
      { 
         if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
         result[resultIndex++] = '=';
      } 
   }
   if(resultIndex >= resultSize) return 1;   /* indicate failure: buffer too small */
   result[resultIndex] = 0;
   return 0;   /* indicate success */
}

//Function to decode, https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C_2
//link provided from the lab
int base64decode (char *in, size_t inLen, unsigned char *out, size_t *outLen) { 
    char *end = in + inLen;
    char iter = 0;
    u_int32_t buf = 0;
    size_t len = 0;
    
    int totalIterations = 0;
    int total = end-in;
    while (in < end) {
       
        unsigned char c = d[*in++];
         /*
            Rule to get decode
            currentDecode/total*100
            
        */
        currentDecode = (end-in) * 1.0;
        //Function to track progress in decode thanks to the help of a ClassMate
        progress = 100.0-(((currentDecode*1.0)/(total*1.0))*100.0);
        infof("Progress: %d\n", progress);
        switch (c) {
        case WHITESPACE: continue;   /* skip whitespace */
        case INVALID:    return 1;   /* invalid input, return error */
        case EQUALS:                 /* pad character, end of data */
            in = end;
            continue;
        default:
            buf = buf << 6 | c;
            iter++; // increment the number of iteration
            totalIterations++;
            /* If the buffer is full, split it into bytes */
            if (iter == 4) {
                if ((len += 3) > *outLen) return 1; /* buffer overflow */
                *(out++) = (buf >> 16) & 255;
                *(out++) = (buf >> 8) & 255;
                *(out++) = buf & 255;
                buf = 0; iter = 0;

            }   
        }
    }
   
    if (iter == 3) {
        if ((len += 2) > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 10) & 255;
        *(out++) = (buf >> 2) & 255;
    }
    else if (iter == 2) {
        if (++len > *outLen) return 1; /* buffer overflow */
        *(out++) = (buf >> 4) & 255;
    }

    *outLen = len; /* modify to reflect the actual output size */
    return 0;
}


void encodeController(char* file){
    char openName[256];
    strcat(openName, "/home/ap-user/ap-labs/labs/progress-notifier/");
    strcat(openName, file);
    strcat(openName, ".txt");
    FILE *fp = fopen(openName, "r");
    if(fp == NULL){
        errorf("Non existing file\n");
        return;
    }
    //Get file size
    fseek(fp, 0, SEEK_END);
    size_t sizeFile = ftell(fp); 
    rewind(fp);
    //Read file and put it into a buffer
    char* fileData = malloc(sizeFile);
    fread(fileData, 1, sizeFile, fp);
    //Get the result encoding size, needed to run function
    //Lenght formula, https://stackoverflow.com/questions/13378815/base64-length-calculation
    char* resultFile = (char*)malloc(sizeof(char)*4*(strlen(fileData)/3));
    int resultSize = 4*(strlen(fileData)/3);
    //Base 64 saves the result in resultFile, we are going to save this into a anew file  
    base64encode(fileData,sizeFile,resultFile,resultSize);
    char name[256];
    strcat(name, file);
    strcat(name, "-encoded.txt");
    FILE *fw = fopen(name,"w");
    fprintf(fw,"%s",resultFile);

    fclose(fp);
    fclose(fw);
}

void decodeController(char* file){
    char openName[256];
    strcat(openName, "/home/ap-user/ap-labs/labs/progress-notifier/");
    strcat(openName, file);
    strcat(openName, ".txt");
    FILE *fp2 = fopen(openName, "r");
    if(fp2 == NULL){
        errorf("Non existing file\n");
        return;
    }
    //Get file size
    fseek(fp2, 0, SEEK_END);
    size_t sizeFile = ftell(fp2); 
    rewind(fp2);
    //Read file and put it into a buffer
    char* fileData = malloc(sizeFile);
    fread(fileData, 1, sizeFile, fp2);

    //Get the result encoding size, needed to run function
    //Lenght formula, https://stackoverflow.com/questions/13378815/base64-length-calculation
    unsigned char* resultFile = (char*)malloc(sizeof(char)*(3*(strlen(fileData)/4)));
    size_t resultSize = sizeof(char)*(3*(strlen(fileData)/4));

    //Base 64 saves the result in resultFile, we are going to save this into a anew file  
    base64decode(fileData,sizeFile,resultFile,&resultSize);

    char writeName[256];
    strcat(writeName, file);
    strcat(writeName, "-decoded-final.txt");

    FILE *fw2 = fopen(writeName,"w");
    fprintf(fw2,"%s",resultFile);

    fclose(fp2);
    fclose(fw2);
}

static void sigHandler(int sig){
    //print progress
    infof("Progress: %d\n",progress);
}

int main(int argc, char *argv[]) {

    if(argc>3){
        errorf("Too many arguments\n");
    }

    if(argc<3){
        errorf("Too few arguments\n");
    }

    signal(SIGINT, sigHandler);
    signal(SIGUSR1, sigHandler);

    if(strcmp(argv[1], "--encode") == 0){
        encodeController(argv[2]);
    }else if(strcmp(argv[1], "--decode") == 0){
        decodeController(argv[2]);
    }

     

}