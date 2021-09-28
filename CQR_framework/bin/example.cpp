#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include "ShowerShapeCorrection/CQR_framework/interface/ShowerShapeCorrector.h"

using namespace std;

int main(){

    float pt                       ;
    float etaSC                    ;
    float phi                      ;
    float rho                      ;
    float r9                       ;
    float s4                       ;
    float sieie                    ;
    float sieip                    ;
    float etaWidth                 ;
    float phiWidth                 ;
    float esEnergyOverSCRawEnergy  ;


    TFile* file = TFile::Open("");
    TTree* tree = (TTree*) file->Get("");

    tree->SetBranchAddress("pt"                      ,&pt                                 );
    tree->SetBranchAddress("etaSC"                   ,&etaSC                              );
    tree->SetBranchAddress("phi"                     ,&phi                                );
    tree->SetBranchAddress("rho"                     ,&rho                                );
    tree->SetBranchAddress("r9"                      ,&r9                                 );
    tree->SetBranchAddress("s4"                      ,&s4                                 );
    tree->SetBranchAddress("sieie"                   ,&sieie                              );
    tree->SetBranchAddress("sieip"                   ,&sieip                              );
    tree->SetBranchAddress("etaWidth"                ,&etaWidth                           );
    tree->SetBranchAddress("phiWidth"                ,&phiWidth                           );
    tree->SetBranchAddress("esEnergyOverSCRawEnergy" ,&esEnergyOverSCRawEnergy            );

    //Campaign {2016, 2017, 2018, 2017_Legacy, 2018_Legacy}
    string camp = "2018_Legacy";
    bool isMC = true;

    ShowerShapeCorrector sscorr(camp, isMC);

    for (int i = 0; i < 1000; i++) {
        tree->GetEntry(i);
        
        sscorr.InputFeatures(
                    pt,                       
                    etaSC,                    
                    phi,                      
                    rho,                      
                    r9,                       
                    s4,                       
                    sieie,                    
                    sieip,                    
                    etaWidth,                 
                    phiWidth,                 
                    esEnergyOverSCRawEnergy  
                );

        auto var_corr = sscorr.GetSSCorr();

        cout << "r9                       --> Uncorrected " << r9                      << ", Corrected : " << var_corr[0] << endl; 
        cout << "s4                       --> Uncorrected " << s4                      << ", Corrected : " << var_corr[1] << endl; 
        cout << "sieie                    --> Uncorrected " << sieie                   << ", Corrected : " << var_corr[2] << endl; 
        cout << "sieip                    --> Uncorrected " << sieip                   << ", Corrected : " << var_corr[3] << endl; 
        cout << "etaWidth                 --> Uncorrected " << etaWidth                << ", Corrected : " << var_corr[4] << endl; 
        cout << "phiWidth                 --> Uncorrected " << phiWidth                << ", Corrected : " << var_corr[5] << endl; 
        cout << "esEnergyOverSCRawEnergy  --> Uncorrected " << esEnergyOverSCRawEnergy << ", Corrected : " << var_corr[6] << endl; 
    }
}
