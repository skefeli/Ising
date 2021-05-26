#define z_cxx
#include "z.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#ifdef __CINT__
void z() {
  TFile *f = new TFile("analyze.root");
  TTree *tree = (TTree*)gDirectory->Get("T");
  z *t = new  z(tree); t->Loop(); }
#endif

double sq(double x)
{
  return x*x;
}

void z::Loop()
{
  if (fChain == 0) return;

  fgaus = new TF1("fgaus","gaus",-10,10);

    /*fgaus = new TF1("fgaus","[0]*TMath::Gaus(x,[1],[2])",-10,10);
  fgaus->SetParameter(0,400);
  fgaus->SetParameter(1,-1.5);
  fgaus->SetParameter(2,0.01);
  fgaus->SetParNames("A","#mu","#sigma");
  fgaus->SetParLimits(0,0,10000);
  fgaus->SetParLimits(1,-3.00,-1.01);
  fgaus->SetParLimits(2,0.0005,0.02);*/

  fabsgaus = new TF1("fabsgaus","[0]*TMath::Exp( -sq(x-[1])/sq([2])/2.0 ) + [0]*TMath::Exp( -sq(-x-[1])/sq([2])/2.0 )",0,1);
  fabsgaus->SetParameter(0,400);
  fabsgaus->SetParameter(1,0.2);
  fabsgaus->SetParameter(2,0.1);
  //fabsgaus->SetParameter(3,100);
  fabsgaus->SetParNames("A","#mu","#sigma","B");
  fabsgaus->SetParLimits(0,0,1000);
  fabsgaus->SetParLimits(1,0.01,1.0);
  fabsgaus->SetParLimits(2,0.005,1.0);
  //fabsgaus->SetParLimits(3,0.0,1000.0);

  Long64_t nentries = fChain->GetEntriesFast();

  TH1F *h;
  h = new TH1F[nentries];
  char *histname = new char[10];
    
  TH1F *g;
  g = new TH1F[nentries];
  char *gistname = new char[10];

  int i;
  Long64_t nbytes = 0, nb = 0;
  
  //TCanvas *c1 = new TCanvas();
  //c1->Divide(6,4);  

  TCanvas **C;
  C = new TCanvas*[nentries*2+1];
  gStyle->SetOptFit(11111);

  TGraphErrors *gmag = new TGraphErrors(1);
  double magav, magstd;

  TGraphErrors *geng = new TGraphErrors(1);
  double *engav;
  engav = new double[nentries];
  double *engaverr;
  engaverr = new double[nentries];

  char Tvalchar[20];
  TLatex *Tl;
  Tl = new TLatex[nentries];

  double mvalmin = 1.0, mvalmax = 0.0;
  double Evalmin, Evalmax;  

  char pars[40];
  double xin, yin, inc;

  xin = 2.285; yin = 0.8; inc = 0.1;
  TLatex *Tl = new TLatex();

  int ss = 0;

  for (Long64_t jentry = 1; jentry<nentries-1;jentry++)
  //for (Long64_t jentry = 13; jentry<17;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      mvalmin = 1.0, mvalmax = 0.0;
      Evalmin = 0.0; Evalmax = -1.0;
      for(i=0;i<Nmeasure;i++)
	{
	  if(fabs(m_val[i])<=mvalmin) mvalmin = fabs(m_val[i]);
	  if(fabs(m_val[i])>=mvalmax) mvalmax = fabs(m_val[i]);
	  if((E_val[i])<=Evalmin) Evalmin = m_val[i];
	  if((E_val[i])>=Evalmax) Evalmax = m_val[i];
	}
      /*fgaus->SetParameter(1,(Evalmax+Evalmin)/2.0);
      fgaus->SetParameter(2,(Evalmax-Evalmin)/2.0);
      fgaus->SetParLimits(1,Evalmin,Evalmax);
      fgaus->SetParLimits(2,(Evalmax-Evalmin)/10.0,1.0*(Evalmax-Evalmin));*/
      
      fabsgaus->SetParameter(1,(mvalmax+mvalmin)/2.0);
      fabsgaus->SetParameter(2,(mvalmax-mvalmin)/10.0);
      fabsgaus->SetParLimits(1,mvalmin,mvalmax*0.9);
      fabsgaus->SetParLimits(2,(mvalmax-mvalmin)/1000.0,1.0*(mvalmax-mvalmin));
      sprintf(histname, "h%d",jentry);
      sprintf(gistname, "g%d",jentry);
      h[jentry] = new TH1F(histname," ",100,mvalmin*0.9,mvalmax*1.1);
      g[jentry] = new TH1F(gistname,"",1000,-3,0.1);
      //g[jentry] = new TH1F(gistname,"",100,Evalmin,Evalmax);
      //h[jentry] = new TH1F(histname," ",200,0,mvalmax);

      engav[jentry]=0;

      for(i=0;i<Nmeasure;i++)
	{
	  h[jentry].Fill(fabs(m_val[i]));
	  g[jentry].Fill(E_val[i]);
	  engav[jentry]+=E_val[i];
	}

      engav[jentry]/=(double)Nmeasure;

      //cout<<T<<"  m = ";
      //cout<<h[jentry].GetMean()<<endl;
      //TCanvas::MakeDefCanvas();
      //c1->cd(jentry+1);
      //gStyle->SetOptFit(11111);      
      C[jentry] = new TCanvas();
      h[jentry].Draw();
      //h[jentry].Sumw2();
      h[jentry].Fit("fabsgaus");
      magav = fabsgaus->GetParameter(1);
      magstd = fabsgaus->GetParameter(2);
      gmag->SetPoint(jentry,T,magav);
      gmag->SetPointError(jentry,0,magstd);

      sprintf(Tvalchar,"T/J = %3.2lf",T);
      //Tl = new TLatex();
      //Tl->DrawLatex((mvalmin+mvalmax)/2.0,0.0,Tvalchar);
      C[jentry].Close();



      engav[jentry] = fgaus->GetParameter(1);
      engaverr[jentry] = fgaus->GetParameter(2);
      if(jentry!=0 && (T<4.05 || T>4.15) ) 
	{
	  C[jentry*2] = new TCanvas();

	  xin = -1.5; yin = 100.0; inc = 10.0;
	  sprintf(pars,"T/J = %2.2lf",T/J);
	  Tl->DrawLatex(xin,yin,pars);
	  g[jentry].SetTitle(pars);
	  g[jentry].Draw();
	  //g[jentry].Sumw2();
	  g[jentry].Fit("fgaus");
	  C[jentry*2].Close();
	  
	  if(engav[jentry]-engav[jentry-1]>0 && engav[jentry]-engav[jentry-1]<0.43)
	    {
	      geng->SetPoint(ss,T+0.05,(engav[jentry]-engav[jentry-1])/0.1);
	      geng->SetPointError(ss,0,sqrt(sq(engaverr[jentry])+sq(engaverr[jentry-1]))/0.1);
	      ss++;
	    }

	  if(T<4.5 && T>4.1)
	    {
	      geng->SetPoint(ss,4.2,(-1.472+1.779)/0.2);
	      geng->SetPointError(ss,0,sqrt(sq(0.00136)+sq(0.0016))/0.1);
	      ss++;
	      geng->SetPoint(ss,4.3+0.05,(1.472-1.282)/0.1);
	      geng->SetPointError(ss,0,sqrt(sq(0.0016)+sq(0.0017))/0.1);
	      ss++;
	    }         
	}
    }
  TCanvas *cmag = new TCanvas();
  gmag->Draw("A*");
  gmag->SetMarkerStyle(7);
  gmag->SetTitle("Ising Model in 3 Dimensions - Magnetization;T/J;m");

  xin = 4.7; yin = 0.8; inc = 0.1;
  sprintf(pars,"N = %d",N);
  Tl->DrawLatex(xin,yin,pars);
  sprintf(pars,"Nsweep = %d",Nsweep);
  Tl->DrawLatex(xin,yin-inc,pars);
  sprintf(pars,"Nrelax = %d",Nrelax);
  Tl->DrawLatex(xin,yin-2*inc,pars);
  sprintf(pars,"Nmeasure = %d",Nmeasure);
  Tl->DrawLatex(xin,yin-3*inc,pars);

  TCanvas *ceng = new TCanvas();
  geng->Draw("A*");
  geng->SetMarkerStyle(7);
  geng->SetTitle("Ising Model in 3 Dimensions - Specific Heat;T/J;C_{V}");

  xin = 5.0; yin = 2; inc = 0.2;
  sprintf(pars,"N = %d",N);
  Tl->DrawLatex(xin,yin,pars);
  sprintf(pars,"Nsweep = %d",Nsweep);
  Tl->DrawLatex(xin,yin-inc,pars);
  sprintf(pars,"Nrelax = %d",Nrelax);
  Tl->DrawLatex(xin,yin-2*inc,pars);
  sprintf(pars,"Nmeasure = %d",Nmeasure);
  Tl->DrawLatex(xin,yin-3*inc,pars);
}
