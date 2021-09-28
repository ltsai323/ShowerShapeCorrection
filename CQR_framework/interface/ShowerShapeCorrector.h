#ifndef __SHOWERSHAPECORRECTOR__
#define __SHOWERSHAPECORRECTOR__

#include <string>
#include <vector>
#include <map>

#include "TMVA/Reader.h"
#include "TFormula.h"

class ShowerShapeCorrector{

    public:

        ShowerShapeCorrector(std::string campaign, bool isMC = true);
        ~ShowerShapeCorrector();

        void InputFeatures(float pt, float etaSC, float phi, float rho, float r9, float s4, float sieie, float sieip, float etaWidth, float phiWidth, float esEnergyOverSCRawEnergy);

        std::vector<float> GetSSCorr();


    private:

        bool isMC_;
        std::vector<std::string> ShowerShapes_;
        std::vector<std::string> ECALpart_;
        bool is_es_;

        std::map<std::string, float> vars_;
        std::map<std::string, TMVA::Reader*> readers_;
        std::map<std::string, TFormula*> scalings_;
        std::map<std::string, std::string> mva_names_;

};

#endif
