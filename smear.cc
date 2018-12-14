#include<iostream>
#include "TH1.h"
#include <fstream>
#include <TF1.h>

double smear (double P, int id) {

    double sigma, xmin, xmax;
    int N = 1;

    //ELETTRONI E MUONI
    if (id == 11 || id == 13) N = 100;
    //QUARKS
    if (id < 7) N = 10;

    //SMEARING SU P
    sigma = abs (P) / N; 
    
    //cosa faccio come xmin e xmax??
    xmin = P - 20*sigma;
    xmax = P + 20*sigma;
    
    TF1 *gaus = new TF1("P","1/([0]*sqrt(2*TMath::Pi()))*exp(-0.5*pow((x-[1])/[0],2))", xmin, xmax);
    gaus -> FixParameter (1, P);
    gaus -> FixParameter (0, sigma);

    P = gaus->GetRandom(xmin, xmax);

    delete gaus;

    return P;
}