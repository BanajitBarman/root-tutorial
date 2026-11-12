// Solution to Exercise 6.1 -- write histograms into a folder of a file, read one back
void clt_write_read()
{
   // ---- write ----
   TFile *out = TFile::Open("clt.root", "RECREATE");
   TDirectory *dir = out->mkdir("sums");
   dir->cd();
   TRandom3 rng(1);
   int nValues[4] = {1, 2, 3, 12};
   for (int N : nValues) {
      auto h = new TH1D(Form("hSum%d", N), Form("Sum of %d uniform numbers;sum;Entries", N),
                        60, 0, N);
      for (int i = 0; i < 100000; i++) {
         double sum = 0;
         for (int j = 0; j < N; j++) sum += rng.Uniform();
         h->Fill(sum);
      }
   }
   out->Write();
   out->Close();

   // ---- read back ----
   TFile *in = TFile::Open("clt.root");
   in->ls();
   auto h12 = in->Get<TH1D>("sums/hSum12");
   if (!h12) { std::cout << "hSum12 not found!" << std::endl; return; }
   std::cout << "hSum12: mean = " << h12->GetMean() << " (expected 6), std dev = "
             << h12->GetStdDev() << " (expected 1)" << std::endl;
   auto c = new TCanvas("c", "Read back", 800, 600);
   h12->Draw();
   c->SaveAs("clt_read.png");
}
