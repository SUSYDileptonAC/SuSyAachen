// -*- C++ -*-
//
// Package:    IsoTreeWriter
// Class:      IsoTreeWriter
// 
/**\class IsoTreeWriter IsoTreeWriter.cc NiklasMohr/IsoTreeWriter/src/IsoTreeWriter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Niklas Mohr,32 4-C02,+41227676330,
//         Created:  Tue Jan  5 13:23:46 CET 2010
// $Id: IsoTreeWriter.cc,v 1.4 2011/01/07 13:08:19 sprenger Exp $
//
//


// system include files
#include <memory>
#include "TFile.h"
#include "TTree.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/PatCandidates/interface/PATObject.h"
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
//
// class declaration
//

template< typename T > 
class IsoTreeWriter : public edm::EDAnalyzer {
   public:
      explicit IsoTreeWriter(const edm::ParameterSet&);
      ~IsoTreeWriter();


    private:
        virtual void beginJob() ;
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

        virtual void fillIso(const edm::Handle< std::vector<T> >&);
        virtual void fillExtraVars(const pat::Electron&);
        virtual void fillExtraVars(const pat::Muon&);
        virtual void fillExtraVars(const pat::Tau&);
        virtual double calcIso(const T &);
        virtual double calcPfIso(const T &);

        // ----------member data ---------------------------
        // Switch for debug output
        bool mcInfo;
        edm::InputTag leptonSrc;
        edm::InputTag jetTag_;
        edm::InputTag metTag_;
        
        TFile *theFile;

        //Trees
        TTree*  treeIso;
        TTree*  treeIsoEvent;
        float pfIso;
        float pt;
        float eta;
        float tanc;
        float ht;
        float met;
        int nLept;
        int nJets;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
//
// constructors and destructor
//
template< typename T  > 
IsoTreeWriter<T>::IsoTreeWriter(const edm::ParameterSet& iConfig)
{
    //now do what ever initialization is needed
    //Input collections
    leptonSrc          = iConfig.getParameter<edm::InputTag> ("src");
    jetTag_          = iConfig.getParameter<edm::InputTag> ("jets");
    metTag_          = iConfig.getParameter<edm::InputTag> ("met");
    
    // Create the root file
    edm::Service<TFileService> theFile;

    TFileDirectory Tree = theFile->mkdir( "Trees" );
    treeIso = Tree.make<TTree>("Iso","Iso"); 
    treeIso->Branch("pfIso",&pfIso,"pfIso/F");
    treeIso->Branch("pt",&pt,"pt/F");
    treeIso->Branch("eta",&eta,"eta/F");
    treeIso->Branch("tanc",&tanc,"tanc/F");
    treeIso->Branch("ht",&ht,"ht/F");
    treeIso->Branch("met",&met,"met/F");
    treeIso->Branch("nLept",&nLept,"nLept/I");
    

}


template< typename T  > 
IsoTreeWriter<T>::~IsoTreeWriter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

template < typename T >
double IsoTreeWriter<T>::calcIso(const T & lepton)
{
    double value = (lepton.trackIso()+lepton.ecalIso()+lepton.hcalIso())/lepton.pt();
    return value;
}

template < typename T >
double IsoTreeWriter<T>::calcPfIso(const T & lepton)
{
    double value = (lepton.chargedHadronIso()+lepton.photonIso()+lepton.neutralHadronIso())/lepton.pt();
    return value;
}

template< typename T > 
void IsoTreeWriter<T>::fillExtraVars(const pat::Electron& lepton)
{
  tanc = -1.0;
}

template< typename T > 
void IsoTreeWriter<T>::fillExtraVars(const pat::Muon& lepton)
{
  tanc = -1.0;
}

template< typename T > 
void IsoTreeWriter<T>::fillExtraVars(const pat::Tau& lepton)
{
  tanc = lepton.tauID("byTaNCfrOnePercent");
}


//
// member functions
template< typename T > 
void IsoTreeWriter<T>::fillIso(const edm::Handle< std::vector<T> >& leptons)
{
    nLept = 0;
    for (typename std::vector<T>::const_iterator lep_i = leptons->begin(); lep_i != leptons->end(); ++lep_i){
            ++nLept;
            pfIso = calcPfIso(*lep_i);
            pt = lep_i->pt();
            eta = lep_i->eta();
	    fillExtraVars(*lep_i);
            treeIso->Fill();
    }
}


// ------------ method called to for each event  ------------
template< typename T  > 
void IsoTreeWriter<T >::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    if (iEvent.isRealData()) mcInfo = false;

    //Collection
    edm::Handle< std::vector<T> > leptons;
    iEvent.getByLabel(leptonSrc, leptons);

    edm::Handle< std::vector< pat::Jet > > jets;
    iEvent.getByLabel(jetTag_, jets);

    edm::Handle< std::vector< pat::MET > > mets;
    iEvent.getByLabel(metTag_, mets);

    met = mets->front().pt();

    ht = 0.0;
    for(std::vector<pat::Jet>::const_iterator it = jets->begin(); it != jets->end() ; ++it){
         ht += (*it).pt();
    }

    //Probes
    //run the TnP
    fillIso(leptons);

}


// ------------ method called once each job just before starting event loop  ------------
template< typename T > 
void IsoTreeWriter<T>::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
template< typename T > 
void IsoTreeWriter<T>::endJob() 
{
}

//define this as a plug-in
typedef IsoTreeWriter< pat::Muon > MuonIsoTreeWriter;
typedef IsoTreeWriter< pat::Electron > ElectronIsoTreeWriter;
typedef IsoTreeWriter< pat::Tau > TauIsoTreeWriter;
DEFINE_FWK_MODULE(MuonIsoTreeWriter);
DEFINE_FWK_MODULE(ElectronIsoTreeWriter);
DEFINE_FWK_MODULE(TauIsoTreeWriter);
