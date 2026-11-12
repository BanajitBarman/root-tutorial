// functions.C -- three ways to make a function (TF1), and what you can do with it
// Run with:   root -l functions.C

// Way 3: write the function in C++. x[0] is the variable, p[0], p[1]... are parameters
double fermi(double *x, double *p)
{
   return p[0] / (1.0 + exp((x[0] - p[1]) / p[2]));
}

void functions()
{
   // Way 1: a formula with parameters [0], [1], [2]
   auto f1 = new TF1("f1", "[0]*exp(-x/[1])*cos([2]*x)", 0, 10);
   f1->SetParameters(1.0, 3.0, 4.0);          // amplitude, decay length, frequency
   f1->SetParNames("A", "tau", "omega");

   // Way 2: a built-in function: "gaus", "expo", "pol0".."pol9", "landau"
   auto f2 = new TF1("f2", "gaus", 0, 10);    // [0]*exp(-0.5*((x-[1])/[2])^2)
   f2->SetParameters(0.8, 5.0, 0.7);          // height, mean, sigma

   // Way 3: from the C++ function above: name, function, x-min, x-max, n-parameters
   auto f3 = new TF1("f3", fermi, 0, 10, 3);
   f3->SetParameters(1.0, 7.0, 0.4);

   f1->SetLineColor(kBlue);    f1->SetLineWidth(3);
   f2->SetLineColor(kRed);     f2->SetLineWidth(3);
   f3->SetLineColor(kGreen + 2); f3->SetLineWidth(3); f3->SetLineStyle(2);
   f1->SetNpx(500);            // draw with 500 points: smoother curve
   f2->SetNpx(500);

   auto c = new TCanvas("c", "Functions", 800, 600);
   f1->SetTitle("Three kinds of TF1;x;f(x)");
   f1->SetMinimum(-1.0);
   f1->SetMaximum(1.9);       // leave room for the legend
   f1->Draw();
   f2->Draw("SAME");
   f3->Draw("SAME");

   auto leg = new TLegend(0.45, 0.74, 0.88, 0.88);
   leg->AddEntry(f1, "formula: A e^{-x/#tau} cos(#omega x)", "l");
   leg->AddEntry(f2, "built-in: gaus", "l");
   leg->AddEntry(f3, "C++ function: Fermi step", "l");
   leg->Draw();
   c->SaveAs("functions.png");

   // Things you can calculate with a TF1
   std::cout << "f2(5.5)                 = " << f2->Eval(5.5)          << std::endl;
   std::cout << "Integral of f2 (0..10)  = " << f2->Integral(0, 10)    << std::endl;
   std::cout << "  (expected 0.8*0.7*sqrt(2 pi) = " << 0.8 * 0.7 * sqrt(2 * TMath::Pi()) << ")" << std::endl;
   std::cout << "Derivative of f1 at x=1 = " << f1->Derivative(1.0)  << std::endl;
   std::cout << "Maximum of f2 is at x   = " << f2->GetMaximumX()     << std::endl;
   std::cout << "f3 = 0.5 at x           = " << f3->GetX(0.5)         << std::endl;
}
