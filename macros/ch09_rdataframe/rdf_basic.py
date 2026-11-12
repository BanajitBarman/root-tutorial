# rdf_basic.py -- first steps with RDataFrame
# Run with:   python rdf_basic.py     (needs ../ch07_trees/toy_tree.root)
import ROOT

# A dataframe on top of a tree: RDataFrame("tree name", "file name")
df = ROOT.RDataFrame("events", "../ch07_trees/toy_tree.root")

# Define = make a new column.  Filter = keep only events that pass a cut.
df_sel = (df.Define("r", "sqrt(x*x + y*y)")
            .Filter("energy > 30", "Energy above 30 GeV"))

# Results are LAZY: these lines only "book" the work, nothing runs yet
n_all  = df.Count()
n_sel  = df_sel.Count()
mean_r = df_sel.Mean("r")
h_e = df.Histo1D(("h_e", "All events;Energy (GeV);Events", 50, 0, 100), "energy")
h_r = df_sel.Histo1D(("h_r", "Energy > 30 GeV;r (cm);Events", 40, 0, 40), "r")

# The first time we ask for a value, ONE loop over the data fills everything
print("All events     :", n_all.GetValue())
print("Selected events:", n_sel.GetValue())
print(f"Mean r         : {mean_r.GetValue():.2f} cm")
print("The event loop ran", df.GetNRuns(), "time(s)")

c = ROOT.TCanvas("c", "RDataFrame", 1200, 500)
c.Divide(2, 1)
c.cd(1)
ROOT.gPad.SetLogy()
h_e.Draw()
c.cd(2)
h_r.Draw()
c.SaveAs("rdf_basic.png")

# Print the first rows of some columns
df_sel.Display(["eventNumber", "energy", "nHits", "r"], 5).Print()
