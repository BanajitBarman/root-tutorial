// tree_inspect.C -- look inside a TTree: its branches and its first entries
// Run with:   root -l tree_inspect.C      (run tree_write.C first)

void tree_inspect()
{
   TFile *file = TFile::Open("toy_tree.root");
   auto tree = file->Get<TTree>("events");

   std::cout << "Number of events: " << tree->GetEntries() << std::endl;

   tree->Print();   // list of branches, their types and sizes

   // Print a table: which columns, which events (cut), options, how many rows
   tree->Scan("eventNumber:energy:nHits:x:y", "", "", 5);
   // With a cut: look at the first 10 events, show only those with nHits > 10
   tree->Scan("eventNumber:energy:nHits", "nHits > 10", "", 10);
}
