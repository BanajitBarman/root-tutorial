// fit_gaus.C -- fit a histogram with a Gaussian and read the results
// Run with:   root -l fit_gaus.C

void fit_gaus()
{
   gStyle->SetOptFit(1111);    // show fit results in the statistics box
   gStyle->SetStatW(0.25);     // statistics box width and height
   gStyle->SetStatH(0.20);     //   (as fractions of the canvas)

   auto h = new TH1D("h", "Measured mass;Mass (GeV);Events", 50, 0, 10);
   TRandom3 rng(5);
   for (int i = 0; i < 1000; i++) h->Fill(rng.Gaus(5.0, 1.5));

   h->SetMaximum(1.6 * h->GetMaximum());   // room for the statistics box

   auto c = new TCanvas("c", "Gaussian fit", 800, 600);

   // Fit with the built-in Gaussian. ROOT prints the result and draws
   // the fitted curve on top of the histogram.
   h->Fit("gaus");
   h->SetMarkerStyle(20);
   h->Draw("E");               // draw again with error bars (the fit is kept)

   c->SaveAs("fit_gaus.png");

   // Get the fitted function and its parameters
   TF1 *fit = h->GetFunction("gaus");
   std::cout << "\n--- Results ---" << std::endl;
   std::cout << "Mean  = " << fit->GetParameter(1) << " +- " << fit->GetParError(1) << std::endl;
   std::cout << "Sigma = " << fit->GetParameter(2) << " +- " << fit->GetParError(2) << std::endl;
   std::cout << "chi2 / ndf = " << fit->GetChisquare() << " / " << fit->GetNDF()
             << " = " << fit->GetChisquare() / fit->GetNDF() << std::endl;
   std::cout << "Fit probability = " << fit->GetProb() << std::endl;
}
