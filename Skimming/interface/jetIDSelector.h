#ifndef jetIDSelector_h
#define jetIDSelector_h

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/ValueMap.h"


//Framework
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"

//STL
#include <vector>

template<typename T, typename collectionType, typename containerType>
struct jetIDSelector {
  typedef collectionType collection;
  typedef containerType container;
  typedef typename container::const_iterator const_iterator;
  jetIDSelector ( const edm::ParameterSet & cfg, edm::ConsumesCollector ) {}
  
  const_iterator begin() const { return selected_.begin(); }
  const_iterator end() const { return selected_.end(); }
  void select(const edm::Handle< collection > &col , const edm::Event &ev , const edm::EventSetup &setup ) {
    


    selected_.clear();

    //~ double etaValue;
    for(typename collection::const_iterator it = col.product()->begin(); it != col.product()->end(); ++it ){


		bool isPass = true;
		double energy = (*it).chargedHadronEnergy() + (*it).neutralHadronEnergy() + (*it).photonEnergy() + (*it).electronEnergy() + (*it).muonEnergy() +  (*it).HFEMEnergy();

		// Loose PF JetID from here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID#Recommendations_for_13_TeV_data
		if (((*it).chargedHadronEnergy()/energy) == 0 && fabs((*it).eta()) < 2.4 ) isPass = false;		
		if (((*it).chargedMultiplicity()) == 0 && fabs((*it).eta() < 2.4 )) isPass = false;
		if (((*it).chargedEmEnergy()/energy) >= 0.99 && fabs((*it).eta() < 2.4 )) isPass = false;		
		if (((*it).neutralHadronEnergy()/energy) >= 0.99) isPass = false;				
		if (((*it).neutralEmEnergy()/energy) >= 0.99) isPass = false;
		//if (((*it).muonEnergy()/energy) >= 0.8) isPass = false; // removed in 13 TeV ID								
		if (((*it).numberOfDaughters()) < 2) isPass = false;	
		
		if (isPass){
			selected_.push_back( & (*it) );
    	}
    }
  }

    

  size_t size() const { return selected_.size(); }
private:
  container selected_;
};

#endif