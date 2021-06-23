
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

    //Initialisation, p-p @ 200GeV
    pythia.readString("Beams:eCM = 200.");
    pythia.init();

    // Create file on which histogram(s) can be saved.
    TFile *outFile = new TFile("softQCD_all_on.root", "RECREATE");

    // Book histogram.
    TH1F *softQCD_all_on = new TH1F("softQCD_all_on", "total event multiplicity with all SoftQCD setting on", 50, 0, 100);

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
        softQCD_all_on->Fill(nCharged);
    }

    // Statistics on event generation.
    pythia.stat();

    // Save histogram on file and close file.
    softQCD_all_on->Write();
    delete outFile;

    return 0;
}
