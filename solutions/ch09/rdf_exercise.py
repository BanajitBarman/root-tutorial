# Solution to Exercise 9.1 -- a small RDataFrame analysis
import ROOT

df = ROOT.RDataFrame("events", "../../macros/ch07_trees/toy_tree.root")
sel = (df.Define("r", "sqrt(x*x + y*y)")
         .Filter("energy > 30", "Energy > 30 GeV")
         .Filter("nHits >= 10", "At least 10 hits"))

h = sel.Histo1D(("h_r", "Selected events;r (cm);Events", 40, 0, 40), "r")
mean_r = sel.Mean("r")
report = df.Report()

c = ROOT.TCanvas("c", "RDF exercise", 800, 600)
h.Draw()
c.SaveAs("rdf_exercise.png")

print(f"Mean r of selected events: {mean_r.GetValue():.2f} cm")
report.Print()

sel.Snapshot("selected", "selected.root", ["energy", "r"])
print("Wrote selected.root")
