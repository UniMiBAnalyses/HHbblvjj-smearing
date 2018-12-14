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

int main(int argc, char** argv){
    if (argc < 1){
        cout << "Usage: " << argv[0] << " ./HH.root " << endl;
        return 1;
    }

	TApplication * Grafica = new TApplication("App", 0, 0);

    //Lettura del TTree
    TFile * input = TFile::Open ("HH.root");
    TTree * tree  = (TTree*)input->Get("tree");
    int N = 0;

    DrawHisto(tree);

    Grafica->Run();
	return 0;
}