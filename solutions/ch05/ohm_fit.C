// Solution to Exercise 5.1 -- fit a straight line and get the resistance
void ohm_fit()
{
   gStyle->SetOptFit(111);
   gStyle->SetStatX(0.88);     // statistics box in the empty lower-right corner
   gStyle->SetStatY(0.35);
   auto g = new TGraphErrors("ohm.txt");
   g->SetTitle("Ohm's law;Current I (mA);Voltage V (V)");
   g->SetMarkerStyle(20);

   auto c = new TCanvas("c", "Ohm fit", 800, 600);
   c->SetGrid();
   g->Fit("pol1");
   g->Draw("AP");
   c->SaveAs("ohm_fit.png");

   TF1 *line = g->GetFunction("pol1");
   // V in volts, I in mA, so the slope is in V/mA = kOhm
   printf("R = %.3f +- %.3f kOhm\n", line->GetParameter(1), line->GetParError(1));
}
