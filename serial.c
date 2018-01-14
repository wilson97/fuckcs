#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"

//helper function to tokenize a string
int* stringToArr(char* str, int N)
{
   char* c;
   int* outArr = (int*) malloc(N * sizeof(int));
   int index = 0; 
   c = strtok(str, " ");
   //char cp[1000];
   while (c != NULL)
   {
      //strcpy(cp, c);
      outArr[index] = atoi(c); 
      c = strtok(NULL, " ");
      //printf("%s\n", outArr[0]); 
      index += 1;
   }
   return outArr;
}

// does one thing: returns the newly created adjacency list
int** readData(char* fileName)
{
   int size = 0;
   int** adjList = NULL;
   FILE *file = fopen(fileName, "r");
   if (file != NULL)
   {
      char line[10000];
      // the first line will have N, the size of the adjList, so now we can initialize adjList
      if (fgets(line, sizeof(line), file) != NULL)
      {
         size = atoi(line);
         adjList = (int**) malloc(size * sizeof(int *));
	 for (int i = 0; i < size; i++)
         {
            adjList[i] = (int*) malloc(size * sizeof(int));
         } 
      }

      // now load the text file into adjList 
      int index = 0;
      while (fgets(line, sizeof(line), file) != NULL)
      {
         adjList[index] = stringToArr(line, size);
         index += 1;
      }
      printf("TRY: %d\n", adjList[2][3]);

   }
   fclose(file);
   return adjList;
}

int writeData(char* fileName, int** minList, int N)
{
   FILE *file = fopen(fileName, "w");
   if (file == NULL)
   {
      printf("Something bad happened when opening file\n");
      return -1;
   }
      
}

int solvePaths(int** adjList, int** minList)
{
   return 0;
}

int main()
{
   /* To test string tokenization */
   
   //int** adjList = (int**) malloc(N * sizeof(int *));
   printf("Testing part 1:\n");
   char str[] = "5 6 7 8 9";
   int* t1 = stringToArr(str, 5);
   for (int i = 0; i < 5; i++)
   {
      printf("%d\n", t1[i]);
   }

   printf("Testing part 2:\n");
   int** t2 = NULL;
   //int size;
   t2 = readData("test.txt"); 
   printf("Size: %lu\n", sizeof(t2[0]));
   printf("%d\n", t2[2][3]);
   return(0);
}
