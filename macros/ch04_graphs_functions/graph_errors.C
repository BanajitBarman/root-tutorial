// graph_errors.C -- read measurements with errors from a text file and
//                   compare them with a theory curve
// Run with:   root -l graph_errors.C     (pendulum.txt must be in the same folder)

void graph_errors()
{
   // Reads 4 columns: x  y  error-x  error-y   (lines starting with # are skipped)
   auto g = new TGraphErrors("pendulum.txt");
   g->SetTitle("Simple pendulum;Length L (m);Period T (s)");
   g->SetMarkerStyle(20);
   g->SetMarkerColor(kBlue + 1);
   g->SetLineColor(kBlue + 1);

   // Theory: T = 2 pi sqrt(L / g), with g = 9.81 m/s^2
   auto theory = new TF1("theory", "2*TMath::Pi()*sqrt(x/9.81)", 0.1, 1.1);
   theory->SetLineColor(kRed);
   theory->SetLineStyle(2);

   auto c = new TCanvas("c", "Pendulum", 800, 600);
   c->SetGrid();
   g->Draw("AP");
   theory->Draw("SAME");

   auto leg = new TLegend(0.15, 0.77, 0.45, 0.88);
   leg->AddEntry(g, "Measurement", "pe");
   leg->AddEntry(theory, "T = 2#pi #sqrt{L/g}", "l");
   leg->Draw();

   // Free text on the plot. NDC = coordinates as fractions of the pad (0..1)
   auto text = new TLatex();
   text->SetNDC();
   text->SetTextFont(42);          // normal (not bold) Helvetica
   text->SetTextSize(0.04);
   text->DrawLatex(0.55, 0.20, "g = 9.81 m/s^{2}");

   c->SaveAs("graph_errors.png");
   std::cout << "Read " << g->GetN() << " points from pendulum.txt" << std::endl;
}
