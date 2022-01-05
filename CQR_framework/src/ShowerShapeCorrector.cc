
#include <iostream>
#include <cstdlib>

#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/PythonParameterSet/interface/MakeParameterSets.h"

#include "ShowerShapeCorrection/CQR_framework/interface/ShowerShapeCorrector.h"

using namespace std;

ShowerShapeCorrector::ShowerShapeCorrector(string campaign, bool isMC):
    isMC_(isMC)
{
    string cmssw( getenv("CMSSW_BASE") );
    const edm::ParameterSet &process = edm::readPSetsFrom( cmssw + "/src/ShowerShapeCorrection/CQR_framework/python/flashgg_CQR_cfi.py" )
        ->getParameter<edm::ParameterSet>("process");

    ShowerShapes_ = {"r9", "s4", "sieie", "sieip", "etaWidth", "phiWidth", "esEnergyOverSCRawEnergy"};
    ECALpart_     = {"EB", "EE"};
    is_es_        = false;

    if isLoaded = false;
    for (const auto& i_ss : ShowerShapes_) {
        for (const auto& i_ep : ECALpart_) {
            
            if ( !process.exists( i_ss + "_corrector_config_"+ i_ep + "_" + campaign ) ) continue;
            isLoaded = true;
            if ( i_ss == "esEnergyOverSCRawEnergy" ) is_es_ = true;
            auto mva_config = process.getParameter<edm::ParameterSet>(i_ss + "_corrector_config_"+ i_ep + "_" + campaign);

            readers_[i_ss + "_" + i_ep]  = new TMVA::Reader( "!Color:Silent" );
            scalings_[i_ss + "_" + i_ep] = new TFormula("", mva_config.getParameter<string>("regr_output_scaling").c_str());
            mva_names_[i_ss + "_" + i_ep] = mva_config.getParameter<string>("classifier");
            cout << "Load : " << mva_config.getParameter<edm::FileInPath>("weights").fullPath() << endl;

            for (const auto& it_f : mva_config.getParameter<vector<string>>("features")) {
                vector<string> feas;
                split(feas, it_f, boost::is_any_of(":="));
                readers_[i_ss + "_" + i_ep]->AddVariable(feas[0].c_str(), &vars_[feas[1]]);
            }

            readers_[i_ss + "_" + i_ep]->BookMVA(mva_config.getParameter<string>("classifier"), mva_config.getParameter<edm::FileInPath>("weights").fullPath());

        }
    }

    if (!isLoaded )
    {
        char mesg[200];
        sprintf( mesg, "input campaign '%s' is invalid. Please check\n", campaign.c_str() );
        throw mesg;
    }

    ShowerShapes_.erase( ShowerShapes_.begin() + 6 );
}


ShowerShapeCorrector::~ShowerShapeCorrector()
{
    for (const auto& i_reader  : readers_) delete i_reader.second;
    for (const auto& i_scaling : scalings_) delete i_scaling.second;
}

vector<float> 
ShowerShapeCorrector::GetSSCorr()
{
    string ecalpart = fabs(vars_["etaSC"]) < 1.5 ? "EB" : "EE";

    vector<float> vars_corr;

    for (const auto& ss : ShowerShapes_)
        if (isMC_)
            vars_corr.push_back( vars_[ss] 
                    + scalings_[ss + "_" + ecalpart]->Eval( readers_[ss + "_" + ecalpart]->EvaluateRegression(mva_names_[ss + "_" + ecalpart])[0] ) 
                    );
        else
            vars_corr.push_back( vars_[ss] );

    if ( isMC_ && is_es_ && fabs(vars_["etaSC"]) > 1.653 )
        vars_corr.push_back( vars_["esEnergyOverSCRawEnergy"] + 
                             scalings_["esEnergyOverSCRawEnergy_EE"]->Eval( 
                                 readers_["esEnergyOverSCRawEnergy_EE"]->EvaluateRegression(mva_names_["esEnergyOverSCRawEnergy_EE"])[0] ) 
                             );
    else
        vars_corr.push_back( vars_["esEnergyOverSCRawEnergy"] );

    return vars_corr;

    // Information of vars_corr
    // vars_corr[0] = r9                       
    // vars_corr[1] = s4                       
    // vars_corr[2] = sieie                    
    // vars_corr[3] = sieip                    
    // vars_corr[4] = etaWidth                 
    // vars_corr[5] = phiWidth                 
    // vars_corr[6] = esEnergyOverSCRawEnergy  

}

void 
ShowerShapeCorrector::InputFeatures(
        float pt, 
        float etaSC, 
        float phi, 
        float rho,
        float r9, 
        float s4, 
        float sieie, 
        float sieip, 
        float etaWidth, 
        float phiWidth,
        float esEnergyOverSCRawEnergy
)
{
    vars_["pt"]                      = pt                     ;
    vars_["etaSC"]                   = etaSC                  ;
    vars_["phi"]                     = phi                    ;
    vars_["rho"]                     = rho                    ;
    vars_["r9"]                      = r9                     ;
    vars_["s4"]                      = s4                     ;
    vars_["sieie"]                   = sieie                  ;
    vars_["sieip"]                   = sieip                  ;
    vars_["etaWidth"]                = etaWidth               ;
    vars_["phiWidth"]                = phiWidth               ;
    vars_["esEnergyOverSCRawEnergy"] = esEnergyOverSCRawEnergy;
}
