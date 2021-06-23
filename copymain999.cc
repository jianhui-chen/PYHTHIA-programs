//main999.cc is the first example that I am experimenting with
//the goal is to combine the main71.cc and main91.cc to produce a histogram of jet transverse momenta (pT) of fast jets using ROOT
//all option at the RHIC center of mass energy of 200 GeV (each beam is 100 GeV energy) 
//produce jets with the anti-kT algorithm and R = 0.6, 
//have a minimum jet pT threshold of say 3 GeV. 

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

//use this interface to access FastJet?
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

// ROOT, for histogramming.
#include "TH1.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"

// ROOT, for saving file.
#include "TFile.h"

using namespace Pythia8;


int main() {
 // Settings
  int  nEvent = 10000;
  bool doMPI  = true;

 // Generator
  Pythia pythia;

 // Single W production
  pythia.readString("WeakSingleBoson:ffbar2W = on");

 // Force decay W->ev
  pythia.readString("24:onMode = off");
  pythia.readString("24:onIfAny = 11 12");
  
 // Multiparton Interactions
  if (doMPI == false) pythia.readString("PartonLevel:MPI = off");

 //set pthatmin to 15
  pythia.readString("PhaseSpace:pTHatMin = 15.");

 //Initialisation, p pbar @ 200GeV
 //idB is for the second incoming particle.
 //-2212 = pbar,
  pythia.readString("Beams:idB = -2212");
  pythia.readString("Beams:eCM = 200."); //change this to 200 Gev
  pythia.init();

 // Create file on which histogram(s) can be saved.
  TFile* outFile = new TFile("first2.root", "RECREATE");

 // Book histogram.
  TH1F *jetpt2 = new TH1F("jetpt2","jet transverse momenta", 100, 0, 100);

 // Fastjet analysis - select algorithm and parameters
  double Rparam = 0.4;
  fastjet::Strategy               strategy = fastjet::Best;
  fastjet::RecombinationScheme    recombScheme = fastjet::E_scheme;
  fastjet::JetDefinition         *jetDef = NULL;
  jetDef = new fastjet::JetDefinition(fastjet::kt_algorithm, Rparam,
                                      recombScheme, strategy);

 // Fastjet input
  std::vector <fastjet::PseudoJet> fjInputs;

 // Statistics for later
  int nEventAccept25[5] = { 0, 0, 0, 0, 0 };
  int vetoCount[4] = { 0, 0, 0, 0 };
  const char *vetoStr[] = { "ET(elec)", "|eta(elec)|",
                            "ET(missing)", "deltaR(elec, jet)" };
  bool firstEvent = true;


  // Begin event loop. Generate event. Skip if error.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    if (!pythia.next()) continue;

    // Reset Fastjet input
    fjInputs.resize(0);

    // Keep track of missing ET
    Vec4 missingETvec;

    // Loop over event record to decide what to pass to FastJet
    for (int i = 0; i < pythia.event.size(); ++i) {
      // Final state only
      if (!pythia.event[i].isFinal())        continue;

      // No neutrinos
      if (pythia.event[i].idAbs() == 12 || pythia.event[i].idAbs() == 14 ||
          pythia.event[i].idAbs() == 16)     continue;

      // Only |eta| < 3.6
      if (abs(pythia.event[i].eta()) > 3.6) continue;

      // Missing ET
      missingETvec += pythia.event[i].p();


      // Store as input to Fastjet
      fjInputs.push_back( fastjet::PseudoJet( pythia.event[i].px(),
        pythia.event[i].py(), pythia.event[i].pz(), pythia.event[i].e() ) );
    }

    if (fjInputs.size() == 0) {
      cout << "Error: event with no final state particles" << endl;
      continue;
    }

    // Run Fastjet algorithm
    vector <fastjet::PseudoJet> inclusiveJets, sortedJets;
    fastjet::ClusterSequence clustSeq(fjInputs, *jetDef);

    // For the first event, print the FastJet details
    if (firstEvent) {
      cout << "Ran " << jetDef->description() << endl;
      cout << "Strategy adopted by FastJet was "
           << clustSeq.strategy_string() << endl << endl;
      firstEvent = false;
    }

    // Extract inclusive jets sorted by pT (note minimum pT of 10.0 GeV)
    inclusiveJets = clustSeq.inclusive_jets(10.0);
    sortedJets    = sorted_by_pt(inclusiveJets);

    // Missing ET cut
    double missingET = missingETvec.pT();
    if (missingET < 30.0) {
      vetoCount[2]++;
      continue;
    }

    // Keep track of jets with pT > 20/25 GeV
    int  jetCount20 = 0, jetCount25 = 0;

    for (unsigned int i = 0; i < sortedJets.size(); i++) {
      // Only count jets that have |eta| < 2.0
      if (abs(sortedJets[i].eta()) > 2.0) continue;

                  jetpt2->Fill(sortedJets[i].pt());
    }

  // End of event loop.
  }

 // Statistics on event generation.
  pythia.stat();

 // Show histogram. Possibility to close it.
  jetpt2->Draw();
  std::cout << "\nDouble click on the histogram window to quit.\n";
  gPad->WaitPrimitive();

 // Save histogram on file and close file.
  jetpt2->Write();
  delete outFile;

 delete jetDef;
 return 0;
}

