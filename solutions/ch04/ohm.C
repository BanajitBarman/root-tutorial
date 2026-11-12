// Solution to Exercise 4.2 -- plot Ohm's-law measurements with error bars
void ohm()
{
   auto g = new TGraphErrors("ohm.txt");
   g->SetTitle("Ohm's law;Current I (mA);Voltage V (V)");
   g->SetMarkerStyle(20);
   g->SetMarkerColor(kBlue + 1);

   auto c = new TCanvas("c", "Ohm", 800, 600);
   c->SetGrid();
   g->Draw("AP");
   c->SaveAs("ohm.png");
}
