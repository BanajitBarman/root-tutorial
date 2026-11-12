// hist_draw_options.C -- the same histogram drawn with four draw options
// Run with:   root -l hist_draw_options.C

void hist_draw_options()
{
   gStyle->SetOptStat(0);   // no statistics box, to keep the plots clean

   auto h = new TH1D("h", "Draw options;x;Entries", 20, -4, 4);
   TRandom3 rng(1);
   for (int i = 0; i < 2000; i++) h->Fill(rng.Gaus(0, 1));

   auto c = new TCanvas("c", "Draw options", 1000, 800);
   c->Divide(2, 2);        // split the canvas into 2 x 2 pads

   // DrawClone draws a copy, so changing the style later does not
   // change the plots we already made.
   c->cd(1);                                   // go to pad 1
   h->SetTitle("Option \"HIST\" (bars outline)");
   h->DrawClone("HIST");

   c->cd(2);
   h->SetTitle("Option \"E1\" (points with error bars)");
   h->SetMarkerStyle(20);
   h->DrawClone("E1");

   c->cd(3);
   h->SetTitle("Option \"BAR\" (filled bars)");
   h->SetFillColor(kOrange - 3);
   h->SetBarWidth(0.8);
   h->SetBarOffset(0.1);
   h->DrawClone("BAR");

   c->cd(4);
   h->SetTitle("Option \"C\" (smooth curve)");
   h->SetFillStyle(0);            // no fill for this one
   h->SetLineColor(kRed);
   h->SetLineWidth(3);
   h->DrawClone("C");

   c->SaveAs("hist_draw_options.png");
}
