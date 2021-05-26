#include <iostream>
#include <iomanip>
#include "Lattice.h"
#include "Lattice.cpp"
using namespace std;

//int main()
void ising1d()
{
  int N, Nsweep, Nrelax, Nmeasure, flag_hot;
  double T, J, B;

  N = 50;
  Nsweep = 50;
  Nrelax = 10;
  Nmeasure = 20;
  flag_hot = 1;
  J = 1.0;
  T = 1.0;

  Lattice l(N,Nsweep,Nrelax,Nmeasure,T,flag_hot,J,1);
  l.showValues();
  l.experiment();
}
