#include <stdio.h>
#include <limits.h>

#define kDim 800
#define kOffset 127
#define kFactor 31

typedef double Matrix[kDim][kDim]; // user defined kDim x kDim matrix type

// autofill the matrix with values
void fillMat(Matrix trg, int seed) {
   int row, col, val = seed;

   for (row = 0; row < kDim; row++)
      for (col = 0; col < kDim; col++) {
         val = val * kFactor + kOffset;
         trg[row][col] = val / (double)INT_MAX;
      }
}

// pretty print matrix as an array
void printMat(Matrix mat) {
   int row, col;

   for (row = 0; row < kDim; row++) {
      for (col = 0; col < kDim; col++) 
         printf("%0.4lf ", mat[row][col]);
      printf("\n");
   }
   printf("\n");
}

// does a matrix multiplication and puts the result in prd
double multMat(Matrix prd, Matrix m1, Matrix m2) {
   int row, col, idx;
   double checkSum = 0.0, val;

   for (row = 0; row < kDim; row++) { // foreach row
      for (col = 0; col < kDim; col++) { // foreach col
         val = 0.0;
         for (idx = 0; idx < kDim; idx++)
            val += m1[row][idx] * m2[idx][col];

         prd[row][col] = val;
         checkSum += val;
      }
   }

   return checkSum;
}

Matrix m1, m2, prd; // global variables ? 

int main(int argc, char **argv) {
   double checkSum;

   fillMat(m1, 10);
   fillMat(m2, 20);
   checkSum = multMat(prd, m1, m2);

   // printMat(m1);
   // printMat(m2);
   printf("\n%lf\n", checkSum);
   // printMat(prd);
}
