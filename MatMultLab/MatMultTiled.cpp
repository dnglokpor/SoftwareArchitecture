#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define kDim 800
#define kOffset 127
#define kFactor 31

using namespace std;

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

// tile auto increment method
inline int limitOf(int begin, int size){
   return begin + size < kDim ? begin + size : kDim;
}

// does a matrix multiplication and puts the result in prd
// breaks the matrix into smaller submatrices
double multMat(Matrix prd, Matrix m1, Matrix m2, int tileSize) {
   int row, col, idx;
   double checkSum = 0.0, val;
   // product submatrices are [rEnd - rIdx][cEnd - cIdx]
   // operand submatrices are defined by [subEnd - subIdx][subEnd - subIdx]
   int rIdx, rEnd, cIdx, cEnd, subIdx, subEnd;
   
   // product matrix tiles loop
   for(rIdx = 0; rIdx < kDim; rIdx = rEnd){
      rEnd = limitOf(rIdx, tileSize);
      for(cIdx = 0; cIdx < kDim; cIdx = cEnd){
         cEnd = limitOf(cIdx, tileSize);
         // operand matrices tiles loop
         for(subIdx = 0; subIdx < kDim; subIdx = subEnd){
            subEnd = limitOf(subIdx, tileSize);
            // matrix multiplication
            for (row = rIdx; row < rEnd; row++) { // foreach row
               for (col = cIdx; col < cEnd; col++) { // foreach col
                  // prd[row][col] = 0.0;
                  for (idx = subIdx; idx < subEnd; idx++)
                     prd[row][col] += m1[row][idx] * m2[idx][col];
                  
                  if(idx == kDim) // last tiles results computed
                     checkSum += prd[row][col];
               }
            }
         }
      }
   }

   return checkSum;
}

Matrix m1, m2, prd; // global variables

int main(int argc, char **argv) {
   int tileSize;
   double checkSum;
   
   // commandline args
   if (argc != 2)
      cout << "Usage: MatMultTiledX <tileSize>" << endl;
   else {
      tileSize = atoi(argv[1]);
   }

   fillMat(m1, 10);
   fillMat(m2, 20);
   checkSum = multMat(prd, m1, m2, tileSize);

   // printMat(m1);
   // printMat(m2);
   printf("\n%lf\n", checkSum);
   // printMat(prd);
}
