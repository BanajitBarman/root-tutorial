// fit_graph.C -- fit a straight line to a graph and measure g
// Run with:   root -l fit_graph.C     (pendulum.txt must be in the same folder)
//
// Physics: T = 2 pi sqrt(L/g)  =>  T^2 = (4 pi^2 / g) * L
// So T^2 against L is a straight line with slope 4 pi^2 / g.

void fit_graph()
{
   gStyle->SetOptFit(111);
   gStyle->SetStatX(0.88);     // move the statistics box to the empty
   gStyle->SetStatY(0.35);     // lower-right corner

   auto data = new TGraphErrors("pendulum.txt");   // L, T, dL, dT

   // Build a new graph of T^2 against L
   auto g = new TGraphErrors();
   for (int i = 0; i < data->GetN(); i++) {
      double L  = data->GetPointX(i);
      double T  = data->GetPointY(i);
      double dL = data->GetErrorX(i);
      double dT = data->GetErrorY(i);
      g->SetPoint(i, L, T * T);
      g->SetPointError(i, dL, 2 * T * dT);     // error of T^2 is 2 T dT
   }
   g->SetTitle("Pendulum: T^{2} against L;Length L (m);T^{2} (s^{2})");
   g->SetMarkerStyle(20);
   g->SetMarkerColor(kBlue + 1);

   auto c = new TCanvas("c", "Graph fit", 800, 600);
   c->SetGrid();
   g->Fit("pol1");        // straight line: [0] + [1]*x
   g->Draw("AP");

   TF1 *line = g->GetFunction("pol1");
   double slope    = line->GetParameter(1);
   double slopeErr = line->GetParError(1);
   double gValue   = 4 * TMath::Pi() * TMath::Pi() / slope;
   double gError   = gValue * slopeErr / slope;    // error propagation

   auto text = new TLatex();
   text->SetNDC();
   text->SetTextFont(42);
   text->SetTextSize(0.045);
   text->DrawLatex(0.15, 0.80, Form("g = %.2f #pm %.2f m/s^{2}", gValue, gError));
   c->SaveAs("fit_graph.png");

   std::cout << "\nSlope = " << slope << " +- " << slopeErr << " s^2/m" << std::endl;
   std::cout << "g     = " << gValue << " +- " << gError << " m/s^2" << std::endl;
}
