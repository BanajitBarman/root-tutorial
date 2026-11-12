// tree_write.C -- create a TTree with one entry per "event" and save it
// Run with:   root -l tree_write.C

void tree_write()
{
   TFile *file = TFile::Open("toy_tree.root", "RECREATE");
   auto tree = new TTree("events", "Toy calorimeter events");

   // 1. Variables that hold the values of ONE event
   int    eventNumber;
   int    nHits;
   double energy;                    // total energy (GeV)
   double x, y;                      // impact position (cm)
   std::vector<double> hitEnergy;    // energy of each hit -- a different
                                     // number of values in every event

   // 2. Connect each variable to a branch: name, address of the variable
   tree->Branch("eventNumber", &eventNumber);
   tree->Branch("nHits", &nHits);
   tree->Branch("energy", &energy);
   tree->Branch("x", &x);
   tree->Branch("y", &y);
   tree->Branch("hitEnergy", &hitEnergy);

   // 3. Event loop: set the variables, then call Fill() to store the event
   TRandom3 rng(1);
   for (int i = 0; i < 50000; i++) {
      eventNumber = i;
      energy = rng.Exp(20.0);                 // falling energy spectrum
      x      = rng.Gaus(0, 10);
      y      = rng.Gaus(0, 10);
      nHits  = 1 + rng.Poisson(energy / 2);   // more energy -> more hits

      // share the energy randomly between the hits
      hitEnergy.clear();
      std::vector<double> w(nHits);
      double sumW = 0;
      for (int k = 0; k < nHits; k++) { w[k] = rng.Exp(1.0); sumW += w[k]; }
      for (int k = 0; k < nHits; k++) hitEnergy.push_back(energy * w[k] / sumW);

      tree->Fill();
   }

   // 4. Write the tree to the file and close it
   tree->Write();
   std::cout << "Wrote " << tree->GetEntries() << " events to toy_tree.root" << std::endl;
   file->Close();
}
