// check_root.C -- check that ROOT works on your computer
// Run with:   root -l check_root.C
// If it works you will see a plot, and check_root.png appears in this folder.

void check_root()
{
   std::cout << "ROOT version: " << gROOT->GetVersion() << std::endl;

   auto h = new TH1D("h", "ROOT works!;x;Entries", 40, -4, 4);
   h->FillRandom("gaus", 5000);

   auto c = new TCanvas("c", "Installation check", 800, 600);
   h->SetFillColor(kGreen - 9);
   h->Draw();
   c->SaveAs("check_root.png");

   std::cout << "Everything is fine. You are ready for the tutorial." << std::endl;
}
