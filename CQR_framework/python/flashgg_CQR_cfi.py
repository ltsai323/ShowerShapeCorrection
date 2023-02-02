import FWCore.ParameterSet.Config as cms

import json
import os


process = cms.PSet()
campaign = ['2016', '2017', '2018', '2017_Legacy', '2018_Legacy', '2016_Legacy_preVFP', '2016_Legacy_postVFP']

features = {}

features['2016'] = [
    "f0=pt",
    "f1=etaSC",
    "f2=phi",
    "f3=rho",
    "f4=phiWidth",
    "f5=sieip",
    "f6=s4",
    "f7=r9",
    "f8=sieie",
    "f9=etaWidth"
]

features['2017'] = [
    "f0=pt",
    "f1=etaSC",
    "f2=phi",
    "f3=rho",
    "f4=sieip",
    "f5=s4",
    "f6=r9",
    "f7=phiWidth",
    "f8=sieie",
    "f9=etaWidth"
]

features['2018'] = [
    "f0=pt",
    "f1=etaSC",
    "f2=phi",
    "f3=rho",
    "f4=sieip",
    "f5=s4",
    "f6=r9",
    "f7=phiWidth",
    "f8=sieie",
    "f9=etaWidth"
]

features['2017_Legacy'] = [
    "f0=pt",
    "f1=etaSC",
    "f2=phi",
    "f3=rho",
    "f4=sieip",
    "f5=s4",
    "f6=r9",
    "f7=phiWidth",
    "f8=sieie",
    "f9=etaWidth"
]

features['2018_Legacy'] = [
    "f0=pt",
    "f1=etaSC",
    "f2=phi",
    "f3=rho",
    "f4=sieip",
    "f5=s4",
    "f6=r9",
    "f7=phiWidth",
    "f8=sieie",
    "f9=etaWidth"
]

features['2016_Legacy_preVFP'] = [
    "f0=pt",
    "f1=etaSC",
    "f2=phi",
    "f3=rho",
    "f4=sieip",
    "f5=s4",
    "f6=r9",
    "f7=phiWidth",
    "f8=sieie",
    "f9=etaWidth"
]

features['2016_Legacy_postVFP'] = [
    "f0=pt",
    "f1=etaSC",
    "f2=phi",
    "f3=rho",
    "f4=sieip",
    "f5=s4",
    "f6=r9",
    "f7=phiWidth",
    "f8=sieie",
    "f9=etaWidth"
]

for i_camp in campaign:

    corrections_summary = {}
    with open(os.path.expandvars('$CMSSW_BASE/src/ShowerShapeCorrection/CQR_framework/data/corrections_summary_%s.json' % i_camp)) as json_file:
        corrections_summary = json.load(json_file)

    #---Shower shapes
    for var in corrections_summary['shower_shapes'].keys():
        for subdet in ['EB', 'EE']:
            ss_summary = corrections_summary['shower_shapes'][var][subdet]
            setattr(process, var+'_corrector_config_'+subdet+'_'+i_camp,
                    cms.PSet(
                        weights = cms.FileInPath(str(ss_summary['weights'])),
                        regr_output_scaling = cms.string('x[0]*(%.10f)+(%.10f)' % (ss_summary['scale'], ss_summary['center'])),
                        classifier = cms.string('BDTG_{}_{}'.format(var, subdet)),
                        features = cms.vstring( features[i_camp] )
                    )
            )

    #---Preshower
    # Runs only for EE if "preshower" key is present in the metaconditions
    if 'preshower' in corrections_summary:
        features[i_camp].append("f10=esEnergyOverSCRawEnergy")

        for var in corrections_summary['preshower'].keys():
            ss_summary = corrections_summary['preshower'][var]
            setattr(process, var+'_corrector_config_EE_'+i_camp,
                    cms.PSet(
                        weights = cms.FileInPath(str(ss_summary['weights'])),
                        regr_output_scaling = cms.string('x[0]*(%.10f)+(%.10f)' % (ss_summary['scale'], ss_summary['center'])),
                        classifier = cms.string('BDTG_{}_EE'.format(var)),
                        features = cms.vstring( features[i_camp] )
                    )
            )
