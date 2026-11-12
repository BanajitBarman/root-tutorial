// fit_likelihood.C -- with few entries per bin, use a likelihood fit ("L")
// Run with:   root -l fit_likelihood.C

void fit_likelihood()
{
   gStyle->SetOptStat(0);

   // Only 60 entries spread over 40 bins: many bins have 0, 1 or 2 entries
   auto h = new TH1D("h", "Few entries: chi2 vs likelihood fit;x;Entries", 40, 0, 10);
   TRandom3 rng(8);
   for (int i = 0; i < 60; i++) h->Fill(rng.Gaus(5.0, 1.5));

   auto fChi2 = new TF1("fChi2", "gaus", 0, 10);
   auto fLike = new TF1("fLike", "gaus", 0, 10);
   fChi2->SetLineColor(kRed);
   fLike->SetLineColor(kBlue);
   fLike->SetLineStyle(2);

   // Options: Q = quiet, 0 = do not draw now, L = likelihood, + = keep the
   // previous fit function attached to the histogram as well
   h->Fit(fChi2, "Q0");
   h->Fit(fLike, "L Q0 +");

   auto c = new TCanvas("c", "Likelihood", 800, 600);
   h->SetMarkerStyle(20);
   h->Draw("E");
   fChi2->Draw("SAME");
   fLike->Draw("SAME");

   auto leg = new TLegend(0.58, 0.74, 0.88, 0.88);
   leg->AddEntry(fChi2, "#chi^{2} fit (default)", "l");
   leg->AddEntry(fLike, "Likelihood fit (\"L\")", "l");
   leg->Draw();
   c->SaveAs("fit_likelihood.png");

   double bw = h->GetBinWidth(1);
   std::cout << "                 chi2 fit    likelihood fit   (true)" << std::endl;
   printf("Mean           %8.2f       %8.2f        5.00\n",
          fChi2->GetParameter(1), fLike->GetParameter(1));
   printf("Sigma          %8.2f       %8.2f        1.50\n",
          fChi2->GetParameter(2), fLike->GetParameter(2));
   printf("Events (area)  %8.1f       %8.1f       60\n",
          fChi2->Integral(0, 10) / bw, fLike->Integral(0, 10) / bw);
}
