// fit_peak_background.C -- fit a peak (signal) on top of a falling background
//                          and count how many signal events there are
// Run with:   root -l fit_peak_background.C

void fit_peak_background()
{
   gStyle->SetOptStat(0);

   // --- 1. Fake data: 20000 background events + 2000 signal events ---
   auto h = new TH1D("h", "Signal on a background;Mass (GeV);Events / 0.1 GeV", 100, 0, 10);
   TRandom3 rng(2);
   for (int i = 0; i < 20000; i++) h->Fill(rng.Exp(3.0));       // background
   for (int i = 0; i < 2000;  i++) h->Fill(rng.Gaus(4.0, 0.3)); // signal

   // --- 2. The model: Gaussian (parameters 0,1,2) + exponential (3,4) ---
   //     gaus(0) = [0]*exp(-0.5*((x-[1])/[2])^2)
   //     expo(3) = exp([3] + [4]*x)
   auto fTotal = new TF1("fTotal", "gaus(0) + expo(3)", 0, 10);
   fTotal->SetParNames("Height", "Mean", "Sigma", "BkgConst", "BkgSlope");

   // Starting values: look at the plot and guess. Good guesses = good fits.
   fTotal->SetParameters(200, 4.2, 0.5, 6.0, -0.3);
   fTotal->SetLineColor(kRed);
   fTotal->SetNpx(500);

   // --- 3. Fit. "S" returns a TFitResultPtr with all the fit details ---
   auto c = new TCanvas("c", "Peak + background", 800, 600);
   TFitResultPtr result = h->Fit(fTotal, "S");

   // --- 4. Draw signal and background parts separately ---
   auto fSig = new TF1("fSig", "gaus", 0, 10);
   fSig->SetParameters(fTotal->GetParameter(0), fTotal->GetParameter(1),
                       fTotal->GetParameter(2));
   fSig->SetLineColor(kBlue);
   fSig->SetLineStyle(2);
   fSig->SetNpx(500);

   auto fBkg = new TF1("fBkg", "expo", 0, 10);
   fBkg->SetParameters(fTotal->GetParameter(3), fTotal->GetParameter(4));
   fBkg->SetLineColor(kGreen + 2);
   fBkg->SetLineStyle(2);

   h->SetMarkerStyle(20);
   h->SetMarkerSize(0.8);
   h->Draw("E");
   fSig->Draw("SAME");
   fBkg->Draw("SAME");

   auto leg = new TLegend(0.58, 0.68, 0.88, 0.88);
   leg->AddEntry(h, "Data", "pe");
   leg->AddEntry(fTotal, "Total fit", "l");
   leg->AddEntry(fSig, "Signal (Gaussian)", "l");
   leg->AddEntry(fBkg, "Background (exponential)", "l");
   leg->Draw();
   c->SaveAs("fit_peak_background.png");

   // --- 5. Number of signal events = area of the Gaussian / bin width ---
   double binWidth = h->GetBinWidth(1);
   double nSignal  = fSig->Integral(0, 10) / binWidth;

   // Error on the area, using the covariance matrix of the 3 signal parameters
   TMatrixDSym covSig = result->GetCovarianceMatrix().GetSub(0, 2, 0, 2);
   double nSignalErr = fSig->IntegralError(0, 10, fSig->GetParameters(),
                                           covSig.GetMatrixArray()) / binWidth;

   std::cout << "\n--- Results ---" << std::endl;
   std::cout << "Peak position  = " << fTotal->GetParameter(1) << " +- "
             << fTotal->GetParError(1) << " GeV" << std::endl;
   std::cout << "Peak width     = " << fTotal->GetParameter(2) << " +- "
             << fTotal->GetParError(2) << " GeV" << std::endl;
   std::cout << "Signal events  = " << nSignal << " +- " << nSignalErr
             << "   (we generated 2000)" << std::endl;
   std::cout << "chi2 / ndf     = " << result->Chi2() << " / " << result->Ndf() << std::endl;
}
