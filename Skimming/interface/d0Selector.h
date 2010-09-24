#ifndef d0Selector_h
#define d0Selector_h

//DataFormats
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

//Framework
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"

//STL
#include <vector>

template<typename T, typename collectionType, typename containerType>
struct d0Selector {
  typedef collectionType collection;
  typedef containerType container;
  typedef typename container::const_iterator const_iterator;
  d0Selector ( const edm::ParameterSet & cfg ):
    d0Min_( cfg.getParameter<double>( "d0Min") ),
    d0Max_( cfg.getParameter<double>( "d0Max" ) ),
    dZMin_( cfg.getParameter<double>( "dZMin") ),
    dZMax_( cfg.getParameter<double>( "dZMax" ) ),
    beamSpotSrc_( cfg.getParameter<edm::InputTag>( "beamSpotSource" ) )  { }
  
  const_iterator begin() const { return selected_.begin(); }
  const_iterator end() const { return selected_.end(); }
  void select(const edm::Handle< collection > &col , const edm::Event &ev , const edm::EventSetup &setup ) {
    
    edm::Handle<T> beamSpotHandle;
    ev.getByLabel(beamSpotSrc_, beamSpotHandle);
    point_ = getPoint(beamSpotHandle);

    selected_.clear();
    std::pair<double, double> dS (0.,0.);
    for(typename collection::const_iterator it = col.product()->begin(); 
	 it != col.product()->end(); ++it ){
      
      dS = calcDs( *it, point_);
      //std::cout << "d0 = " << dS.first << "  dZ = " << dS.second << std::endl;
      if ( dS.first >= d0Min_ && dS.first < d0Max_ && dS.second >= dZMin_ && dS.second < dZMax_ )
      //std::cout << "Selected it d0 = " << dS.first << "  dZ = " << dS.second << std::endl;
	selected_.push_back( & (*it) );
    }
  }
  std::pair<double, double> calcDs( reco::PFCandidate p, math::XYZPoint vx)
  {    
    if(p.gsfTrackRef().isNonnull())
      return std::make_pair(std::abs( p.gsfTrackRef()->dxy( vx )), std::abs( p.gsfTrackRef()->dz( vx )));  
    return std::make_pair(std::abs( p.trackRef()->dxy( vx )), std::abs( p.trackRef()->dz( vx )));  
  }
  // fast hack: this should be specialized
  std::pair<double, double> calcDs( pat::Electron p, math::XYZPoint vx)
  {    
    return std::make_pair(std::abs( p.gsfTrack()->dxy( vx )), std::abs( p.gsfTrack()->dz( vx )));  
  }
  // fast hack: this should be the normal one
  std::pair<double, double> calcDs( pat::Muon p, math::XYZPoint vx )
  {
    return std::make_pair(std::abs( p.track()->dxy( vx )), std::abs( p.track()->dz( vx )));  
  }
  // fast hack: this should be specialized
  math::XYZPoint getPoint( edm::Handle<reco::BeamSpot> beamSpotHandle)
  { 
      return beamSpotHandle->position();  
  }
  // fast hack: this should be the normal one
  math::XYZPoint getPoint( edm::Handle<reco::VertexCollection> vertexHandle )
  {
      math::XYZPoint pv;
      for (reco::VertexCollection::const_iterator it = vertexHandle->begin(); it != vertexHandle->end(); ++it) {
            pv = it->position();
            break;
      }
      return pv;
  }
    

  size_t size() const { return selected_.size(); }
private:
  container selected_;
  double d0Min_;
  double d0Max_;
  double dZMin_;
  double dZMax_;
  math::XYZPoint point_;
  edm::InputTag beamSpotSrc_;
};

#endif
