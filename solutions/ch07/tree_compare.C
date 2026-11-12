// Solution to Exercise 7.1 -- compare two selections from a tree, normalised
void tree_compare()
{
   gStyle->SetOptStat(0);
   TFile *file = TFile::Open("../../macros/ch07_trees/toy_tree.root");
   auto tree = file->Get<TTree>("events");

   // "goff" = graphics off: fill the histograms without drawing them
   tree->Draw("energy>>hAll(50,0,100)", "", "goff");
   tree->Draw("energy>>hMany(50,0,100)", "nHits > 20", "goff");
   auto hAll  = (TH1F *)gDirectory->Get("hAll");
   auto hMany = (TH1F *)gDirectory->Get("hMany");

   hAll->Scale(1.0 / hAll->Integral());
   hMany->Scale(1.0 / hMany->Integral());
   hAll->SetTitle("Energy, normalised to unit area;Energy (GeV);Fraction of events");
   hAll->SetLineColor(kBlue);
   hMany->SetLineColor(kRed);
   hAll->SetLineWidth(2);
   hMany->SetLineWidth(2);
   hAll->SetMaximum(1.2 * std::max(hAll->GetMaximum(), hMany->GetMaximum()));

   auto c = new TCanvas("c", "Compare", 800, 600);
   hAll->Draw("HIST");
   hMany->Draw("HIST SAME");
   auto leg = new TLegend(0.55, 0.76, 0.88, 0.88);
   leg->AddEntry(hAll, "All events", "l");
   leg->AddEntry(hMany, "nHits > 20", "l");
   leg->Draw();
   c->SaveAs("tree_compare.png");
}
