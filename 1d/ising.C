#include <stdio.h>
#include <math.h>
#include <TGraph.h>
#include <TF1.h>
#include <TCanvas.h>

using namespace std;

//g++ -I.. `root-config --cflags --libs` ising.C

TRandom3 *rnd = new TRandom3(time(0));

double exact_curve(double B, double T, double J)
{
  return sinh(B/T)/sqrt(exp(-4*J/T)+sinh(B/T)*sinh(B/T));
}

double random_uniform(double xmin, double xmax) 
{
  return rnd->Uniform(xmin,xmax);
}

double measure(int N, int *s)
{
  double M = 0;
  int i;

  for(i=0;i<N;i++) M+= s[i];

  M /= (double)N;
  return M;
}

void sweep(double J, double B, double T, int *s, int N, int Nsweep)
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
	      r = random_uniform(0,1);
	      if(g>r) s[i] = -s[i];
	    }
	}
    }
}

void experiment(double J, double B, double T,
		int N, int Nsweep, int Nrelax, int Nmeasure,
		double *M, double *sigma)
{
  int i, *s;
  double sum, sum2, M1, q;

  s = (int *)malloc(N*sizeof(int));

  for(i=0; i<N;i++)
    {
      q = rnd ->Integer(2);
      if(q==0) q=-1;
      s[i] = q;
    }

  //relaxation
  for(i=0;i<Nrelax;i++)sweep(J, B, T, s, N, Nsweep);
  
  sum = 0.0;
  sum2 = 0.0;

  for(i=0;i<Nmeasure;i++)
    {
      sweep(J, B, T, s, N, Nsweep);
      M1 = measure(N,s);
      sum += M1;
      sum2 += M1*M1;
    }
  
  *M = sum / (double)Nmeasure;
  *sigma = sqrt(sum2 / (double)Nmeasure - (*M)*(*M))/sqrt(Nmeasure);

  free(s);

}

void ising()
{  
  int N, Nsweep, Nrelax, Nmeasure;
  double J,B,T,M,sigma;
  double sum, M1;
  int *s;
  int i;

  TGraphErrors *graph = new TGraphErrors(1);

  N = 256;
  Nsweep = 50;
  Nrelax = 20;
  Nmeasure = 50;
  J = 1.0;
  T = 2.0;


  printf("N = %d   Nswepp = %d   Nrelax = %d   Nmeasure = %d\n",
	 N, Nsweep, Nrelax, Nmeasure);
  
  int n = 0;

  //for(T=1.0;T<1.1;T+=1)
    {
      printf("T = %.11f K\n",T);
      printf("    B        M     sigma\n");
      for(B=-1.0;B<1.01;B+=0.1)
	{
	  experiment(J,B,T,N,Nsweep,Nrelax,Nmeasure,&M,&sigma);
	  printf("%5.3lf %8.3lf %8.3lf\n", B, M, sigma);
	  graph->SetPoint(n,B,M);
	  graph->SetPointError(n,0,sigma);
	  n++;
	}
    }
  
  TF1 *f1 = new TF1("func1","exact_curve(x,[0],[1])",-1.5,1.5);
  f1->SetParameters(1.0,1.0);
  TF1 *f2 = new TF1("func2","exact_curve(x,[0],[1])",-1.5,1.5);
  f2->SetParameters(2.0,1.0);
  f1->SetLineColor(kRed);
  f2->SetLineColor(kRed);
  graph->SetTitle("Ising model in 1 Dimension");
  graph->GetXaxis()->SetTitle("B/k");
  graph->GetYaxis()->SetTitle("M/N");

  TCanvas *c1 = new TCanvas();
  graph->Draw("A*");
  //f1->Draw("same");
  f2->Draw("same");
  //c1->Print("projec_2.pdf");
  
  //fclose(afile);

  //graph->SetMarkerStyle(6);

  char pars[40];
  TLatex *Tl = new TLatex();
  sprintf(pars,"N = %d",N);
  Tl->DrawLatex(0.50,0.40,pars);
  sprintf(pars,"Nsweep = %d",Nsweep);
  Tl->DrawLatex(0.50,0.20,pars);
  sprintf(pars,"Nrelax = %d",Nrelax);
  Tl->DrawLatex(0.50,0.0,pars);
  sprintf(pars,"Nmeasure = %d",Nmeasure);
  Tl->DrawLatex(0.50,-0.20,pars);
  sprintf(pars,"T/J = %d",T/J);
  Tl->DrawLatex(0.50,-0.40,pars);
 
  TLegend *leg = new TLegend(0.65,0.10,0.90,0.30);
  leg->AddEntry(graph,"Experiment","p");  leg->Draw();
  leg->AddEntry(f1,"Theory","l");  leg->Draw();


}

#if !defined(__CINT__) || defined(__MAKECINT__)
int main()
{
  ising();
  return 0;
}
#endif
