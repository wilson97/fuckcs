#ifndef parallel_h
#define parallel_h

int getSize(char* fileName); 
int** readData(char* fileName);
int writeData(char* fileName, int** minList, int N);
int** solvePathStart(int** adjList, int N, int T);
void* solvePaths(void *ptr);
 
#endif
