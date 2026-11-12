// Solution to Exercise 3.1 -- the central limit theorem
// The sum of N uniform random numbers looks more and more Gaussian as N grows.
void clt()
{
   int nValues[4] = {1, 2, 3, 12};
   TRandom3 rng(1);
   auto c = new TCanvas("c", "Central limit theorem", 1000, 800);
   c->Divide(2, 2);

   for (int k = 0; k < 4; k++) {
      int N = nValues[k];
      auto h = new TH1D(Form("hSum%d", N), Form("Sum of %d uniform numbers;sum;Entries", N),
                        60, 0, N);
      for (int i = 0; i < 100000; i++) {
         double sum = 0;
         for (int j = 0; j < N; j++) sum += rng.Uniform(0, 1);
         h->Fill(sum);
      }
      c->cd(k + 1);
      h->SetFillColor(kAzure - 9);
      h->SetMinimum(0);
      h->Draw();
      if (N > 1) h->Fit("gaus", "Q");   // compare with a Gaussian
   }
   c->SaveAs("clt.png");
}
