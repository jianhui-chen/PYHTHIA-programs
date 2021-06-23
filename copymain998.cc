//copymain998.cc is the second version of main998.cc, which graphs the total event multiplicity using ROOT
//the main difference between this version and the first version is that I want to turn on the softQCD setting and see how that will change the graphs
//I will also delete some useless codes which Helen point out, that I don't necessarily need

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
   bool doMPI = true;

   // Generator
   Pythia pythia;

   // Multiparton Interactions
   if (doMPI == false)
      pythia.readString("PartonLevel:MPI = off"); //turn on/off to see the difference

   //turning on SoftQCD and use that instead of HardQCD
   pythia.readString("SoftQCD:all = on");

   //Initialisation, p-p @ 200GeV
   pythia.readString("Beams:eCM = 200."); //change this to 200 Gev
   pythia.init();

   // Create file on which histogram(s) can be saved.
   TFile *outFile = new TFile("total_event_multiplicity_2.root", "RECREATE");

   // Book histogram.
   TH1F *multiplicity2 = new TH1F("multiplicity2", "total event multiplicity", 100, 0, 100);

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
      multiplicity2->Fill(nCharged);
      // End of event loop. Statistics. Histogram. Done.
   }

   // Statistics on event generation.
   pythia.stat();

   // Save histogram on file and close file.
   multiplicity2->Write();
   delete outFile;

   return 0;
}