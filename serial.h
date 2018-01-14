#ifndef serial_h
#define serial_h
 
int** readData(char* fileName);
int writeData(char* fileName, int** minList, int N);
int solvePaths(int** adjList, int** minList);
 
#endif
