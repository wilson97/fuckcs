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
   //int i;
   int thread_id;
   //int j;
   //int k;
   int** dist;
   int N;
   int T;
   int firstRow; // inclusive
   int lastRow; // not inclusive
   //int tid;
   //pthread_mutex_t *mutex_i;
   //pthread_mutex_t *mutex_j; 
   //pthread_mutex_t *mutex;
   // k over is 1, still work to do is 0
   //int isKDone;   

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
   pthread_barrier_init(&bar, NULL, T);

   // algorithm for determing row many rows each thread does
   int R = N % T;
   int floor = N / T;
   int totalRows = 0;

   for (int i = 0; i < T; i++)
   {
      pthread_t thread;
      counters *cs = (counters*) malloc(sizeof(cs));
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
   //StopWatch_t sw;
   //startTimer(&sw);

   counters *cs = (counters *) ptr;
   //int counter = cs->thread_id; 
   //int size = 0;
   int N = cs->N;
   //int T = cs->T;
   int k = 0;
   int firstRow = cs->firstRow;
   int lastRow = cs->lastRow;
   /*
   // maybe first precompute the i/j values?
   while (counter < N * N)
   {
      //int i = counter / N; 
      //int j = counter - i*N;
      counter += T;
      size += 1;   
   }

   int i_list[size]; 
   //int* i_list = (int*) malloc(size * sizeof(int));
   int j_list[size]; 
   counter = cs->thread_id;      
   int index = 0;

   
   while (counter < N * N)
   {
      int temp = counter / N;
      i_list[index] = temp;
      //printf("temp is: %d\n", temp);
      j_list[index] = counter - temp*N;
      counter += T;
      index += 1;
   }
   */

   //stopTimer(&sw);
   //printf("Elapsed Time for thread setup: %f in i j update\n", getElapsedTime(&sw));
   /*
   for (int i = 0; i < size; i++)
   {
      printf("%d\n", i_list[i]);
   }
   */

  // printf("i_list end is: %d\n", i_list[size - 1]);
   //startTimer(&sw);
   while (k < N)
   {
      for (int i = firstRow; i < lastRow; i++)
      {
         for (int j = 0; j < N; j++)
         {
            //(1,2) (1,0), (0,2)
            if (cs->dist[i][j] > cs->dist[i][k] + cs->dist[k][j])
            {
               cs->dist[i][j] = cs->dist[i][k] + cs->dist[k][j];
            }
         }
      }      
      //startTimer(&sw);
      //printf("Thread id is:%d\n", cs->thread_id);      

      /*
      while (counter < N * N)
      {
         int i = 0;
         int j = 0;
         //startTimer(&sw);
         //int i = counter / N;
         //int j = counter - i*N;
         //int i = counter / cs->N;
         //int j = counter - i*cs->N;
         //stopTimer(&sw);
         //printf("Elapsed Time: %f in i j update\n", getElapsedTime(&sw));
         //printf("Working on i: %d and j:%d and k:%d with thread:%d \n", i, j, k, cs->thread_id);      

         // update rule
         //startTimer(&sw);
         if (cs->dist[i][j] > cs->dist[i][k] + cs->dist[k][j])
         {
            cs->dist[i][j] = cs->dist[i][k] + cs->dist[k][j];
         }
         //stopTimer(&sw);
         //printf("Elapsed Time in update rule: %f\n", getElapsedTime(&sw));      
         //printf("N is %d with thread %d\n", cs->N, cs->thread_id);
         //printf("Working on i: %d and j:%d and k:%d with thread:%d \n", i, j, k, cs->thread_id);

         //startTimer(&sw);
         counter += T;
         //stopTimer(&sw);
         //printf("Elapsed Time in counter + T: %f\n", getElapsedTime(&sw));
         
      }
      */
      /*
      for (int a = 0; a < size; a++)
      {
         if (cs->dist[i_list[a]][j_list[a]] > cs->dist[i_list[a]][k] + cs->dist[k][j_list[a]])
         {
            cs->dist[i_list[a]][j_list[a]] = cs->dist[i_list[a]][k] + cs->dist[k][j_list[a]];
         }      
      }
      */
      //stopTimer(&sw);
      //printf("Elapsed Time in inner loop: %f\n", getElapsedTime(&sw));      

      // update k here
      //printf("Barrier hit with thread %d and k=%d \n", cs->thread_id, k);
      //startTimer(&sw);
      pthread_barrier_wait(&bar);
      //stopTimer(&sw);
      //printf("Elapsed Time in barrier wait: %f\n", getElapsedTime(&sw));  
      k += 1;   
      //counter = cs->thread_id;
      //printf("k updated with thread %d and counter %d \n", cs->thread_id, counter);
   }
   //stopTimer(&sw);
   //stopTimer(&sw);
   //printf("Elapsed Time in outer loop: %f\n", getElapsedTime(&sw));
   pthread_exit(0);
}

int main()
{
   int** t2 = NULL;
   t2 = readData("1.txt");
   int size = getSize("1.txt"); 
   int** t3 = NULL;
   StopWatch_t sw;
   startTimer(&sw);   
   t3 = solvePathStart(t2, size, 1);
   stopTimer(&sw);
   printf("Elapsed Time: %f\n", getElapsedTime(&sw));
   writeData("output.txt", t3, size);
   return(0);
}
