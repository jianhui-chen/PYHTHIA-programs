//this task was given by Helen, to see how many jets are produced per event on average
//it is expected that this task will require a high number of events generated
//for now, I want to test the codes by setting a pT cut and uses 100,000 events and see what happens 

//using anti-kT jet algorithm and R = .4

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

int main()
{
    // Settings, 1,000,000 events generated 
    int nEvent = 1000000;

    // Generator
    Pythia pythia;

    // Turning on HardQCD
    pythia.readString("HardQCD:all = on");

    //Initialisation, p-p @ 200GeV
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 200.");
    pythia.init();

    // Create file on which histogram(s) can be saved.
    TFile *outFile = new TFile("jet production.root", "RECREATE");

    // Book histogram.

    TH1F *jet_production_with_minpT_10 = new TH1F("jet_production_with_minpT_10", "jet produced with a minimum pT of 10 GeV", 100, 0, 20);


    // Fastjet analysis - select algorithm and parameters
    double Rparam = 0.4;
    fastjet::Strategy strategy = fastjet::Best;
    fastjet::RecombinationScheme recombScheme = fastjet::E_scheme;
    fastjet::JetDefinition *jetDef = NULL;
    jetDef = new fastjet::JetDefinition(fastjet::antikt_algorithm, Rparam, recombScheme, strategy);

    // Fastjet input
    std::vector<fastjet::PseudoJet> fjInputs;

    // Begin event loop. Generate event. Skip if error.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent)
    {
        if (!pythia.next())
            continue;

        // Reset Fastjet input
        fjInputs.resize(0);

        // Loop over event record to decide what to pass to FastJet
        for (int i = 0; i < pythia.event.size(); ++i)
        {
            // Final state only
            if (!pythia.event[i].isFinal())
                continue;

            // No neutrinos
            if (pythia.event[i].idAbs() == 12 || pythia.event[i].idAbs() == 14 ||
                pythia.event[i].idAbs() == 16)
                continue;

            // Store as input to Fastjet
            fjInputs.push_back(fastjet::PseudoJet(pythia.event[i].px(), pythia.event[i].py(), pythia.event[i].pz(), pythia.event[i].e()));
        }

        if (fjInputs.size() == 0)
        {
            cout << "Error: event with no final state particles" << endl;
            continue;
        }

        // Run Fastjet algorithm
        vector<fastjet::PseudoJet> inclusiveJets, sortedJets;
        fastjet::ClusterSequence clustSeq(fjInputs, *jetDef);

        // Extract inclusive jets sorted by pT
        inclusiveJets = clustSeq.inclusive_jets(10.0);
        sortedJets = sorted_by_pt(inclusiveJets);

        for (unsigned int i = 0; i < sortedJets.size(); i++)
        {
            // Only count jets that have |eta| < 1.0
            if (abs(sortedJets[i].eta()) > 1.0)
                continue;

            jet_production_with_minpT_10->Fill(sortedJets[i].pt());
        }
        // End of event loop.
    }

    // Statistics on event generation.
    pythia.stat();

    // Save histogram on file and close file.
    jet_production_with_minpT_10->Write();
    delete outFile;

    delete jetDef;
    return 0;
}