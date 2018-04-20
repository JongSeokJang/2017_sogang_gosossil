#ifndef _MY_SOLVER
#define _MY_SOLVER

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const double EPSILON = 0.0000001;

extern "C"
{
	int gespp_(void*, float*, int*, int*, float*);
    int solve_(void*, float*, int*, int*, float*, float*);
	int rpoly_(double *, int *, double *, double *, long int *);
	int hybrj1_(void fcn(int *, double *, double *, double *, int *, int *), int *, double *, double *, double *, int *, double *, int *, double *, int *);
	int hybrd1_(void fcn(int *, double *, double *, int *), int *, double *, double *, double *, int *, double *, int *);
};

#endif