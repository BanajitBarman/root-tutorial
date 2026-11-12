// tree_loop.C -- read a TTree event by event in a loop (SetBranchAddress)
// Run with:   root -l tree_loop.C      (run tree_write.C first)

void tree_loop()
{
   TFile *file = TFile::Open("toy_tree.root");
   auto tree = file->Get<TTree>("events");

   // 1. Variables of the SAME type as the branches
   double energy, x, y;
   int    nHits;

   // 2. Tell the tree where to put the values: SetBranchAddress(name, &variable)
   tree->SetBranchAddress("energy", &energy);
   tree->SetBranchAddress("x", &x);
   tree->SetBranchAddress("y", &y);
   tree->SetBranchAddress("nHits", &nHits);

   auto hR = new TH1D("hR", "Distance from centre, E > 30 GeV;r (cm);Events", 40, 0, 40);
   auto hEperHit = new TH1D("hEperHit", "Energy per hit;E / nHits (GeV);Events", 50, 0, 5);

   // 3. Loop: GetEntry(i) fills the variables with the values of event i
   int nSelected = 0;
   Long64_t nEntries = tree->GetEntries();
   for (Long64_t i = 0; i < nEntries; i++) {
      tree->GetEntry(i);

      hEperHit->Fill(energy / nHits);

      if (energy < 30) continue;          // skip low-energy events
      nSelected++;
      double r = sqrt(x * x + y * y);
      hR->Fill(r);
   }
   std::cout << "Selected " << nSelected << " of " << nEntries << " events" << std::endl;

   auto c = new TCanvas("c", "Event loop", 1200, 500);
   c->Divide(2, 1);
   c->cd(1);  hR->Draw();
   c->cd(2);  hEperHit->SetLineColor(kRed);  hEperHit->Draw();
   c->SaveAs("tree_loop.png");
}
