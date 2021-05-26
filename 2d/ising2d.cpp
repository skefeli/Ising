#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
//#include "Lattice.h"
#include "Lattice.cpp"
using namespace std;

//int main(int argn, char *arg[])
int main()
{

  int N, Nsweep, Nrelax, Nmeasure, flag_hot;
  double T, J, B;
  N = 32;
  Nsweep = 50;
  Nrelax = 20;
  Nmeasure = 1000;
  flag_hot = 0;
  T = 2.26;
  J = 1.0;
  
  int k=0; int k_max=50;
  double *M;
  double *Msig;
  double *Eval;
  M = (double*)malloc(sizeof(double)*k_max);
  Msig = (double*)malloc(sizeof(double)*k_max);
  Eval = (double*)malloc(sizeof(double)*k_max);

  double Tinterval = 0.01;
  k_max=20;
  for(T=2.16, k=0; k<k_max;T+=Tinterval, k++)
    {
      Lattice LL(k,Eval,M,Msig,N,Nsweep,Nrelax,Nmeasure,T,flag_hot,J,1);
      LL.experiment();
    }
  //return 0;
}
