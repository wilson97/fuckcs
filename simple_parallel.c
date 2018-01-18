#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
// for testing
#include <unistd.h>
#include "stopwatch.c"
#include "parallel.h"

/* 
Plan: The base function will spin up T threads that are running the inner dist block. When they finish updating dist
for the i and j they are given, they will lock i and j, read them, update them, and then unlock them (using pthread mutex). To make sure all threads wait before advancing to next k we can use a pthread_barrier (do we even need this?) 
*/

typedef struct counters
{
   int thread_id;
   int** dist;
   int N;
   int T;
   int firstRow; // inclusive
   int lastRow; // not inclusive

} counters;

// pthread_barrier
pthread_barrier_t bar;
//pthread_barrier_t k_update;

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
      char line[10000];
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
      for (int j = 0; j < N; j++)
      {
         fprintf(file, "%d ", minList[i][j]);      
      }
      fprintf(file, "%s", "\n");
   }
   fclose(file);   
   return 0;
}

// the base function, initalizes dist, starts the threads
int** solvePathStart(int** adjList, int N, int T)
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

   pthread_t* thread_list = (pthread_t*) malloc(T * sizeof(pthread_t));
   //pthread_barrier_init(&bar, NULL, T);

   // algorithm for determing row many rows each thread does
   int R = N % T;
   int floor = N / T;
   int totalRows = 0;

   // TODO: need to test
   if (T > N)
   { 
      pthread_barrier_init(&bar, NULL, N);
      for (int i = 0; i < N; i++)
      {
         pthread_t thread;
         counters *cs = (counters*) malloc(sizeof(counters));
         cs->N = N;
         cs->thread_id = i;
         cs->dist = dist;
         cs->T = T;
         cs->firstRow = i;
         cs->lastRow = i + 1;           
         pthread_create(&thread, NULL, solvePaths, cs);
         thread_list[i] = thread;
      }
   }

   else
   {
      pthread_barrier_init(&bar, NULL, T);
      for (int i = 0; i < T; i++)
      {
         pthread_t thread;
         counters *cs = (counters*) malloc(sizeof(counters));
         cs->N = N;
         cs->thread_id = i; 
         cs->dist = dist;
         cs->T = T;
         cs->firstRow = totalRows;
      
         if (i < R)
         {
            cs->lastRow = totalRows + floor + 1;
            totalRows += floor + 1;
         }
         else
         {
            cs->lastRow = totalRows + floor;
            totalRows += floor;
         }
         pthread_create(&thread, NULL, solvePaths, cs);
         thread_list[i] = thread;
      }
   }

   for (int j = 0; j < T; j++)
   {
      pthread_join(thread_list[j], NULL);
   }

   pthread_barrier_destroy(&bar);
   return dist;
}

// follow the wikipedia algorithm
void* solvePaths(void *ptr)
{
   counters *cs = (counters *) ptr;
   int N = cs->N;
   int k = 0;
   int firstRow = cs->firstRow;
   int lastRow = cs->lastRow;
   while (k < N)
   {   
      for (int i = firstRow; i < lastRow; i++)
      {
         for (int j = 0; j < N; j++)
         {
            if (cs->dist[i][j] > cs->dist[i][k] + cs->dist[k][j])
            {
               cs->dist[i][j] = cs->dist[i][k] + cs->dist[k][j];
            }
         }
      }      

      pthread_barrier_wait(&bar);
      k += 1;   

   }
   pthread_exit(0);
}

int main(int argc, char *argv[])
{
   int T = 1;
   char* fileName;
   if (argc == 3)
   {
      T = atoi(argv[1]);
      fileName = argv[2];
   }
   else
   {
      printf("Wrong number of arguments specified\n");
      return(1);
   }

   int** t2 = NULL;
   t2 = readData(fileName);
   int size = getSize(fileName); 
   int** t3 = NULL;
   StopWatch_t sw;
   startTimer(&sw);   
   t3 = solvePathStart(t2, size, T);
   stopTimer(&sw);
   printf("Elapsed Time: %f\n", getElapsedTime(&sw));
   writeData("output.txt", t3, size);
   return(0);
}
