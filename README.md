# ShowerShapeCorrection

This shower shape correction is based on flashgg framework https://github.com/cms-analysis/flashgg under cmssw.

## Install
```
cmsrel CMSSW_10_X_X

cd CMSSW_10_X_X/src

git clone https://github.com/youyingli/ShowerShapeCorrection.git

scram b -j 4
```

## Usage
### Inport the shower shape corrector
Include the following header file in your C++ codes
```
#include "ShowerShapeCorrection/CQR_framework/interface/ShowerShapeCorrector.h"
```
and add the following into your BuildFile.xml
```
<use name="ShowerShapeCorrection/CQR_framework"/>
```

### Example codes in C++

The campaign can be 2016, 2017, 2018, 2017_Legacy, 2018_Legacy. And note that there is a flag to identify if the input is MC or data. (There is no correction for data.)

```cpp
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

```
