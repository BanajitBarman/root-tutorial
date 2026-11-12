// Solution to Exercise 5.2 -- measure the muon lifetime with an exponential fit
void muon_lifetime()
{
   gStyle->SetOptFit(111);
   auto h = new TH1D("h", "Muon decay times;t (#mus);Decays / 0.25 #mus", 40, 0, 10);
   TRandom3 rng(3);
   for (int i = 0; i < 5000; i++) h->Fill(rng.Exp(2.197));   // true lifetime 2.197 us

   auto c = new TCanvas("c", "Muon lifetime", 800, 600);
   c->SetLogy();
   h->Fit("expo", "L");          // expo = exp([0] + [1]*t); likelihood fit
   h->SetMarkerStyle(20);
   h->Draw("E");
   c->SaveAs("muon_lifetime.png");

   TF1 *f = h->GetFunction("expo");
   double slope = f->GetParameter(1), slopeErr = f->GetParError(1);
   double tau = -1.0 / slope;
   double tauErr = tau * slopeErr / std::abs(slope);
   printf("Lifetime = %.3f +- %.3f us   (true value 2.197 us)\n", tau, tauErr);
}
