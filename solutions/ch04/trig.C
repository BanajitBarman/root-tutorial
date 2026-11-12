// Solution to Exercise 4.1 -- three functions with a legend, and an integral
void trig()
{
   double twoPi = 2 * TMath::Pi();
   auto f1 = new TF1("f1", "sin(x)", 0, twoPi);
   auto f2 = new TF1("f2", "cos(x)", 0, twoPi);
   auto f3 = new TF1("f3", "sin(x)*cos(x)", 0, twoPi);
   f1->SetLineColor(kBlue);
   f2->SetLineColor(kRed);
   f3->SetLineColor(kGreen + 2);
   f3->SetLineStyle(2);

   auto c = new TCanvas("c", "Trig", 800, 600);
   c->SetGrid();
   f1->SetTitle("Trigonometric functions;x (rad);f(x)");
   f1->SetMinimum(-1.2);
   f1->SetMaximum(1.6);
   f1->Draw();
   f2->Draw("SAME");
   f3->Draw("SAME");

   auto leg = new TLegend(0.65, 0.75, 0.88, 0.88);
   leg->AddEntry(f1, "sin(x)", "l");
   leg->AddEntry(f2, "cos(x)", "l");
   leg->AddEntry(f3, "sin(x) cos(x)", "l");
   leg->Draw();
   c->SaveAs("trig.png");

   auto f4 = new TF1("f4", "sin(x)^2", 0, TMath::Pi());
   std::cout << "Integral of sin^2 from 0 to pi = " << f4->Integral(0, TMath::Pi())
             << "  (pi/2 = " << TMath::Pi() / 2 << ")" << std::endl;
}
