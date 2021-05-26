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


Lattice::Lattice(char *fname_in, char *gname_in, int k_in,int N_in, int Nsweep_in, int Nrelax_in, int Nmeasure_in, double T_in, int flag_hot_in, double J_in, int use_rng_source_in)
{
  //rnd.SetSeed(0);
  int i,j,k; // counters, lattice sites
  double q; // random integer {-1,+1}
  int n_T = 10;

  //M_in = &sum;
  kN = k_in;

  use_rng_source = use_rng_source_in;

  // Experiment parameters
  N=N_in; Nsweep=Nsweep_in; T=T_in; flag_hot=flag_hot_in; J=J_in; Nsweep=Nsweep_in; Nmeasure=Nmeasure_in; Nrelax=Nrelax_in;
  B=0;
  fname = fname_in;
  gname = gname_in;

  // Constructing the Lattice

  s = new int**[N];
  for(i=0;i<N;i++){
      s[i] = new int*[N];
      for(j=0;j<N;j++){
	  s[i][j] = new int[N];}}
  
  for(i=0;i<N;i++){
      for(j=0;j<N;j++){
	  for(k=0;k<N;k++)
	    if(flag_hot){
		q = rand()%2;
		//q = rnd.Integer(2);
		if(q==0) q=-1;
		s[i][j][k] = q;}
	    else{
	      s[i][j][k]=1;}}}

  calcexp();
  showValues();
  //print_spin();
  cout<<"M_i = "<<measure()<<endl;

 
  datafile = fopen(fname,"a+");
  if(datafile == NULL)
    {
      datafile = fopen("datafile.dat","w");
    }
  //datafile = fopen("datafile.dat","a+");
  //datafile = fopen("datafile.dat","w");
  fprintf(datafile,"%d\n",N);
  fprintf(datafile,"%d\n",Nsweep);
  fprintf(datafile,"%d\n",Nrelax);
  fprintf(datafile,"%d\n",Nmeasure);
  fprintf(datafile,"%d\n",flag_hot);
  fprintf(datafile,"%lf\n",T);
  fprintf(datafile,"%lf\n",J);
  
  moresweep = fopen(gname,"a+");
  if(moresweep == NULL)
    {
      moresweep = fopen(gname,"w");
    }
  fprintf(moresweep,"%d\n",N);
  fprintf(moresweep,"%d\n",Nsweep);
  fprintf(moresweep,"%d\n",Nrelax);
  fprintf(moresweep,"%d\n",Nmeasure);
  fprintf(moresweep,"%d\n",flag_hot);
  fprintf(moresweep,"%lf\n",T);
  fprintf(moresweep,"%lf\n",J);
 
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
  fflush(moresweep);
  fclose(moresweep);
}

void Lattice::calcexp()
{
  expdET[0]=exp(12*J/T);
  expdET[1]=0;
  expdET[2]=exp(8*J/T);
  expdET[3]=0;
  expdET[4]=exp(4*J/T);
  expdET[5]=0;
  expdET[6]=1;
  expdET[7]=0;
  expdET[8]=exp(-4*J/T);
  expdET[9]=0;
  expdET[10]=exp(-8*J/T);
  expdET[11]=0;
  expdET[12]=exp(-12*J/T);
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
  int i,j,k;
  printf("\n");
  for (i=0;i<N;i++) 
    {
      cout<<i<<endl;
      for (j=0;j<N;j++) 
	{
	  for (k=0;k<N;k++)
	    {
	      if(s[i][j][k] == 1)
		printf("+1 ");
	      if(s[i][j][k] == -1)
		printf("-1 ");
	    }
	  printf("\n");
	}
      printf("\n");
    }

}



// sweep Nsweep times
void Lattice::sweep(int Nsweep)
{
  int i,j,k; //lattice sites
  double deltaE; // energy difference
  double g; // ratio of probabilities
  double r; // random number between [0,1]
  
  double f; // debug

  int z;
  for(z=0;z<Nsweep;z++)
    {
      for(k=0;k<N;k++)
	{
	  for(j=0;j<N;j++)
	    {
	      for(i=0;i<N;i++)
		{
		  g = expdET[6+s[i][j][k]*(s[(i-1+N)%N][j][k]+s[(i+1)%N][j][k]+s[i][(j-1+N)%N][k]+s[i][(j+1)%N][k]+s[i][j][(k-1+N)%N]+s[i][j][(k+1)%N])];
		  //deltaE=fdE(i,j,k);
		  //g = exp(-deltaE/T);
		  if(g>1)
		    {
		      s[i][j][k] = -s[i][j][k];
		    }
		  else
		    {
		      r = Random();
		      //r = rand()/RAND_MAX;
		      if(g>r) s[i][j][k] = -s[i][j][k];
		    }
		}
	    }
	}
    }

}

void Lattice::experiment()
{
  int i,j;
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
      for(j=0;j<5;j++)
	{
	  sweep(Nsweep/5);
	  M1 = measure();
      
	  // Filling the tree parameters
	  m_val[i] = M1;
	  E_val[i] = totEnergy();
	  fprintf(moresweep,"%d %lf   %lf\n",i,M1,totEnergy());
	}
      //cout<<i<<endl;
      fprintf(datafile,"%d %lf   %lf\n",i,M1,totEnergy());
      
    }
  /*sum/=double(Nmeasure);
  sum2/=double(Nmeasure);
  std=sqrt((sum2-sum*sum)/(double)(Nmeasure));
  cout<<"M_f = "<<M1<<endl;
  cout<<"Mean = "<<sum<<endl;
  cout<<" Std = "<<std<<endl;
  cout<<"---------------------"<<endl;*/
}

double Lattice::measure()
{
  // Adds all the spins in the Lattics and divides by the number of spins
  int i, j, k;
  M=0;

  for (i=0;i<N;i++) 
    {
      for (j=0;j<N;j++) 
	{
	  for(k=0;k<N;k++)
	    {
	      M+=s[i][j][k];
	    }
	}
    }
  M /= cube((double)N);
  //cout<<"M = "<<M<<endl;
  return M;
}


double Lattice::totEnergy()
{
  // Adds all the mutual interactions of the spins
  int i,j,k;
  Etot=0;
  for (i=0;i<N;i++) 
    {
      for (j=0;j<N;j++) 
	{
	  for(k=0;k<N;k++)
	    {
	      Etot+=-J*s[i][j][k]*(s[(i+1)%N][j][k]+s[i][(j+1)%N][k]+s[i][j][(k+1)%N]);
	    }
	}
    }
  Etot /= cube((double)N);
  //cout<<"Etot = "<<Etot<<endl;
  return Etot;
}

double Lattice::sq(double x)
{
  return x*x;
}

double Lattice::cube(double x)
{
  return x*x*x;
}

double Lattice::fdE(int i, int j, int k)
{
  // without magnetic field
  //return 2*s[i][j]*(J*(s[(i-1+N)%N][j]+s[(i+1)%N][j]+s[i][(j-1+N)%N]+s[i][(j+1)%N]));
  return 2*J*s[i][j][k]*(s[(i-1+N)%N][j][k]+s[(i+1)%N][j][k]+s[i][(j-1+N)%N][k]+s[i][(j+1)%N][k]+s[i][j][(k-1+N)%N]+s[i][j][(k+1)%N]);
  // with magnetic field
  //return 2*s[i][j]*(J*(s[(i-1+N)%N][j]+s[(i+1)%N][j]+s[i][(j-1)%N]+s[i][(j+1)%N])+B);
}
