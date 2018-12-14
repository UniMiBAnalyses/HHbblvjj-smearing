#include <iostream>
#include "TH1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include <TStyle.h>
#include <TMath.h>
#include <TF1.h>
#include <TLegend.h>
#include <THStack.h>
#include <TApplication.h>
#include <fstream>
#include "delta.h"
#include "TLorentzVector.h"
#include "TString.h"
#include <algorithm>
#include <TRandom.h>

using namespace std;

void DrawHisto	(TTree *tree){	
	
	TTreeReader reader (tree);

	TTreeReaderValue<TLorentzVector> var_I(reader, "MET_I"); 
	TTreeReaderValue<TLorentzVector> var_F(reader, "MET_F");

	int nbin = 100;
	double min = -1.5, max = 1.5;
	double prima = 0, dopo = 0, delta = 0;
	string title = "delta";

	TCanvas *can = new TCanvas("can", "Confronto tra MET_{P}");
 	TH1F *h = new TH1F( title.c_str(), "Confronto tra MET_{P}", nbin, min, max); 
	
	//SCRITTURA ISTOGRAMMA DISTRIBUZIONE
	while ( reader.Next() ) {
		prima = var_I->P();
		dopo = var_F->P();
		delta = (dopo - prima) / prima;
		h->Fill (delta);
	}
	
	gStyle->SetOptStat(0);
	h -> Draw();
	
	TF1 *fit = new TF1("gaus", "gaus(0)", min, max);
	fit -> SetParameter(1, 0.);
	fit -> SetParameter(2, 0.2);

	h -> Fit(fit);

	cout << "media = " << fit -> GetParameter(1) << endl;
	cout << "sigma = " << fit -> GetParameter(2) << endl;


	return;
}

