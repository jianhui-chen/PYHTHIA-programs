  //main999.cc is the first example that I am experimenting with
  //the goal is to combine the main71.cc and main91.cc to produce a histogram of jet transverse momenta (pT) of fast jets using ROOT
  //all option at the RHIC center of mass energy of 200 GeV (each beam is 100 GeV energy) 
  //produce jets with the anti-kT algorithm and R = 0.6, 
  //have a minimum jet pT threshold of say 3 GeV. 

  // Header file to access Pythia 8 program elements.
  #include "Pythia8/Pythia.h"

  //use this interface to access FastJet
  #include "fastjet/PseudoJet.hh"
  #include "fastjet/ClusterSequence.hh"

  // ROOT, for histogramming.
  #include "TH1.h"
  #include "TH2.h"


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

    
  // Multiparton Interactions
    if (doMPI == false) pythia.readString("PartonLevel:MPI = off");



  //set pthatmin to 15
    //pythia.readString("PhaseSpace:pTHatMin = 15.");

    pythia.readString("SoftQCD:all = on");

  //Initialisation, p pbar @ 200GeV
  //idB is for the second incoming particle.
  //-2212 = pbar,
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 200."); //change this to 200 Gev
    pythia.init();

  // Create file on which histogram(s) can be saved.
    TFile* outFile = new TFile("first3.root", "RECREATE");

  // Book histogram.
    TH1F *jetpt3 = new TH1F("jetpt3","jet transverse momenta", 100, 0, 100);

    TH2F *etavspt = new TH2F("etavspt","transverse momenta vs rapidity", 100, 0, 100, 100, -6, 6);


  // Fastjet analysis - select algorithm and parameters
    double Rparam = 0.4;
    fastjet::Strategy               strategy = fastjet::Best;
    fastjet::RecombinationScheme    recombScheme = fastjet::E_scheme;
    fastjet::JetDefinition         *jetDef = NULL;
    jetDef = new fastjet::JetDefinition(fastjet::kt_algorithm, Rparam,
                                        recombScheme, strategy);

  // Fastjet input
    std::vector <fastjet::PseudoJet> fjInputs;



    // Begin event loop. Generate event. Skip if error.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
      if (!pythia.next()) continue;

      // Reset Fastjet input
      fjInputs.resize(0);



      // Loop over event record to decide what to pass to FastJet
      for (int i = 0; i < pythia.event.size(); ++i) {
        // Final state only
        if (!pythia.event[i].isFinal())        continue;

        // No neutrinos
        if (pythia.event[i].idAbs() == 12 || pythia.event[i].idAbs() == 14 ||
            pythia.event[i].idAbs() == 16)     continue;

      


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

    

      // Extract inclusive jets sorted by pT (note minimum pT of 10.0 GeV)
      inclusiveJets = clustSeq.inclusive_jets(0.0);
      sortedJets    = sorted_by_pt(inclusiveJets);


    

      for (unsigned int i = 0; i < sortedJets.size(); i++) {
        // Only count jets that have |eta| < 2.0
        if (abs(sortedJets[i].eta()) > 2.0) continue;

        jetpt3->Fill(sortedJets[i].pt());
        etavspt->Fill(sortedJets[i].pt(),sortedJets[i].eta());
      }

    // End of event loop.
    }

  // Statistics on event generation.
    pythia.stat();

  // Save histogram on file and close file.
    jetpt3->Write();
    delete outFile;

  delete jetDef;
  return 0;
  }

