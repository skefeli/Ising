void Lattice::Lattice(int N_in, int Nsweep_in, int Nrelax_in, int Nmeasure_in, double T_in, int flag_hot_in, double J_in, double B_in)
{
  N=N_in; Nsweep=Nsweep_in; T=T_in; flag_hot=flag_hot_in; J=J_in; Nsweep=Nsweep_in; Nmeasure=Nmeasure_in; Nrelax=Nrelax_in; B=B_in;
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
  cout<<"B="<<J<<endl;
}

void Lattice::experiment()
{
  
  double B=0;
  int i;
  double sum, sum2, M1, q;

  s = (int *)malloc(N*sizeof(int));

  for(i=0; i<N;i++)
    {
      q = rnd.Integer(2);
      if(q==0) q=-1;
      s[i] = q;
    }

  //relaxation
  for(i=0;i<Nrelax;i++)sweep();
  
  sum = 0.0;
  sum2 = 0.0;
  
  
  for(B=-1.0;B<1.01;B+=0.1)
    {
      for(i=0;i<Nmeasure;i++)
	{
	  sweep();
	  M1 = measure();
	  sum += M1;
	  sum2 += M1*M1;
	}
  
      M = sum / (double)Nmeasure;
      sigma = sqrt(sum2 / (double)Nmeasure - (M)*(M))/sqrt((double)Nmeasure);
  
      free(s);
  
      cout<<"B = "<<B<<"  M = "<<M<<endl;
    }
}

void Lattice::sweep()
{
  double deltaE,g,r;
  int i,j;
  
  for(j=0;j<Nsweep;j++)
    {
      for(i=0;i<N;i++)
	{
	  deltaE = 2*s[i]*(J*(s[(i-1+N)%N]+s[(i+1)%N])+B);
	  g = exp(-deltaE/T);
	  if(g>1)
	    {
	      s[i] = -s[i];
	    }
	  else
	    {
	      r = random_uniform();
	      if(g>r) s[i] = -s[i];
	    }
	}
    }
}

double Lattice::measure()
{
  double M = 0;
  int i;

  for(i=0;i<N;i++) M+= s[i];
  M /= (double)N;
  return M;
}

double Lattice::random_uniform() 
{
  return rnd.Uniform(0,1);
}
