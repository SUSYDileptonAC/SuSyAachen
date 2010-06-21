// -*- C++ -*-
//
// Package:    MtFilter
// Class:      MtFilter
// 
/**\class MtFilter MtFilter.cc SuSyAachen/Skimming/src/MtFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Matthias Edelhoff
//         Created:  Mon Nov 16 11:26:19 CET 2009
// $Id: MtFilter.cc,v 1.1 2010/06/09 20:49:42 edelhoff Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <vector>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"


#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/Math/interface/deltaPhi.h"

//
// class declaration
//

class MtFilter : public edm::EDFilter {
public:
  explicit MtFilter(const edm::ParameterSet&);
  ~MtFilter();
  
private:
  typedef reco::Candidate cand;
  typedef edm::View<cand> collection;
  typedef std::vector<pat::MET> metCollection;
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  // ----------member data ---------------------------
  
  edm::InputTag inputTag_;
  edm::InputTag inputTagMET_;

  double minMT_;

};

// constructors and destructor
MtFilter::MtFilter(const edm::ParameterSet& iConfig)
{
  inputTagMET_ = iConfig.getParameter<edm::InputTag> ("srcMET");
  inputTag_ = iConfig.getParameter<edm::InputTag> ("src");
  minMT_ = iConfig.getParameter<double> ("minMT");

}

MtFilter::~MtFilter(){}


// member functions
// ------------ method called on each new Event  ------------
bool
MtFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  edm::Handle< collection > candidates;
  iEvent.getByLabel(inputTag_, candidates);

  edm::Handle< metCollection > metCollection;
  iEvent.getByLabel(inputTagMET_, metCollection);

  pat::MET met = *((*metCollection).begin());
  bool result = false;
  for(collection::const_iterator it = candidates->begin(); it != candidates->end() ; ++it){

    result |= sqrt( 2*(*it).et()*met.et()*( 1 - cos(reco::deltaPhi(((*it).p4()).phi(),met.phi())) )) >= minMT_;
  }
  return result;
}

// ------------ method called once each job just before starting event loop  ------------
void 
MtFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MtFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(MtFilter);