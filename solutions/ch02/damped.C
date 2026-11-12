// Solution to Exercise 2.2 -- draw exp(-x/3)*cos(x) and save it as PNG and PDF
void damped()
{
   auto c = new TCanvas("c", "Damped oscillation", 800, 600);
   auto f = new TF1("f", "exp(-x/3)*cos(x)", 0, 15);
   f->SetTitle("Damped oscillation;Time t (s);Amplitude");
   f->SetLineColor(kRed);
   f->SetLineWidth(3);
   f->SetNpx(300);
   f->Draw();
   c->SaveAs("damped.png");
   c->SaveAs("damped.pdf");
}
