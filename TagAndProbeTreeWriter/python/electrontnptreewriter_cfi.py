import FWCore.ParameterSet.Config as cms

electrontnptreewriter = cms.EDAnalyzer('ElectronTnPTreeWriter',
mcInfoAvailable = cms.untracked.bool(False),

mcSource = cms.InputTag("genParticles"),
tagSource = cms.InputTag("cleanLayer1Electrons"),
probeSource = cms.InputTag("cleanLayer1Electrons"),
jetSource = cms.InputTag("cleanLayer1JetsAK5")
)