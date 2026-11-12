// hist_operations.C -- adding, scaling, dividing and rebinning histograms
// Run with:   root -l hist_operations.C

void hist_operations()
{
   gStyle->SetOptStat(0);
   TRandom3 rng(11);

   // "All" particles and "detected" particles. The detector misses many
   // low-energy particles, so the detection probability depends on energy.
   auto hAll = new TH1D("hAll", "All particles;Energy (GeV);Particles", 20, 0, 10);
   auto hDet = new TH1D("hDet", "Detected particles;Energy (GeV);Particles", 20, 0, 10);
   hAll->Sumw2();   // keep track of errors properly (do this before filling)
   hDet->Sumw2();

   for (int i = 0; i < 20000; i++) {
      double E = rng.Uniform(0, 10);
      hAll->Fill(E);
      double probability = 1.0 - exp(-E / 2.0);   // the "true" efficiency
      if (rng.Uniform() < probability) hDet->Fill(E);
   }

   // Bin-by-bin information
   int bin = hAll->FindBin(5.2);
   std::cout << "Energy 5.2 GeV is in bin " << bin
             << " (from " << hAll->GetBinLowEdge(bin)
             << " to " << hAll->GetBinLowEdge(bin) + hAll->GetBinWidth(bin)
             << " GeV, centre " << hAll->GetBinCenter(bin) << ")" << std::endl;
   std::cout << "Bin " << bin << ": all = " << hAll->GetBinContent(bin)
             << ", detected = " << hDet->GetBinContent(bin) << std::endl;

   // Integrals (sums of bin contents)
   std::cout << "Total detected            : " << hDet->Integral() << std::endl;
   std::cout << "Detected with E in [0, 2] : "
             << hDet->Integral(hDet->FindBin(0.01), hDet->FindBin(1.99)) << std::endl;

   // Clone + Divide: efficiency = detected / all  ("B" = binomial errors)
   auto hEff = (TH1D *)hDet->Clone("hEff");
   hEff->Divide(hDet, hAll, 1.0, 1.0, "B");
   hEff->SetTitle("Detection efficiency;Energy (GeV);Efficiency");

   // Add and Scale
   auto hSum = (TH1D *)hAll->Clone("hSum");
   hSum->Add(hDet);              // hSum = hAll + hDet
   hSum->Scale(0.5);             // multiply every bin by 0.5
   std::cout << "Entries in (all + detected) / 2 : " << hSum->Integral() << std::endl;

   // Rebin: merge every 2 neighbouring bins
   auto hCoarse = (TH1D *)hAll->Rebin(2, "hCoarse");
   std::cout << "Bins before rebin: " << hAll->GetNbinsX()
             << ", after: " << hCoarse->GetNbinsX() << std::endl;

   auto c = new TCanvas("c", "Efficiency", 800, 600);
   hEff->SetMarkerStyle(20);
   hEff->SetMarkerColor(kBlue + 1);
   hEff->SetLineColor(kBlue + 1);
   hEff->SetMinimum(0);
   hEff->SetMaximum(1.1);
   hEff->Draw("E1");

   // Draw the true efficiency on top for comparison
   auto fTrue = new TF1("fTrue", "1 - exp(-x/2)", 0, 10);
   fTrue->SetLineColor(kRed);
   fTrue->SetLineStyle(2);       // dashed
   fTrue->Draw("SAME");

   auto leg = new TLegend(0.45, 0.20, 0.88, 0.32);
   leg->AddEntry(hEff, "Measured (detected / all)", "pe");
   leg->AddEntry(fTrue, "True efficiency 1 - e^{-E/2}", "l");
   leg->Draw();

   c->SaveAs("hist_operations.png");
}
