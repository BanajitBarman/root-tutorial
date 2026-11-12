// rdf_basic_cpp.C -- the same first RDataFrame analysis, in C++
// Run with:   root -l rdf_basic_cpp.C     (needs ../ch07_trees/toy_tree.root)

void rdf_basic_cpp()
{
   ROOT::RDataFrame df("events", "../ch07_trees/toy_tree.root");

   // A Define can be a string (as in Python) ...
   auto dfSel = df.Define("r", "sqrt(x*x + y*y)")
                  .Filter("energy > 30", "Energy above 30 GeV");

   // ... or a C++ function (here a "lambda") plus the list of input columns
   auto dfPhi = dfSel.Define("phi", [](double x, double y) { return atan2(y, x); },
                             {"x", "y"});

   auto nSel = dfPhi.Count();
   auto hPhi = dfPhi.Histo1D({"hPhi", "Energy > 30 GeV;#phi (rad);Events", 40, -4, 4}, "phi");

   std::cout << "Selected events: " << *nSel << std::endl;   // * gives the value

   auto c = new TCanvas("c", "RDataFrame in C++", 800, 600);
   hPhi->SetMinimum(0);
   // DrawClone: the histogram belongs to the dataframe and is deleted when
   // the macro ends; a clone stays on the canvas.
   hPhi->DrawClone();
   c->SaveAs("rdf_basic_cpp.png");
}
