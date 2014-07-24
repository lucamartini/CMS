// -*- C++ -*-
//
// Package:    TrigNames
// Class:      TrigNames
// 
/**\class TrigNames TrigNames.cc TrigNames/TrigNames/src/TrigNames.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Luca MARTINI
//         Created:  Fri Feb  6 09:44:33 CET 2009
// $Id: TrigNames.cc,v 1.2 2012/01/24 13:17:52 lmartini Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
//
// class decleration
//

class TrigNames : public edm::EDAnalyzer {
   public:
      explicit TrigNames(const edm::ParameterSet&);
      ~TrigNames();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

      edm::InputTag thetriggerEventLabel;
      edm::InputTag theHLTriggerResults;
      edm::TriggerNames triggerNames_;

      int ntrigs;
      bool HLTBits_wasrun[1000];       // Each HLT bits was run or not
      bool HLTBits_accept[1000];       // Each HLT bits fired or not
      bool HLTBits_error[1000];        // Each HLT bits run successfully or not 
      bool HLTGlobal_wasrun;          // The HLT was run or not
      bool HLTGlobal_Decision;        // Global HLT decision
      bool HLTGlobal_error;           // HLT path error or not

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
TrigNames::TrigNames(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  thetriggerEventLabel       = iConfig.getParameter<edm::InputTag>("triggerEventLabel");
  theHLTriggerResults        = iConfig.getParameter<edm::InputTag>("HLTriggerResults");

}


TrigNames::~TrigNames()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TrigNames::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace trigger;



  Handle<TriggerResults> hltresults;
  iEvent.getByLabel(theHLTriggerResults, hltresults);
  if (hltresults.isValid()) {
    HLTGlobal_wasrun=hltresults->wasrun();
    HLTGlobal_Decision=hltresults->accept();
    HLTGlobal_error=hltresults->error();

    ntrigs=hltresults->size();
    if (ntrigs==0) cout << "%HLTInfo -- No trigger name given in TriggerResults of the input " << endl;

//     triggerNames_.init(* hltresults);
    triggerNames_ = iEvent.triggerNames(* hltresults);

    for ( int i=0; i!=ntrigs; i++) {
      HLTBits_wasrun[i]=hltresults->wasrun(i);
      HLTBits_accept[i]=hltresults->accept(i);
 //     cout << "trig # "<<i<<" is accepted? "<< HLTBits_accept[i]<<endl;
      bool accept = hltresults->accept(i);

      string trigName=triggerNames_.triggerName(i);
      HLTBits_error[i]=hltresults->error(i);
      cout << "%HLTInfo --  HLTTrigger(" << i << "): " << trigName << " = " << accept << endl;
    }
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
TrigNames::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TrigNames::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrigNames);

