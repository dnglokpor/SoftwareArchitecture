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
   int rBase;                    // Row base for computation
   int numWorkers;               // Number of threads total, and amount we should skip
   double *totalSum;             // Address of unique checkSum

   // constructor: asserting base struct is auto built since constructor
   // is standalone
   MatThread(Matrix m1[], Matrix m2[], Matrix prd[], int rB, int nW, double *s):
      Thread(), op1(m1), op2(m2), res(prd), rBase(rB), numWorkers(nW),
      totalSum(s)
   {}

   // ThreadMain() is inherited
   // Join() is inherited
   
   // implement Run()
   // computes select rows of the product matrix by matrix multiplication
   void *Run() {
      int row, col, idx;
      double partialSum = 0.0, val;

      // index select rows based on rBase and numWorkers
      for (row = rBase; row < kDim; row += numWorkers) {
         for (col = 0; col < kDim; col++) { // for each column
            val = 0.0;
            for (idx = 0; idx < kDim; idx++){ // each cell
               // compute sum of products
               val += (*op1)[row][idx] * (*op2)[idx][col];
            }

            (*res)[row][col] = val;
            partialSum += val; // sum of the matrix
         }
      }
      // synchronized block
      pthread_mutex_lock(&mutex);
      (*totalSum) += partialSum; // add inner sum to total sum
      pthread_mutex_unlock(&mutex);
   }
}; // MatThread END

// initilialize static mutex member
pthread_mutex_t MatThread::mutex = PTHREAD_MUTEX_INITIALIZER;

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

// global declaration of matrices (but why ?)
Matrix m1, m2, prd;

// main
int main(int argc, char **argv) {
   int idx, numThreads;
   double checkSum = 0.0;
   Thread **threads; // for threading
   
   if (argc != 2)
      cout << "Usage: MatMultThreads <numThreads>" << endl;
   else {
      numThreads = atoi(argv[1]);
   }

   // autofill matrices
   fillMat(m1, 10);
   fillMat(m2, 20);
   
   // start threads
   threads = (Thread **)calloc(sizeof(Thread *), numThreads);
   for (idx = 0; idx < numThreads; idx++)
      threads[idx] = new MatThread(&m1, &m2, &prd, idx, numThreads, &checkSum);
   
   // wait out threads and collect results
   for (idx = 0; idx < numThreads; idx++)
      threads[idx]->Join();
   
   // print results and exit
   // printMat(m1);
   // printMat(m2);
   printf("\n%lf\n", checkSum);
   // printMat(prd);
}
