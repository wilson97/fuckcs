#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "stopwatch.c"

// helper function to tokenize a string
int* stringToArr(char* str, int N)
{
   char* c;
   int* outArr = (int*) malloc(N * sizeof(int));
   int index = 0; 
   c = strtok(str, " ");
   while (c != NULL)
   {
      outArr[index] = atoi(c); 
      c = strtok(NULL, " ");
      index += 1;
   }
   //printf("Final Index is: %d, N is %d\n", index - 1, N);
   return outArr;
}

// returns the size - from the first line of the text file
int getSize(char* fileName)
{
   int size = 0;
   FILE *file = fopen(fileName, "r");
   if (file != NULL)
   {
      char line[100];
      if (fgets(line, sizeof(line), file) != NULL)
      {
         size = atoi(line);   
      }
   }
   fclose(file);
   return size;
}

// returns the newly created adjacency list
int** readData(char* fileName)
{
   int size = 0;
   int** adjList = NULL;
   FILE *file = fopen(fileName, "r");
   if (file != NULL)
   {
      char line[12345];
      // the first line will have N, the size of the adjList, so now we can initialize adjList
      if (fgets(line, sizeof(line), file) != NULL)
      {
         size = atoi(line);
         adjList = (int**) malloc(size * sizeof(int *));
      }

      // now load the text file into adjList 
      int index = 0;
      while (fgets(line, sizeof(line), file) != NULL)
      {
         adjList[index] = stringToArr(line, size);
         index += 1;
      }

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
   for (int i = 0; i < N; i++)
   {
      //printf("i is : %d\n", i);
      for (int j = 0; j < N; j++)
      {
         //printf("j is : %d\n", j);
         fprintf(file, "%d ", minList[i][j]);      
      }
      fprintf(file, "%s", "\n");
   }
   fclose(file);   
   return 0;
}

// follow the wikipedia algorithm
int** solvePaths(int** adjList, int N)
{
   if (N == 0)
   {
      printf("N is 0\n");
      return NULL;
   }
   
   // copy adjacency list into dist, we assume adjList already has 0 for dist[k][k] and 10^7 for no edge
   int** dist = (int**) malloc(N * sizeof(int *));
   for (int i = 0; i < N; i++)
   {  
      dist[i] = (int*) malloc(N * sizeof(int));
      for (int j = 0; j < N; j++)
      {
         dist[i][j] = adjList[i][j];
      } 
   }

   for (int k = 0; k < N; k++)
   {
      for (int i = 0; i < N; i++)
      {
         for (int j = 0; j < N; j++)
         {
            if (dist[i][j] > dist[i][k] + dist[k][j])
            {
               dist[i][j] = dist[i][k] + dist[k][j];
            } 
         }
      }
   }
   return dist;
}

int main(int argc, char *argv[])
{
   char* fileName = "10.txt";
   if (argc == 2)
   {
      fileName = argv[1];
   }

   int** t2 = readData(fileName);
   int size = getSize(fileName); 
   //printf("Size is %d\n", size);
   int** t3 = NULL;
   StopWatch_t sw;
   startTimer(&sw);
   t3 = solvePaths(t2, size);
   stopTimer(&sw);
   printf("Elapsed Time for serial with size %d is: %f\n", size, getElapsedTime(&sw));
   writeData("answer.txt", t3, size);
   //if (t2 == NULL) {}

   return(0);
}
