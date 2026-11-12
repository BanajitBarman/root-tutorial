// tree_draw.C -- quick plots straight from a TTree with TTree::Draw
// Run with:   root -l tree_draw.C      (run tree_write.C first)

void tree_draw()
{
   TFile *file = TFile::Open("toy_tree.root");
   auto tree = file->Get<TTree>("events");

   auto c = new TCanvas("c", "TTree::Draw", 1000, 800);
   c->Divide(2, 2);

   // 1. One variable. ">>hE(50,0,100)" names the histogram and sets the bins
   c->cd(1);
   gPad->SetLogy();                      // logarithmic y axis
   tree->Draw("energy>>hE(50,0,100)");
   auto hE = (TH1F *)gDirectory->Get("hE");
   hE->SetTitle("All events;Energy (GeV);Events");

   // 2. With a cut (selection): only events with more than 20 hits
   c->cd(2);
   tree->Draw("energy>>hECut(50,0,100)", "nHits > 20");
   auto hECut = (TH1F *)gDirectory->Get("hECut");
   hECut->SetTitle("Events with nHits > 20;Energy (GeV);Events");
   hECut->SetLineColor(kRed);

   // 3. Two variables "y:x" (y first!) -> 2D plot
   c->cd(3);
   tree->Draw("nHits:energy>>h2(50,0,100,50,0,60)", "", "COLZ");
   auto h2 = (TH2F *)gDirectory->Get("h2");
   h2->SetTitle("Hits vs energy;Energy (GeV);Number of hits");
   h2->SetStats(0);                      // no statistics box on this one
   gPad->SetRightMargin(0.15);

   // 4. Any formula of the branches; vectors are drawn element by element
   c->cd(4);
   tree->Draw("sqrt(x*x + y*y)>>hR(50,0,40)");
   auto hR = (TH1F *)gDirectory->Get("hR");
   hR->SetTitle("Distance from the centre;r = #sqrt{x^{2}+y^{2}} (cm);Events");

   c->SaveAs("tree_draw.png");

   std::cout << "Events with nHits > 20: " << hECut->GetEntries() << std::endl;
   std::cout << "Mean distance r: " << hR->GetMean() << " cm" << std::endl;

   // Counting without drawing: GetEntries with a cut
   std::cout << "Events with energy > 50 GeV: "
             << tree->GetEntries("energy > 50") << std::endl;
}
