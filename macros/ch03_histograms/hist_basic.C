// hist_basic.C -- create, fill, draw and save a 1D histogram
// Run with:   root -l hist_basic.C

void hist_basic()
{
   // 1. Create the histogram:
   //    name, "title;x-axis title;y-axis title", number of bins, x-min, x-max
   auto h = new TH1D("h", "Gaussian random numbers;x;Entries", 50, -5, 5);

   // 2. Fill it with 10000 random numbers from a Gaussian (mean 0, sigma 1)
   TRandom3 rng(42);                 // random-number generator, seed = 42
   for (int i = 0; i < 10000; i++) {
      h->Fill(rng.Gaus(0, 1));
   }

   // 3. Make it look nice, draw it and save the picture
   auto c = new TCanvas("c", "Histogram", 800, 600);
   h->SetLineColor(kBlue + 1);
   h->SetLineWidth(2);
   h->SetFillColor(kAzure - 9);
   h->Draw();
   c->SaveAs("hist_basic.png");

   // 4. Ask the histogram about itself
   std::cout << "Entries : " << h->GetEntries() << std::endl;
   std::cout << "Mean    : " << h->GetMean()    << std::endl;
   std::cout << "Std Dev : " << h->GetStdDev()  << std::endl;
   std::cout << "Bin with x = 1.0 is bin number " << h->FindBin(1.0)
             << ", it has " << h->GetBinContent(h->FindBin(1.0))
             << " entries" << std::endl;
}
