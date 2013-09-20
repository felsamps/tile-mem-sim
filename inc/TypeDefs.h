#ifndef TYPEDEFS_H
#define	TYPEDEFS_H

typedef int Int;
typedef long long int LLInt;

#define CTU_SIZE 64
#define CU_32_SIZE 32

#define TILE_CLASS_FACTOR 0
#define N_BASE_CONST 6
#define N_G_CONST 5
#define BASE_NUMBER_OF_GROUPS 8

enum PartSize
{
  SIZE_2Nx2N,           ///< symmetric motion partition,  2Nx2N
  SIZE_2NxN,            ///< symmetric motion partition,  2Nx N
  SIZE_Nx2N,            ///< symmetric motion partition,   Nx2N
  SIZE_NxN,             ///< symmetric motion partition,   Nx N
  SIZE_2NxnU,           ///< asymmetric motion partition, 2Nx( N/2) + 2Nx(3N/2)
  SIZE_2NxnD,           ///< asymmetric motion partition, 2Nx(3N/2) + 2Nx( N/2)
  SIZE_nLx2N,           ///< asymmetric motion partition, ( N/2)x2N + (3N/2)x2N
  SIZE_nRx2N,           ///< asymmetric motion partition, (3N/2)x2N + ( N/2)x2N
  SIZE_NONE = 15
};

#endif	/* TYPEDEFS_H */

