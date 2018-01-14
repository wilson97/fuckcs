#ifndef serial_h
#define serial_h
 
int readData(char* fileName, int** adjList);
int writeData(char* fileName, int** minList);
int solvePaths(int** adjList, int** minList);
 
#endif
