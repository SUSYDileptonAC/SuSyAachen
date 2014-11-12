import FWCore.ParameterSet.Config as cms
from SuSyAachen.DiLeptonHistograms.pdgIdDefinition_cff import defaultPdgIdDefinition
from SuSyAachen.DiLeptonHistograms.vertexWeights_cfi import vertexWeights as vertexWeightPars

from SuSyAachen.DiLeptonHistograms.efficiencies.electronEffPSet_cff import electronCenterEfficiencies as electronEfficiency
from SuSyAachen.DiLeptonHistograms.efficiencies.muonEffPSet_cff import muonCenterEfficiencies as muonEfficiency
from SuSyAachen.TagAndProbeTreeWriter.isolationFunctor_cfi import isolationDefinitions

DiLeptonTriggerTreesNoTaus = cms.EDAnalyzer("DiLeptonTriggerTrees",
   electrons = cms.InputTag("triggerMatchedPatElectronsPF"),
   muons = cms.InputTag("triggerMatchedPatMuonsPF"),
#   taus = cms.InputTag("triggerMatchedPatTausPF"),
   jets = cms.InputTag("triggerMatchedPatJetsPF"),	   	   
   bJets = cms.InputTag("triggerMatchedPatJetsPF"),
   met = cms.InputTag("patMETsPF"),  	   
   tcmet = cms.InputTag("patMETsTC"), 
   type1met = cms.InputTag("pfType1CorrectedMet"),
   caloMet = cms.InputTag("patMETsAK5Calo"),
   genMetTrue = cms.InputTag("genMetTrue"),	   	   
   vertices = cms.InputTag("offlinePrimaryVertices"),
   tauId = cms.string("byTaNCfrHalfPercent"),
   pfCands = cms.InputTag("particleFlow"),
   genParticles = cms.InputTag("genParticles"),
   triggerSummaryTag = cms.InputTag("hltTriggerSummaryAOD","","HLT"),	   
   susyVars = cms.VPSet(),
   pdfWeightTags = cms.VInputTag(),
   vertexWeights = vertexWeightPars,
   pdgIdDefinition = defaultPdgIdDefinition,
   isolationDefinitions = isolationDefinitions,
   NOelectronCorrections = cms.VPSet(
    cms.PSet(
    absEta = cms.double(0.4),
    correction = cms.double(1.00594),
    ),
    cms.PSet(
    absEta = cms.double(0.8),
    correction = cms.double(1.0082),
    ),
    cms.PSet(
    absEta = cms.double(1.2),
    correction = cms.double(1.01092),
    ),
    cms.PSet(
    absEta = cms.double(1.5),
    correction = cms.double(1.01659),
    ),
    cms.PSet(
    absEta = cms.double(2.),
    correction = cms.double(1.01271),
    ),
    cms.PSet(
    absEta = cms.double(2.5),
    correction = cms.double(1.02727),
    ),

    ),                                  

# NOT USED RIGHT NOW
   fakeRates =  cms.PSet(
        electrons = cms.VPSet(),
        muons =  cms.VPSet(),
        taus = cms.VPSet()
   ),
   efficiencies =  cms.PSet(
        electrons = electronEfficiency,
        muons =  muonEfficiency,
        taus = cms.VPSet()
   )
#        cms.VPSet(
#            cms.PSet(
#                etaMin = cms.double(-2.4), etaMax = cms.double(-1.3),
#                ptMin = cms.double(0), ptMax = cms.double(10),
#                
#                weight = cms.double(0.5)
#            ),
#            cms.PSet(
#                etaMin = cms.double(-1.3),  etaMax = cms.double(1.3),
#                ptMin = cms.double(0), ptMax = cms.double(10),
#    
#                weight = cms.double(0.2)
#            ),
#        )
#    )

)

DiLeptonTriggerTrees = DiLeptonTriggerTreesNoTaus.clone(
    taus = cms.InputTag("triggerMatchedPatTausPF"),
    tauId = cms.string("byTaNCfrHalfPercent"), 
    )

DiLeptonTriggerTreesForLMPoints = DiLeptonTriggerTrees.clone(
          pdfWeightTags = cms.VInputTag(
#            "susyScanPdfWeights:cteq66",
#            "susyScanPdfWeights:MSTW2008nlo68cl",
#            "susyScanPdfWeights:NNPDF20"
            "susyScanPdfWeights:cteq66",
            "susyScanPdfWeights:MRST2006nnlo",
            "susyScanPdfWeights:NNPDF10"
            )
          )

mSugraVars =  [
    cms.PSet(var = cms.InputTag("susycafscan","susyScanA0"), type = cms.string("float")),
    cms.PSet(var = cms.InputTag("susycafscan","susyScanM0"), type = cms.string("float")),
    cms.PSet(var = cms.InputTag("susycafscan","susyScanM12"), type = cms.string("float")),
    cms.PSet(var = cms.InputTag("susycafscan","susyScantanbeta"), type = cms.string("float")),
      #       cms.PSet(var = cms.InputTag("susyScanRun"), type = cms.string("float")),
    cms.PSet(var = cms.InputTag("susycafscan","susyScanMu"), type = cms.string("int"))
    ]

xSecVars = [
    cms.PSet(var = cms.InputTag("susycafscan","susyScanLOXSection"), type = cms.string("float")),
    cms.PSet(var = cms.InputTag("susycafscan","susyScanGenFilterEfficiency"), type = cms.string("float")),
    cms.PSet(var = cms.InputTag("susyScanNLOCrossSection"), type = cms.string("float")),
    cms.PSet(var = cms.InputTag("susyScanNLOCrossSectionScale2"), type = cms.string("float")),
    cms.PSet(var = cms.InputTag("susyScanNLOCrossSectionScale05"), type = cms.string("float")),
    cms.PSet(var = cms.InputTag("susyScankFactor"), type = cms.string("float")),
]

DiLeptonTriggerTreesmSugra = DiLeptonTriggerTrees.clone(
   susyVars = cms.VPSet( mSugraVars + xSecVars),
   pdfWeightTags = cms.VInputTag(
        "susyScanPdfWeights:cteq66",
#        "susyScanPdfWeights:MSTW2008nlo68cl",
#        "susyScanPdfWeights:NNPDF20"
#        "susyScanPdfWeights:MRST2006nnlo",
#        "susyScanPdfWeights:NNPDF10"
       )
)

simplifedVars = cms.VPSet(
        cms.PSet(var = cms.InputTag("susycafscan","susyScanMLSP"), type = cms.string("float")),
            cms.PSet(var = cms.InputTag("susycafscan","susyScanMGL"), type = cms.string("float")),
        #    cms.PSet(var = cms.InputTag("seqSimplifiedPars","susyScanLower"), type = cms.string("float")),
        #    cms.PSet(var = cms.InputTag("seqSimplifiedPars","susyScanHigher"), type = cms.string("float")),
            cms.PSet(var = cms.InputTag("susycafscan","susyScanXChi"), type = cms.string("float")),
        #    cms.PSet(var = cms.InputTag("seqSUSYPARS","susyScanCrossSection"), type = cms.string("float")),
        #    cms.PSet(var = cms.InputTag("susyScanNLOCrossSection"), type = cms.string("float")),
        #                  cms.PSet(var = cms.InputTag("susyScanNLOCrossSectionScale2"), type = cms.string("float")),
        #    cms.PSet(var = cms.InputTag("susyScanNLOCrossSectionScale05"), type = cms.string("float")),
        #    cms.PSet(var = cms.InputTag("susyScankFactor"), type = cms.string("float")),
            #       cms.PSet(var = cms.InputTag("susyScanRun"), type = cms.string("float")),
        #    cms.PSet(var = cms.InputTag("seqSUSYPARS","susyScanMu"), type = cms.string("int"))
            )

DiLeptonTriggerTreesSimplified = DiLeptonTriggerTrees.clone(
#    vertexWeights = vertexWeightPars.clone(doWeight = False)
    susyVars = simplifedVars,
    pdfWeightTags = cms.VInputTag(
        "susyScanPdfWeights:cteq66",
#        "susyScanPdfWeights:MSTW2008nlo68cl",
#        "susyScanPdfWeights:NNPDF20"
#    "susyScanPdfWeights:MRST2006nnlo",
#    "susyScanPdfWeights:NNPDF10"
    )
    )
DiLeptonTriggerTreesSimplified.vertexWeights.doWeight=False