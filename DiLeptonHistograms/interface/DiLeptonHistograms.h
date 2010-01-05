#ifndef DiLeptonHistograms_h
#define DiLeptonHistograms_h

/** \class DiLeptonHistograms
 *
 *  
 *  This class is an EDAnalyzer for PAT
 *  Layer 0 and Layer 1 output
 *
 *  $Date: 2008/12/9 08:07:03 $
 *  $Revision: 1.4 $
 *  for CMSSW_2_2_3
 *  \author Niklas Mohr  --  niklas.mohr@cern.ch
 *
 */

// system include files
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include <vector>
#include <map>
#include <utility> 
#include <memory>
#include <iostream>
#include <fstream>
#include <string>

// user include files
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/PatCandidates/interface/PATObject.h"
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Flags.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "SuSyAachen/DiLeptonHistograms/interface/Combinations.h"


class DiLeptonHistograms : public edm::EDAnalyzer {
    public:
    
    /// Constructor
    DiLeptonHistograms(const edm::ParameterSet &iConfig);

    /// Destructor
    virtual ~DiLeptonHistograms();

    /// Perform the real analysis
    void analyze(const edm::Event &iEvent, const edm::EventSetup &iSetup);

    private:
    //Histograms
    TH1F**       hLeptonMult;
    TH1F**       hElectronMult;
    TH1F**       hMuonMult;
    TH1F**       hJetMult;
    TH1F**       hbJetMult;

    TH1F**       hElectronIso;
    TH1F**       hElectronTrackIso;
    TH1F**       hElectronCaloIso;
    TH1F**       hMuonIso;
    TH1F**       hMuonTrackIso;
    TH1F**       hMuonCaloIso;

    TH1F**       hMissingET;
    TH1F**       hMissingETmc;
    TH1F**       hEtSum;
    TH1F**       halphaT;
    TH1F**       hHT;
    TH1F**       hMHT;
    TH2F**       h2dMETEtSumJets;
    TH2F**       h2dMETHT;
    TH2F**       h2dMETMHT;
    TH1F**       hJet1METPhi;
    TH2F**       hJet1EtaMET;

    TH1F**       hJetEt;
    TH1F**       hJetEta;
    TH1F**       hJetPhi;
    TH1F**       hEtJet1;
    TH1F**       hEtJet2;
    TH1F**       hEtJet3;
    TH1F**       hEtJet4;
    TH1F**       hJet1Eta;
    TH1F**       hJet2Eta;
    TH1F**       hJet3Eta;
    TH1F**       hJet4Eta;

    TH1F**       hInvMSFOS;
    TH1F**       hInvMOFOS;
    TH1F**       hInvMass;
    TH1F**       hDileptonPt;
    TH1F**       hJZB;
    TH1F**       hInvMMuon;
    TH1F**       hInvMMuonSS;
    TH1F**       hInvMElectron;
    TH1F**       hInvMElectronSS;
    TH1F**       hInvMassMC;
    TH1F**       hInvMassZMC;
    
    TH1F**       hMuonResolution;
    TH1F**       hElectronResolution;
        
    TH1F**       hDeltaR;
    TH1F**       hDeltaPhi;
    TH1F**       hDeltaPhiMET;
    
    TH1F**       hInvMbbllSFOS;
    TH1F**       hInvMbbllOFOS;

    TH2F**       h2dMETInvMassSFOS;
    TH2F**       h2dHTInvMassSFOS;
    TH2F**       h2dEtJetsInvMassSFOS;
    TH2F**       h2dEtJet4InvMassSFOS;
    TH2F**       h2dIsoInvMassSFOS;

    TH2F**       h2dMETInvMassOFOS;
    TH2F**       h2dHTInvMassOFOS;
    TH2F**       h2dEtJetsInvMassOFOS;
    TH2F**       h2dEtJet4InvMassOFOS;
    TH2F**       h2dIsoInvMassOFOS;

    TH1F**       hMuonPt;
    TH1F**       hMuonSumPt;
    TH1F**       hMuon1Pt;
    TH1F**       hMuon2Pt;
    TH1F**       hMuonEta;
    TH1F**       hMuon1Eta;
    TH1F**       hMuon2Eta;
    TH1F**       hMuonPhi; 
    TH1F**       hGenMuonPt;
    TH1F**       hGenMuonEta;
        
    TH1F**       hMuonChi2;
    TH1F**       hMuond0;
    TH1F**       hMuond0Sig;
    TH1F**       hMuonnHits;
    TH2F**       hMuonIsod0;
    TH2F**       hMuonEtaPhi;
        
    TH1F**       hElectronPt;
    TH1F**       hElectronSumPt;
    TH1F**       hElectron1Pt;
    TH1F**       hElectron2Pt;
    TH1F**       hElectronEta;
    TH1F**       hElectron1Eta;
    TH1F**       hElectron2Eta;
    TH1F**       hElectronPhi;
    TH1F**       hElectrond0;
    TH1F**       hGenElectronPt;
    TH1F**       hGenElectronEta;
        
    TH1F**       hElectronEoverP;
    TH1F**       hElectronfBrem;
    TH1F**       hElectronHoverE;
    TH1F**       hElectrondeltaPhiIn;
    TH1F**       hElectrondeltaEtaIn;
    TH2F**       hElectronIsod0;
    TH2F**       hElectronEtaPhi;

    TH2F**       h2dMuonEtaPt;
    TH2F**       h2dMatchedMuonEtaPt;
    TH2F**       h2dGenMuonEtaPt;
    TH2F**       h2dElectronEtaPt;
    TH2F**       h2dMatchedElectronEtaPt;
    TH2F**       h2dGenElectronEtaPt;
        
    TH2F**       h2dTnPProbeSigEtaPt;
    TH2F**       h2dTnPPassSigEtaPt;
    TH2F**       h2dTnPProbeSSSigEtaPt;
    TH2F**       h2dTnPPassSSSigEtaPt;
    TH2F**       h2dTnPProbeSB1EtaPt;
    TH2F**       h2dTnPPassSB1EtaPt;
    TH2F**       h2dTnPProbeSSSB1EtaPt;
    TH2F**       h2dTnPPassSSSB1EtaPt;
    TH2F**       h2dTnPProbeSB2EtaPt;
    TH2F**       h2dTnPPassSB2EtaPt;
    TH2F**       h2dTnPProbeSSSB2EtaPt;
    TH2F**       h2dTnPPassSSSB2EtaPt;
    TH1F**       hTnPProbeInvMass;
    TH1F**       hTnPPassInvMass;
    TH1F**       hTnPSSInvMass;
    TH2F**       h2dTnPProbeDRPt;
    TH2F**       h2dTnPProbenMatchPt;
    TH2F**       h2dTnPProbeChargePt;
	
    TH1F**       hTrigger;
    TH1F**       hWeight;
    
    //Trees
    TTree*       treeOFOS;
    TTree*       treeSFOS;
    TTree*       treeMuon;
    TTree*       treeElec;
    float invMOFOS;
    float invMSFOS;
    float invMMuon;
    float invMElec;
    float invweight;

    // The file which will store the histos
    TFile *theFile;

    // Switch for debug output
    bool debug;
    
    // Switch for mc analysis
    bool mcInfo;

    //std::string rootFileName;
    edm::InputTag mcSrc;
    edm::InputTag beamSpotSrc;
    edm::InputTag muonSrc;
    edm::InputTag electronSrc;
    edm::InputTag metSrc;
    edm::InputTag jetSrc;
    edm::InputTag jetObj;
    edm::InputTag trackSrc;
    
    std::string muon_fname;
    std::string electron_fname;

    //Set weight via cfg
    double externalWeight;
    bool Signal_Analysis;

    //Cuts for the analysis
    double cut_MuonPt;
    double cut_MuonEta;
    
    double cut_ElectronPt;
    double cut_ElectronEta;
    
    double cut_bTagDiscriminator;
   
    double electron_Scale;
    double eff_Electron_Scale;
    double eff_Muon_Scale;
    double jet_Scale;
    
    std::string bJetAlgo;

    math::XYZPoint bs;

    static const int nEtaBins=10;
    static const float boundEta[nEtaBins+1]; 
    static const int nPtBins=4;
    static const float boundPt[nPtBins+1]; 
    float Muon_Eff[nEtaBins][nPtBins];
    float Electron_Eff[nEtaBins][nPtBins];

    std::vector<unsigned int> mlaMinDpt; //!< list of good jet indices forming pseudo-jet a (MinDEt)
    std::vector<unsigned int> mlbMinDpt; //!< list of good jet indices forming pseudo-jet b (MinDEt)
    
    int general;
    int effcor;
    int unmatched;
    int promt;
    int decay;
  
    //Global counters
    int numTotEvents;
    int numTotElectrons;
    int numTotMuons;
    int numTotJets;

    inline void InitHisto(TFileDirectory *fs, const int process);
    inline void ReadEfficiency();


    virtual void Analysis(const edm::Handle< std::vector<pat::Muon> >&, const edm::Handle< std::vector<pat::Electron> >&, const edm::Handle< std::vector<pat::Jet> >&, const edm::Handle< std::vector<pat::MET> >&, double weight, const int process);

    virtual void TriggerMonitor(const edm::Handle< edm::TriggerResults >&, double weight, const int process);
  
    virtual void ElectronMonitor(const pat::Electron*, const int, double, const int);
    virtual void MuonMonitor(const pat::Muon*, const int, double, const int);

    virtual void MuonInvMonitor(const double, const double, const double, const double, const double, const double, double, const int);
    virtual void OFOSInvMonitor(const double, const double, const double, const double, const double, const double, double, const int);
    virtual void ElectronInvMonitor(const double, const double, const double, const double, const double, const double, double, const int);
  
    virtual bool MCAnalysis(const edm::Handle< std::vector<pat::Muon> >&, const edm::Handle< std::vector<pat::Electron> >&, const edm::Handle< std::vector<reco::GenParticle> >& , double weight, const int process); 

    virtual bool FindMinCombo(std::vector<const reco::Candidate* >, std::vector<unsigned int>&, std::vector<unsigned int>&);
    virtual double CalcalphaT(std::vector<const reco::Candidate* >, std::vector<unsigned int>, std::vector<unsigned int> );

    virtual double getMuonWeight(const pat::Muon*);
    virtual double getElectronWeight(const pat::Electron*);
    virtual void PrintStatistics(void);

};

const float DiLeptonHistograms::boundEta[] = {-2.,-1.6,-1.2,-0.8,-0.4,0,0.4,0.8,1.2,1.6,2};
const float DiLeptonHistograms::boundPt[] = {0.,35,40.,45.,10000.};

#endif