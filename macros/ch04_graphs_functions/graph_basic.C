// graph_basic.C -- a graph (x,y points) from two arrays
// Run with:   root -l graph_basic.C

void graph_basic()
{
   // Temperature measured every 2 hours during one day
   const int n = 13;
   double hour[n] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24};
   double temp[n] = {18.7, 16.4, 17.8, 19.0, 23.2, 26.1, 29.1,
                     30.5, 31.3, 28.7, 26.4, 22.0, 19.1};

   auto g = new TGraph(n, hour, temp);
   g->SetTitle("Temperature during one day;Time (hours);Temperature (#circC)");
   g->SetMarkerStyle(21);           // filled square
   g->SetMarkerSize(1.3);
   g->SetMarkerColor(kRed + 1);
   g->SetLineColor(kGray + 2);
   g->SetLineWidth(2);

   auto c = new TCanvas("c", "Graph", 800, 600);
   c->SetGrid();                    // grid lines help reading values
   g->Draw("APL");                  // A = draw axes, P = points, L = line

   c->SaveAs("graph_basic.png");

   std::cout << "Number of points: " << g->GetN() << std::endl;
   std::cout << "Point 6: hour = " << g->GetPointX(6)
             << ", temperature = " << g->GetPointY(6) << std::endl;
   std::cout << "Temperature at 13:00 (linear interpolation) = "
             << g->Eval(13.0) << std::endl;
}
