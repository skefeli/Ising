class Lattice
{
 public:
  int N, Nsweep, Nrelax, Nmeasure, flag_hot;
  double T, J, B;
  double M, sigma;
  TRandom3 rnd;
  int *s;
  Lattice(int,int,int,int,double,int,double); //constructor
  void showValues();
  void experiment();
  void sweep();
  double measure();
  double random_uniform();
};
