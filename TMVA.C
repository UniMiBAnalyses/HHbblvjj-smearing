#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"
#include <TMVA/Config.h>

using namespace std;

int TMVATrain(){
  //this loads the library
  TMVA::Tools::Instance();

  TFile *sinput = TFile::Open("HH_cutF.root");
  TFile *b1input = TFile::Open("ttbar_cutF.root");
  TFile *b2input = TFile::Open("WWZ_cutF.root");
  TFile *b3input = TFile::Open("WZZ_cutF.root");  

  TTree *signal = (TTree*)sinput->Get("tree");
  TTree *bg1 = (TTree*)b1input->Get("tree");
  TTree *bg2 = (TTree*)b2input->Get("tree");
  TTree *bg3 = (TTree*)b3input->Get("tree");

  TString outfileName ("no_smear.root");
  TFile *outputFile = TFile::Open(outfileName, "RECREATE");

  TMVA::Factory * factory = new TMVA::Factory 
  (
    "TMVAClassification", 
    outputFile,                                           
    "!V:!Silent:Color:DrawProgressBar:Transformations=I;P;G:AnalysisType=Classification" 
  ) ;
  
  //Define the input variables that shall be used for the MVA training
  TMVA::DataLoader * dataloader = new TMVA::DataLoader("dataset");
  
  //MOMENTI TRASVERSI
  // dataloader->AddVariable("lep_F->Pt()", 'F');
  dataloader->AddVariable("bb_F->Pt()", 'F');
  dataloader->AddVariable("WW_F->Pt()", 'F');
  dataloader->AddVariable("tot_F->Pt()", 'F');
  // dataloader->AddVariable("MET_F->Pt()", 'F');

  //MASSE
  // dataloader->AddVariable("lep_F->M()", 'F');
  // dataloader->AddVariable("bb_F->M()", 'F');
  dataloader->AddVariable("WW_F->M()", 'F');
  dataloader->AddVariable("tot_F->M()", 'F');
  // dataloader->AddVariable("MET_F->M()", 'F');
  dataloader->AddVariable("jj_F->M()", 'F');

  //ENERGIE
  // dataloader->AddVariable("lep_F->E()", 'F');
  dataloader->AddVariable("bb_F->E()", 'F');
  // dataloader->AddVariable("WW_F->E()", 'F');
  // dataloader->AddVariable("tot_F->E()", 'F');
  // dataloader->AddVariable("MET_F->E()", 'F');

  //VARIABILI ANGOLARI
  dataloader->AddVariable("ljj_Phi", 'F');
  dataloader->AddVariable("ljj_R", 'F');
  dataloader->AddVariable("bbljj_Phi", 'F');
  dataloader->AddVariable("bbljj_R", 'F');
  dataloader->AddVariable("bb_Phi", 'F');
  dataloader->AddVariable("bb_R", 'F');

  dataloader->AddSignalTree (signal, 1.);
  dataloader->AddBackgroundTree (bg1, 1.);
  dataloader->AddBackgroundTree (bg2, 1.);
  dataloader->AddBackgroundTree (bg3, 1.);
  
  TCut mycuts = "ljj_R>0";
  TCut mycutb = mycuts;

  dataloader->PrepareTrainingAndTestTree
  ( 
    mycuts, 
    mycutb,
    "SplitMode=Random:NormMode=NumEvents:!V"
  );

  // Monte Carlo (MC)
  // factory->BookMethod
  // (
  //   dataloader,
  //   TMVA::Types::kCuts,
  //   "Monte Carlo",
  //   "!H:!V:FitMethod=MC:EffSel:SampleSize=100000:VarProp=FSmart"
  // ) ;

  //Genetic Algorithm (GA)
  factory->BookMethod
  (
    dataloader,
    TMVA::Types::kCuts,
    "Genetic algorithm",
    "!H:!V:FitMethod=GA:VarProp=FSmart:PopSize=1:Cycles=1:Steps=1"
  ) ;

  factory->TrainAllMethods ();
  factory->TestAllMethods ();
  factory->EvaluateAllMethods ();
  
  outputFile->Close();
  delete factory;
  delete dataloader;

  if (!gROOT->IsBatch()) TMVA::TMVAGui(outfileName);
    
  return 0;
}


int main(int argc, char ** argv){

    return TMVATrain();
    
}
