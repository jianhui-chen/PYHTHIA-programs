//This program is designed to study the correlation between highest jetpT and event multiplicty,
//as well as how they vary with differnt pT hat min cut
//using anti-kT jet algorithm and R = .4
//this one has pT hat min cut of 5 GeV

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
    // Settings
    int nEvent = 10000;

    // Generator
    Pythia pythia;

    //turning on HardQCD
    pythia.readString("HardQCD:all = on");

     //set pthatmin to 5
    pythia.readString("PhaseSpace:pTHatMin = 5.");

    //Initialisation, p-p @ 200GeV
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 200.");
    pythia.init();

    // Create file on which histogram(s) can be saved.
    TFile *outFile = new TFile("highest_pt_and_mult_5.root", "RECREATE");

    // Book histogram.
    TH1F *highest_pt_5 = new TH1F("highest_pt_5", "highest jet transverse momenta with pT Hat Min cut of 5 GeV", 100, 0, 50);
    TH2F *highest_pt_and_mult_5 = new TH2F("highest_pt_and_mult_5", "highest jetpT vs event multiplicty with pTHatMin cut of 5", 100, 0, 50, 100, 0, 100);

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

        // Find number of all final charged particles and fill histogram.
        int nCharged = 0;
        for (int i = 0; i < pythia.event.size(); ++i)
        {
            if (pythia.event[i].isFinal() && pythia.event[i].isCharged())
                ++nCharged;
        }

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
        inclusiveJets = clustSeq.inclusive_jets();
        sortedJets = sorted_by_pt(inclusiveJets);

        //take the jet with the highest jetpT
        fastjet::PseudoJet leadingJet = sortedJets[0];

        if (abs(leadingJet.eta()) < 1.0)
        {
            highest_pt_5->Fill(leadingJet.pt());
            highest_pt_and_mult_5->Fill(leadingJet.pt(), nCharged);
        }


        // End of event loop.
    }

    // Statistics on event generation.
    pythia.stat();

    // Save histogram on file and close file.
    highest_pt_5->Write();
    highest_pt_and_mult_5->Write();
    delete outFile;

    delete jetDef;
    return 0;
}