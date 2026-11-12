# rdf_cutflow.py -- several named cuts and a cut-flow report
# Run with:   python rdf_cutflow.py     (needs ../ch07_trees/toy_tree.root)
import ROOT

df = ROOT.RDataFrame("events", "../ch07_trees/toy_tree.root")

# Give every Filter a name: the report then tells you what each cut did
sel = (df.Filter("energy > 10", "Energy > 10 GeV")
         .Filter("nHits >= 5", "At least 5 hits")
         .Filter("sqrt(x*x + y*y) < 20", "Inside r < 20 cm")
         .Filter("Max(hitEnergy) < 0.5 * energy", "No dominant hit"))

report = sel.Report()
n_final = sel.Count()

print("Events after all cuts:", n_final.GetValue())
report.Print()
