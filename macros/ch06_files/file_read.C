// file_read.C -- open a ROOT file, list it, read objects back and draw them
// Run with:   root -l file_read.C      (run file_write.C first)

void file_read()
{
   TFile *file = TFile::Open("my_results.root");      // read-only by default
   if (!file || file->IsZombie()) {
      std::cout << "Cannot open my_results.root -- run file_write.C first" << std::endl;
      return;
   }

   file->ls();                                        // what is inside?

   // Get<Type>("name") returns a pointer, or nullptr if the name is wrong
   auto hEnergy = file->Get<TH1D>("hEnergy");
   auto hGain   = file->Get<TH1D>("calibration/hGain");   // folder/name
   auto gResp   = file->Get<TGraph>("gResponse");

   if (!hEnergy || !hGain || !gResp) {
      std::cout << "Something is missing in the file!" << std::endl;
      return;
   }
   std::cout << "hEnergy: " << hEnergy->GetEntries() << " entries, mean "
             << hEnergy->GetMean() << " MeV" << std::endl;

   auto c = new TCanvas("c", "Read back", 1200, 400);
   c->Divide(3, 1);
   c->cd(1);  hEnergy->Draw();
   c->cd(2);  hGain->Draw();
   c->cd(3);  gResp->SetMarkerStyle(20);  gResp->Draw("AP");
   c->SaveAs("file_read.png");

   // We do NOT close the file here: closing it would delete hEnergy and hGain
   // and they would vanish from the canvas. (See the chapter text.)
}
