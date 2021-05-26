#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
//#include "Lattice.h"
#include "Lattice.cpp"
using namespace std;

int main(int argn, char *arg[])
{

  int N, Nsweep, Nrelax, Nmeasure, flag_hot;
  double T, J;
  N = 16;
  Nsweep = 50;
  Nrelax = 20;
  Nmeasure = 10;
  flag_hot = 0;
  T = 2.26;
  J = 1.0;

  // if input parameters are given
  
  char *fname = "datafile.dat"; // default input name
  char *gname = "moresweep.dat";

  if(argn==8)
    {
      fname = arg[1];
      gname = arg[2];
      N = atoi(arg[3]);
      Nsweep = atoi(arg[4]);
      Nrelax = atoi(arg[5]);
      Nmeasure = atoi(arg[6]);
      T = atof(arg[7]);
    }

  int k=0; int k_max=50;

  double Tinterval = 0.1;
  k_max=1;
  //for(T=3.50, k=0; k<k_max;T+=Tinterval, k++)
    {
      Lattice LL(fname,gname,k,N,Nsweep,Nrelax,Nmeasure,T,flag_hot,J,1);
      LL.experiment();
    }
  return 0;
}
