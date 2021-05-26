#include "Lattice.h"

using namespace std;

// This function returns random numbers between [0,1]
// uniformly distributed. It uses system's rng device
// for "true random number" every given period to seed
// the "pseduo random number" generator.
//
double Lattice::Random(void) {
  unsigned int r;
  
  if (use_rng_source) {
    static FILE *fd = NULL;
    static int count = 0;
    
    // Try to open the RNG device if this is the first call to this function
    if (count==0) {
      printf("Using /dev/urandom for periodic seeds.\n");
      fd = fopen("/dev/urandom", "r");
    }

    // If the system does not have a RNG (Random Number Generator) device
    // do not insist to use use: disable this feature.
    if (fd == NULL) {
      fprintf(stderr, "Cannot open RNG device. Using pseduo random numbers!\n");
      use_rng_source = 0;
    } else { 
      // read a true random number every 100000 calls
      if ((count%100000) == 0) {
        count = 0;
        fread(&r, sizeof(unsigned int), 1, fd);
        // seed the pseduo random number generator
        srandom(r);
      }  
      count++;
    }
  }
  
  // Return a random number between [0, 1]
  return (double)random() / (double)RAND_MAX;
}


Lattice::Lattice(int k_in,double *Eval_in,double *M_in, double *Msig_in, int N_in, int Nsweep_in, int Nrelax_in, int Nmeasure_in, double T_in, int flag_hot_in, double J_in, int use_rng_source_in)
{
  //rnd.SetSeed(0);
  int i,j; // counters, lattice sites
  double q; // random integer {-1,+1}
  int n_T = 10;

  //M_in = &sum;
  Mval = M_in;
  Msigval = Msig_in;
  Eval = Eval_in;
  k = k_in;

  use_rng_source = use_rng_source_in;

  // Experiment parameters
  N=N_in; Nsweep=Nsweep_in; T=T_in; flag_hot=flag_hot_in; J=J_in; Nsweep=Nsweep_in; Nmeasure=Nmeasure_in; Nrelax=Nrelax_in;
  B=0;
  
  // Constructing the Lattice

  s = (int **)malloc(sizeof(int *)*N);
  for (i=0;i<N;i++) s[i] = (int *)malloc(sizeof(int)*N);  
  for(i=0;i<N;i++)
    {
      for(j=0;j<N;j++)
	if(flag_hot)
	  {
	    q = rand()%2;
	    //q = rnd.Integer(2);
	    if(q==0) q=-1;
	    s[i][j] = q;
	  }
	else
	  {
	    s[i][j]=1;
	  }
    }
  calcexp();
  showValues();
  //print_spin();
  cout<<"M_i = "<<measure()<<endl;

  datafile = fopen("datafile.dat","a+");
  fprintf(datafile,"%d\n",N);
  fprintf(datafile,"%d\n",Nsweep);
  fprintf(datafile,"%d\n",Nrelax);
  fprintf(datafile,"%d\n",Nmeasure);
  fprintf(datafile,"%d\n",flag_hot);
  fprintf(datafile,"%lf\n",T);
  fprintf(datafile,"%lf\n",J);

  m_val = new double[Nmeasure];
  E_val = new double[Nmeasure];

}


Lattice::~Lattice() 
{
  //if (fout!=stdout) fclose(fout);
  //delete g1;
  //delete c1;
  fflush(datafile);
  fclose(datafile);
}

void Lattice::calcexp()
{
  expdET[0]=exp(8*J/T);
  expdET[1]=0;
  expdET[2]=exp(4*J/T);
  expdET[3]=0;
  expdET[4]=1;
  expdET[5]=0;
  expdET[6]=exp(-4*J/T);
  expdET[7]=0;
  expdET[8]=exp(-8*J/T);
}

void Lattice::showValues()
{
  cout<<"N="<<N<<endl;
  cout<<"Nsweep="<<Nsweep<<endl;
  cout<<"Nrelax="<<Nrelax<<endl;
  cout<<"Nmeasure="<<Nmeasure<<endl;
  cout<<"flag_hot="<<flag_hot<<endl;
  cout<<"T="<<T<<endl;
  cout<<"J="<<J<<endl;
  cout<<"B="<<B<<endl;
  //for(int i=0;i<8;i++) cout<<expdET[i]<<endl;
}

void Lattice::print_spin() 
{
  int i, j;
  // print the input file
  printf("\n");
  for (i=0;i<N;i++) 
    {
      for (j=0;j<N;j++) 
	{
	  if(s[i][j] == 1)
	    printf("+1 ");
	  if(s[i][j] == -1)
	    printf("-1 ");
	}
      printf("\n");
    }
  printf("\n");
}
// sweep Nsweep times
void Lattice::sweep(int Nsweep)
{
  int i,j; //lattice sites
  double deltaE; // energy difference
  double g; // ratio of probabilities
  double r; // random number between [0,1]
  
  double f; // debug

  int k;
  for(k=0;k<Nsweep;k++)
    {
      for(j=0;j<N;j++)
	{
	  for(i=0;i<N;i++)
	    {
	      g = expdET[4+s[i][j]*(s[(i-1+N)%N][j]+s[(i+1)%N][j]+s[i][(j-1+N)%N]+s[i][(j+1)%N])];
	      //deltaE=fdE(i,j);
	      //g = exp(-deltaE/T);
	      if(g>1)
		{
		  s[i][j] = -s[i][j];
		}
	      else
		{
		  r = Random();
		  if(g>r) s[i][j] = -s[i][j];
		}
	    }
	}
    }

}

void Lattice::experiment()
{
  int i;
  //double sum, sum2;
  double M1;
  double std;
  double avg;


  sum = 0.0;
  sum2 = 0.0;

  for(i=0;i<Nrelax;i++)
    {
      sweep(Nsweep);
    }

  for(i=0;i<Nmeasure;i++)
    {
      sweep(Nsweep);
      M1 = measure();
      sum += M1;
      sum2 += M1*M1;
      //cout<<i<<"  "<<"M = "<<measure()<<endl;
      //print_spin();
      /*if(i%1000==0)
	cout<<i<<endl;*/
      //cout<<i<<endl;
      
      // Filling the tree parameters
      m_val[i] = M1;
      E_val[i] = totEnergy();
      fprintf(datafile,"%d %lf   %lf\n",i,M1,totEnergy());
      
    }
  sum/=double(Nmeasure);
  sum2/=double(Nmeasure);
  std=sqrt((sum2-sum*sum)/(double)(Nmeasure));
  cout<<"M_f = "<<M1<<endl;
  cout<<"Mean = "<<sum<<endl;
  cout<<" Std = "<<std<<endl;
  cout<<"---------------------"<<endl;
  Mval[k]=sum;
  Msigval[k]=std;
  Eval[k]=totEnergy();
}

double Lattice::measure()
{
  // Adds all the spins in the Lattics and divides by the number of spins
  int i, j;
  M=0;

  for (i=0;i<N;i++) 
    {
      for (j=0;j<N;j++) 
	{
	  M+=s[i][j];
	}
    }
  M /= sq((double)N);
  //cout<<"M = "<<M<<endl;
  return M;
}


double Lattice::totEnergy()
{
  // Adds all the mutual interactions of the spins
  int i,j;
  Etot=0;
  for (i=0;i<N;i++) 
    {
      for (j=0;j<N;j++) 
	{
	  Etot+=-J*s[i][j]*(s[(i+1)%N][j]+s[i][(j+1)%N]);
	}
    }
  Etot /= sq((double)N);
  //cout<<"Etot = "<<Etot<<endl;
  return Etot;
}

double Lattice::sq(double x)
{
  return x*x;
}

double Lattice::fdE(int i, int j)
{
  // without magnetic field
  return 2*s[i][j]*(J*(s[(i-1+N)%N][j]+s[(i+1)%N][j]+s[i][(j-1+N)%N]+s[i][(j+1)%N]));
  // with magnetic field
  //return 2*s[i][j]*(J*(s[(i-1+N)%N][j]+s[(i+1)%N][j]+s[i][(j-1)%N]+s[i][(j+1)%N])+B);
}
