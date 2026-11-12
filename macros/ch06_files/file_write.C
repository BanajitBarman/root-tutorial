// file_write.C -- save histograms and a graph into a ROOT file
// Run with:   root -l file_write.C

void file_write()
{
   // Open (create) a file. "RECREATE" = create it, overwrite if it exists.
   TFile *file = TFile::Open("my_results.root", "RECREATE");

   // Histograms created while a file is open belong to that file
   auto hEnergy = new TH1D("hEnergy", "Deposited energy;E (MeV);Events", 100, 0, 100);
   auto hAngle  = new TH1D("hAngle", "Polar angle;#theta (rad);Events", 50, 0, TMath::Pi());
   TRandom3 rng(4);
   for (int i = 0; i < 10000; i++) {
      hEnergy->Fill(rng.Landau(20, 4));        // energy loss in a thin detector
      hAngle->Fill(acos(rng.Uniform(-1, 1)));  // directions uniform on a sphere
   }

   // A folder (directory) inside the file
   TDirectory *calib = file->mkdir("calibration");
   calib->cd();                              // new histograms now go into this folder
   auto hGain = new TH1D("hGain", "Channel gains;Gain;Channels", 40, 0.8, 1.2);
   for (int i = 0; i < 500; i++) hGain->Fill(rng.Gaus(1.0, 0.05));

   // Other objects (graphs, functions, ...) do not belong to the file
   // automatically: write them yourself, giving them a name.
   file->cd();                               // back to the top of the file
   auto g = new TGraph();
   for (int i = 0; i < 10; i++) g->SetPoint(i, i, 2.0 * i + rng.Gaus(0, 1));
   g->SetTitle("Detector response;Input;Output");
   g->Write("gResponse");

   file->Write();     // write all histograms that belong to the file
   file->Close();     // close it -- this also deletes those histograms from memory!

   std::cout << "Wrote my_results.root" << std::endl;
}
