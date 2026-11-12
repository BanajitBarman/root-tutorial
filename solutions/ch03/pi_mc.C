// Solution to Exercise 3.2 -- estimate pi with random points
// Points uniform in the square [-1,1] x [-1,1]. The fraction inside the
// circle x^2 + y^2 < 1 is (area of circle) / (area of square) = pi / 4.
void pi_mc()
{
   TRandom3 rng(3);
   auto hR2 = new TH1D("hR2", "x^{2} + y^{2} of the random points;x^{2} + y^{2};Points", 40, 0, 2);
   auto gIn  = new TGraph();       // for the picture: points inside ...
   auto gOut = new TGraph();       // ... and outside the circle

   int nTotal = 1000000, nInside = 0;
   for (int i = 0; i < nTotal; i++) {
      double x = rng.Uniform(-1, 1);
      double y = rng.Uniform(-1, 1);
      double r2 = x * x + y * y;
      hR2->Fill(r2);
      bool inside = (r2 < 1);
      if (inside) nInside++;
      if (i < 5000) {              // only draw the first 5000 points
         if (inside) gIn->AddPoint(x, y);
         else        gOut->AddPoint(x, y);
      }
   }
   double piEstimate = 4.0 * nInside / nTotal;
   double piError = 4.0 * sqrt(nInside * (1.0 - double(nInside) / nTotal)) / nTotal;
   printf("pi = %.4f +- %.4f   (true value %.4f)\n", piEstimate, piError, TMath::Pi());

   auto c = new TCanvas("c", "pi", 1200, 600);
   c->Divide(2, 1);
   c->cd(1);
   gIn->SetTitle("First 5000 points;x;y");
   gIn->SetMarkerStyle(7);
   gIn->SetMarkerColor(kBlue);
   gOut->SetMarkerStyle(7);
   gOut->SetMarkerColor(kRed);
   gIn->GetXaxis()->SetLimits(-1, 1);
   gIn->SetMinimum(-1);
   gIn->SetMaximum(1);
   gIn->Draw("AP");
   gOut->Draw("P");
   c->cd(2);
   hR2->SetMinimum(0);
   hR2->Draw();            // flat below 1 (inside the circle), falling above
   c->SaveAs("pi_mc.png");
}
