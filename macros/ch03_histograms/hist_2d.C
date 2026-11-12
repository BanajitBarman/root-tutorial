// hist_2d.C -- a 2D histogram drawn two ways
// Run with:   root -l hist_2d.C

void hist_2d()
{
   gStyle->SetOptStat(0);

   // name, title, nbins-x, x-min, x-max, nbins-y, y-min, y-max
   auto h2 = new TH2D("h2", "Correlated variables;x;y",
                      40, -4, 4, 40, -4, 4);

   TRandom3 rng(3);
   for (int i = 0; i < 50000; i++) {
      double x = rng.Gaus(0, 1);
      double y = 0.6 * x + 0.8 * rng.Gaus(0, 1);  // y depends on x
      h2->Fill(x, y);
   }

   auto c = new TCanvas("c", "2D histogram", 1200, 550);
   c->Divide(2, 1);

   c->cd(1);
   gPad->SetRightMargin(0.15);        // room for the colour scale
   h2->Draw("COLZ");                  // colour map + colour scale (Z)

   c->cd(2);
   h2->DrawClone("LEGO2");            // 3D-looking bars

   c->SaveAs("hist_2d.png");

   std::cout << "Correlation factor = " << h2->GetCorrelationFactor() << std::endl;

   // Projections: squash the 2D histogram into 1D
   TH1D *px = h2->ProjectionX("px");
   std::cout << "Projection on x: mean = " << px->GetMean()
             << ", std dev = " << px->GetStdDev() << std::endl;
}
