//This is a second version of the event multiplicty of softQCD events
//I think something went wrong along the way and I am supposed to separate the different programs
//over all we are measuring event multiplicty of SoftQCD events at 200 GeV

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for histogramming.
#include "TH1.h"

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

    //turning on SoftQCD
    pythia.readString("SoftQCD:all = on");

    //turning off all settings
    pythia.readString("PartonLevel:MPI = off");
    pythia.readString("PartonLevel:ISR = off");
    pythia.readString("HadronLevel:Hadronize = off");

    //Initialisation, p-p @ 200GeV
    pythia.readString("Beams:eCM = 200.");
    pythia.init();

    // Create file on which histogram(s) can be saved.
    TFile *outFile = new TFile("multiplicity_all_off.root", "RECREATE");

    // Book histogram.
    TH1F *multiplicity_all_off = new TH1F("multiplicity_all_off", "total event multiplicity with all settings off", 50, 0, 100);

    // Begin event loop. Generate event. Skip if error. List first one.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent)
    {
        if (!pythia.next())
            continue;

        // Find number of all final charged particles and fill histogram.
        int nCharged = 0;
        for (int i = 0; i < pythia.event.size(); ++i)
        {
            if (pythia.event[i].isFinal() && pythia.event[i].isCharged())
                ++nCharged;
        }
        multiplicity_all_off->Fill(nCharged);
    }
    // Statistics on event generation.
    pythia.stat();

    // Save histogram on file and close file.
    multiplicity_all_off->Write();
    delete outFile;

    return 0;
}