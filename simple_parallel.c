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

   // set up shared objects to pass to threads
   //counters cs;
   //cs.i = 0;
   //cs.j = 0;
   //cs.k = 0;
   //cs.N = N;
   //cs.dist = dist;
   //pthread_mutex_t mutex_i;
   //pthread_mutex_init(&mutex_i, NULL);
   //pthread_mutex_t mutex_j;
   //pthread_mutex_init(&mutex_j, NULL);
   //pthread_mutex_t mutex;
   //pthread_mutex_init(&mutex, NULL);
   //cs.mutex = &mutex;
   //cs.mutex_i = &mutex_i;
   //cs.mutex_j = &mutex_j;
   //cs.isKDone = 0;

   pthread_t* thread_list = (pthread_t*) malloc(T * sizeof(pthread_t));
   pthread_barrier_init(&bar, NULL, T);
   for (int i = 0; i < T; i++)
   {
      pthread_t thread;
      counters *cs = (counters*) malloc(sizeof(cs));
      cs->N = N;
      cs->thread_id = i; 
      cs->dist = dist;
      cs->T = T;
      pthread_create(&thread, NULL, solvePaths, cs);
      //printf("Thread creation id: %d\n", cs.thread_id);
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
   //printf("Thread Entered!\n");
   // first lock i and j and read it
   //printf("Thread started!\n");
   counters *cs = (counters *) ptr;
   int counter = cs->thread_id; 
   int k = 0;
   //printf("Thread id is:%d\n", cs->thread_id);
   while (k < cs->N)
   {
      
      //printf("Thread id is:%d\n", cs->thread_id);      
      while (counter < cs->N * cs->N)
      {
         int i = counter / cs->N;
         int j = counter - i*cs->N;
         //printf("Working on i: %d and j:%d and k:%d with thread:%d \n", i, j, k, cs->thread_id);      

         // update rule
         if (cs->dist[i][j] > cs->dist[i][k] + cs->dist[k][j])
         {
            cs->dist[i][j] = cs->dist[i][k] + cs->dist[k][j];
         }      
         //printf("N is %d with thread %d\n", cs->N, cs->thread_id);
         //printf("Working on i: %d and j:%d and k:%d with thread:%d \n", i, j, k, cs->thread_id);

         counter += cs->T;
      }
      
      // update k here
      //printf("Barrier hit with thread %d and k=%d \n", cs->thread_id, k);
      pthread_barrier_wait(&bar);
      k += 1;   
      counter = cs->thread_id;
      //printf("k updated with thread %d and counter %d \n", cs->thread_id, counter);
   }
   pthread_exit(0);
}

int main()
{
   int** t2 = NULL;
   t2 = readData("2.txt");
   int size = getSize("2.txt"); 
   int** t3 = NULL;
   StopWatch_t sw;
   startTimer(&sw);   
   t3 = solvePathStart(t2, size, 4);
   stopTimer(&sw);
   printf("Elapsed Time: %f\n", getElapsedTime(&sw));
   writeData("output.txt", t3, size);
   return(0);
}
