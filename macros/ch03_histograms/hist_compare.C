// hist_compare.C -- two histograms on one canvas, normalised, with a legend
// Run with:   root -l hist_compare.C

void hist_compare()
{
   gStyle->SetOptStat(0);           // hide the statistics box

   auto hNarrow = new TH1D("hNarrow", "Comparing two distributions;x;Probability", 60, -6, 8);
   auto hWide   = new TH1D("hWide",   "Wide", 60, -6, 8);

   TRandom3 rng(7);
   for (int i = 0; i < 10000; i++) hNarrow->Fill(rng.Gaus(0, 1));
   for (int i = 0; i < 5000;  i++) hWide->Fill(rng.Gaus(2, 2));

   // Normalise both to area 1, so we compare shapes (not number of entries)
   hNarrow->Scale(1.0 / hNarrow->Integral());
   hWide->Scale(1.0 / hWide->Integral());

   hNarrow->SetLineColor(kBlue);  hNarrow->SetLineWidth(2);
   hWide->SetLineColor(kRed);     hWide->SetLineWidth(2);
   hWide->SetFillColorAlpha(kRed, 0.2);       // transparent fill

   // The FIRST histogram drawn sets the axis range, so make room for both
   double ymax = std::max(hNarrow->GetMaximum(), hWide->GetMaximum());
   hNarrow->SetMaximum(1.2 * ymax);

   auto c = new TCanvas("c", "Compare", 800, 600);
   hNarrow->Draw("HIST");
   hWide->Draw("HIST SAME");                  // SAME = draw on top

   auto leg = new TLegend(0.62, 0.76, 0.88, 0.88);   // x1,y1,x2,y2 (fractions)
   leg->AddEntry(hNarrow, "#mu = 0, #sigma = 1", "l");
   leg->AddEntry(hWide,   "#mu = 2, #sigma = 2", "lf");
   leg->Draw();

   c->SaveAs("hist_compare.png");
}
