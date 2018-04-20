#ifndef _MY_SOLVER
#define _MY_SOLVER

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern "C"
{
    int rpoly_(double *, int *, double *, double *, long int *);
    int hybrj1_(void fcn(int *, double *, double *, double *, int *, int *), int *, double *, double *, double *, int *, double *, int *, double *, int *);
    int hybrd1_(void fcn(int *, double *, double *, int *), int *, double *, double *, double *, int *, double *, int *);
    int gespp_(void*, float*, int*, int*, float*);
    int solve_(void*, float*, int*, int*, float*, float*);

};

#endif