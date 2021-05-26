void createTree()
{
  TFile *f;
  TTree *tree;
  ifstream ifs("datafile.dat");

  int N, Nsweep, Nrelax, Nmeasure, flag_hot;
  double T, J;  
  double *m_val; // array for magnetization
  double *E_val; // array for energy
  f = new TFile("analyze.root","RECREATE");
  tree = new TTree("T","data");

  int i;
  
  int q;

  ifs>>N>>Nsweep>>Nrelax>>Nmeasure>>flag_hot>>T>>J;
  m_val = new double[Nmeasure];
  E_val = new double[Nmeasure];
  for(i=0;i<Nmeasure;i++)
    {
      ifs>>q;
      ifs>>m_val[i];
      ifs>>E_val[i];
    }

  int k;
  tree->Branch("k",&k,"k/I");
  tree->Branch("N",&N,"N/I");
  tree->Branch("Nsweep",&Nsweep,"Nsweep/I");
  tree->Branch("Nrelax",&Nrelax,"Nrelax/I");
  tree->Branch("Nmeasure",&Nmeasure,"Nmeasure/I");
  tree->Branch("flag_hot",&flag_hot,"flag_hot/I");
  tree->Branch("T",&T,"T/D");
  tree->Branch("J",&J,"J/D");
  tree->Branch("m_val",m_val,"m_val[Nmeasure]/D");
  tree->Branch("E_val",E_val,"E_val[Nmeasure]/D");

  tree->Fill();
  
  k=0;
  k++;
  while(!ifs.eof())
    {
      //if(k==9) break;
      ifs>>N>>Nsweep>>Nrelax>>Nmeasure>>flag_hot>>T>>J;
      //cout<<N<<Nsweep<<Nrelax<<Nmeasure<<flag_hot<<T<<J<<endl;
      for(i=0;i<Nmeasure;i++)
	{
	  ifs>>q;
	  ifs>>m_val[i];
	  //cout<<q<<" "<<m_val[i]<<endl;
	  ifs>>E_val[i];
	}
      cout<<T<<endl;
      //cout<<k<<endl;
      tree->Fill();
      //cout<<k<<endl;
      k++;
    }
  //tree->Print();
  f->Write();
}
