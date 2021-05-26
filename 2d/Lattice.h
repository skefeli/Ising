#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

class Lattice
{
  int **s;
 public:
  int N, Nsweep, Nrelax, Nmeasure, flag_hot;
  int k;
  double T, J, B;
  double M, sigma;
  double Etot;
  double sum, sum2;
  double *Mval; double *Msigval;
  double *Eval;
  //TRandom3 rnd;
  double expdET[9]; // calculate exponentianls before
  // expdET[4+s*(s1+s2+s3+s4)]
  Lattice(int,double *,double *,double *,int,int,int,int,double,int,double,int use_rng_source_in=0); //constructor
  ~Lattice();
  void calcexp();
  void showValues(); // print the current parameters
  void print_spin(); // print the spin configuration
  void experiment(); // perform the experiment
  double measure(); // measure the magnetic moment
  double totEnergy(); // measures the total energy of the system
  void sweep(int); // metropolis et. al.
  double sq(double); // taking the square
  //double random_uniform(); // random number between 0 and 1
  double fdE(int,int); // the difference in energy when the spin in lattice site (i,j) is fliped

  int use_rng_source;
  double Random(void);


  FILE *datafile;
  
  double *m_val; // array for magnetization
  double *E_val; // array for energy
};
