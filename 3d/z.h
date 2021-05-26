//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jan 15 17:34:53 2013 by ROOT version 5.32/04
// from TTree T/data
// found on file: analyze.root
//////////////////////////////////////////////////////////

#ifndef z_h
#define z_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class z {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           k;
   Int_t           N;
   Int_t           Nsweep;
   Int_t           Nrelax;
   Int_t           Nmeasure;
   Int_t           flag_hot;
   Double_t        T;
   Double_t        J;
   Double_t        m_val[1000];   //[Nmeasure]
   Double_t        E_val[1000];   //[Nmeasure]

   // List of branches
   TBranch        *b_k;   //!
   TBranch        *b_N;   //!
   TBranch        *b_Nsweep;   //!
   TBranch        *b_Nrelax;   //!
   TBranch        *b_Nmeasure;   //!
   TBranch        *b_flag_hot;   //!
   TBranch        *b_T;   //!
   TBranch        *b_J;   //!
   TBranch        *b_m_val;   //!
   TBranch        *b_E_val;   //!

   z(TTree *tree=0);
   virtual ~z();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef z_cxx
z::z(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("analyze.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("analyze.root");
      }
      f->GetObject("T",tree);

   }
   Init(tree);
}

z::~z()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t z::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t z::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void z::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("k", &k, &b_k);
   fChain->SetBranchAddress("N", &N, &b_N);
   fChain->SetBranchAddress("Nsweep", &Nsweep, &b_Nsweep);
   fChain->SetBranchAddress("Nrelax", &Nrelax, &b_Nrelax);
   fChain->SetBranchAddress("Nmeasure", &Nmeasure, &b_Nmeasure);
   fChain->SetBranchAddress("flag_hot", &flag_hot, &b_flag_hot);
   fChain->SetBranchAddress("T", &T, &b_T);
   fChain->SetBranchAddress("J", &J, &b_J);
   fChain->SetBranchAddress("m_val", m_val, &b_m_val);
   fChain->SetBranchAddress("E_val", E_val, &b_E_val);
   Notify();
}

Bool_t z::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void z::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t z::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef z_cxx
