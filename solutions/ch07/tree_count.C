// Solution to Exercise 7.2 -- count events in a loop, then check with a one-liner
void tree_count()
{
   TFile *file = TFile::Open("../../macros/ch07_trees/toy_tree.root");
   auto tree = file->Get<TTree>("events");

   double energy, x, y;
   tree->SetBranchAddress("energy", &energy);
   tree->SetBranchAddress("x", &x);
   tree->SetBranchAddress("y", &y);

   int count = 0;
   for (Long64_t i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);
      if (energy > 50 && sqrt(x * x + y * y) < 10) count++;
   }
   std::cout << "Loop count     : " << count << std::endl;
   std::cout << "GetEntries(cut): "
             << tree->GetEntries("energy > 50 && sqrt(x*x + y*y) < 10") << std::endl;
}
