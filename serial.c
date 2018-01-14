#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"

//helper function to tokenize a string
char** stringToArr(char* str, int N)
{
   char* c;
   char** outArr = (char**) malloc(N * sizeof(char*));
   int index = 0; 
   c = strtok(str, " ");
   char cp[1000];
   while (c != NULL)
   {
      strcpy(cp, c);
      outArr[index] = cp; 
      c = strtok(NULL, " ");
   }
   return outArr;
}

// does two things: reads the file data into adjList and then return the size (N, number of nodes) of the adjList
int readData(char* fileName, int** adjList)
{
   int size;
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
      while (fgets(line, sizeof(line), file) != NULL)
      {
         

      }
   }
   return 0;
}

int writeData(char* fileName, int** minList)
{
   return 0;
}

int solvePaths(int** adjList, int** minList)
{
   return 0;
}

int main()
{
   //int** adjList = (int**) malloc(N * sizeof(int *));
   char** t1 = stringToArr("5 6 7 8 9", 5);
   for (int i = 0; i < 5; i++)
   {
      printf("%s", t1[i]);
   }
   return(0);
}
