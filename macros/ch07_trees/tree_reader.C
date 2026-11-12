// tree_reader.C -- the modern, safer way to loop over a tree: TTreeReader
// Run with:   root -l tree_reader.C      (run tree_write.C first)

void tree_reader()
{
   TFile *file = TFile::Open("toy_tree.root");
   TTreeReader reader("events", file);

   // The type is checked: a wrong type gives a clear error message
   TTreeReaderValue<double> energy(reader, "energy");
   TTreeReaderValue<int>    nHits(reader, "nHits");
   TTreeReaderArray<double> hitEnergy(reader, "hitEnergy");   // vector branch

   int nEvents = 0, nWithBigHit = 0;
   double sumMaxFraction = 0;

   while (reader.Next()) {               // loops over all events
      nEvents++;
      // *energy gives the value; hitEnergy works like an array
      double maxHit = 0;
      for (double e : hitEnergy) {
         if (e > maxHit) maxHit = e;
      }
      if (maxHit > 10) nWithBigHit++;
      sumMaxFraction += maxHit / *energy;
   }

   std::cout << "Events read                       : " << nEvents << std::endl;
   std::cout << "Events with a hit above 10 GeV    : " << nWithBigHit << std::endl;
   std::cout << "Average (largest hit / total E)   : " << sumMaxFraction / nEvents << std::endl;
}
