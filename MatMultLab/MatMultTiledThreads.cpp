#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <limits.h>

using namespace std;

#define kDim 800 
#define kOffset 127
#define kFactor 31

typedef double Matrix[kDim][kDim];

// signature of used method
int limitOf(int begin, int size);

// Thread base class.  Derive subclass and implement Run
struct Thread {
   pthread_t id;        // ID of created thread

   // workload call
   static void *ThreadMain(void *vThread) {
      return ((Thread *)vThread)->Run();
   }
   
   // activity control
   virtual void Join() {
      pthread_join(id, NULL);
   }
   
   // constructor
   Thread() {
      pthread_create(&id, NULL, ThreadMain, this);
   }
   
   // pure abstract method to be implemented
   virtual void *Run() = 0;
};

// MatThread struct derived from Thread
struct MatThread : public Thread{
   // Thread provides thread ID thus we don't need our own
   static pthread_mutex_t mutex; // synchronization control
   Matrix *op1, *op2;            // Operand matrices
   Matrix *res;                  // result matrix
   int cBase;                    // column base for computation
   int numWorkers;               // number of threads total, and amount we should skip
   double *totalSum;             // address of unique checkSum
   int tileSize;                 // dimension of sub matrices

   // constructor: asserting base struct is auto built since constructor
   // is standalone
   MatThread(Matrix m1[], Matrix m2[], Matrix prd[], int cB, int nW, double *s,
      int tS) : Thread(), op1(m1), op2(m2), res(prd), cBase(cB), numWorkers(nW),
      totalSum(s), tileSize(tS)
   {}

   // ThreadMain() is inherited
   // Join() is inherited
   
   // implement Run()
   // computes select rows of the product matrix by matrix multiplication
   void *Run() {
      int row, col, idx;
      double partialSum = 0.0, val;
      int rIdx, rEnd, cIdx, cEnd, subIdx, subEnd;
      int incr = numWorkers * tileSize, next = cBase, count;
      
      // index select rows based on rBase and numWorkers
      // product matrix tiles loop
      for (rIdx = 0; rIdx < kDim; rIdx = rEnd){
         rEnd = limitOf(rIdx, tileSize);
         for (cIdx = next; cIdx < kDim; cIdx = next){
            cEnd = limitOf(cIdx, tileSize);
            next += incr; // find next submatrix
            // operand matrices tiles loop
            for (subIdx = 0; subIdx < kDim; subIdx = subEnd){
               subEnd = limitOf(subIdx, tileSize);
               // matrix multiplication
               for (row = rIdx; row < rEnd; row++) { // foreach row
                  for (col = cIdx; col < cEnd; col++) { // foreach col
                     // prd[row][col] = 0.0;
                     for (idx = subIdx; idx < subEnd; idx++)
                        (*res)[row][col] += (*op1)[row][idx] * (*op2)[idx][col];
                     
                     if(idx == kDim) // last submatrices multiplication
                        partialSum += (*res)[row][col];
                  }
               }
            }
         }
         // next submatrix
         count = -1;
         while(next >= kDim){ // case next submatrix is on next row
            next -= tileSize;
            count++;
         }
         // cout << "count is " << count << endl; DEBUG
         if(count != -1) // case next submatrix is on this row
            next = count * tileSize;
         
      }
      // synchronized block
      pthread_mutex_lock(&mutex);
      (*totalSum) += partialSum; // add inner sum to total sum
      pthread_mutex_unlock(&mutex);
   }
}; // MatThread END

// initilialize static mutex member
pthread_mutex_t MatThread::mutex = PTHREAD_MUTEX_INITIALIZER;

// tile auto increment method
inline int limitOf(int begin, int size){
   return begin + size < kDim ? begin + size : kDim;
}

// autofill procedure
void fillMat(Matrix trg, int seed) {
   int row, col, val = seed;

   for (row = 0; row < kDim; row++)
      for (col = 0; col < kDim; col++) {
         val = val * kFactor + kOffset;
         trg[row][col] = val / (double)INT_MAX;
      }
}

// pretty print procedure
void printMat(Matrix mat) {
   int row, col;

   for (row = 0; row < kDim; row++) {
      for (col = 0; col < kDim; col++) 
         printf("%0.4lf ", mat[row][col]);
      printf("\n");
   }
   printf("\n");
}

// global declaration of matrices
Matrix m1, m2, prd; // global variables ? 

// main
int main(int argc, char **argv) {
   int idx, numThreads, tileSize;
   double checkSum = 0.0;
   Thread **threads; // for threading
   
   // commandline args
   if (argc != 3)
      cout << "Usage: MatMultThreads <numThreads> <tileSize>" << endl;
   else {
      numThreads = atoi(argv[1]);
      tileSize = atoi(argv[2]);
   }

   // autofill matrices
   fillMat(m1, 10);
   fillMat(m2, 20);
   
   // start threads
   threads = (Thread **)calloc(sizeof(Thread *), numThreads);
   for (idx = 0; idx < numThreads; idx++)
      threads[idx] = new MatThread(&m1, &m2, &prd, idx * tileSize, numThreads,
      &checkSum, tileSize);
   
   // wait out threads and collect results
   for (idx = 0; idx < numThreads; idx++)
      threads[idx]->Join();
   
   // print results and exit
   // printMat(m1);
   // printMat(m2);
   printf("\n%lf\n", checkSum);
   // printMat(prd);
}
