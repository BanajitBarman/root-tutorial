# rdf_snapshot.py -- save a selection to a new file, use all CPU cores,
#                    and get columns as NumPy arrays
# Run with:   python rdf_snapshot.py     (needs ../ch07_trees/toy_tree.root)
import ROOT

ROOT.EnableImplicitMT()      # use all CPU cores (call it BEFORE making the dataframe)

df = ROOT.RDataFrame("events", "../ch07_trees/toy_tree.root")
skim = (df.Filter("energy > 50", "High energy")
          .Define("r", "sqrt(x*x + y*y)"))

# Write the selected events, with only some columns, to a new file
skim.Snapshot("events_skim", "skim.root", ["eventNumber", "energy", "nHits", "r"])
print("Wrote skim.root")

# Read it back and get NumPy arrays
df_skim = ROOT.RDataFrame("events_skim", "skim.root")
arrays = df_skim.AsNumpy(["energy", "r"])
energy = arrays["energy"]
print("Events in skim :", len(energy))
print(f"Mean energy    : {energy.mean():.2f} GeV")
print(f"Largest energy : {energy.max():.2f} GeV")
