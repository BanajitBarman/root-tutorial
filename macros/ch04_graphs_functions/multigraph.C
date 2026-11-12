// multigraph.C -- several graphs on one set of axes with TMultiGraph
// Run with:   root -l multigraph.C

void multigraph()
{
   // A cup of coffee cooling down, with and without a lid
   const int n = 13;
   double t[n]     = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60};
   double noLid[n] = {89.6, 77.6, 68.6, 59.7, 51.6, 47.0, 42.1,
                      38.9, 34.8, 33.4, 31.3, 30.5, 28.4};
   double lid[n]   = {90.3, 82.0, 77.8, 69.6, 66.3, 60.8, 56.4,
                      52.8, 49.6, 47.2, 44.3, 42.9, 38.8};

   auto gNoLid = new TGraph(n, t, noLid);
   gNoLid->SetMarkerStyle(20);
   gNoLid->SetMarkerColor(kRed);
   gNoLid->SetLineColor(kRed);

   auto gLid = new TGraph(n, t, lid);
   gLid->SetMarkerStyle(21);
   gLid->SetMarkerColor(kBlue);
   gLid->SetLineColor(kBlue);

   auto mg = new TMultiGraph();
   mg->SetTitle("Cooling coffee;Time (min);Temperature (#circC)");
   mg->Add(gNoLid, "PL");      // each graph can have its own draw option
   mg->Add(gLid,   "PL");

   auto c = new TCanvas("c", "MultiGraph", 800, 600);
   c->SetGrid();
   mg->Draw("A");              // the multigraph draws the axes for all of them

   auto leg = new TLegend(0.60, 0.76, 0.88, 0.88);
   leg->AddEntry(gNoLid, "Without lid", "lp");
   leg->AddEntry(gLid,   "With lid",    "lp");
   leg->Draw();

   c->SaveAs("multigraph.png");
}
