# rdf_vectors.py -- working with vector columns (one value per hit)
# Run with:   python rdf_vectors.py     (needs ../ch07_trees/toy_tree.root)
import ROOT

df = ROOT.RDataFrame("events", "../ch07_trees/toy_tree.root")

# hitEnergy is an RVec: an array with different length in each event.
# Operations work on all elements at once, no loop needed:
#   hitEnergy > 2           -> array of true/false
#   Sum(hitEnergy > 2)      -> how many hits above 2 GeV
#   hitEnergy[hitEnergy > 2] -> only the hits above 2 GeV
df2 = (df.Define("nBigHits", "Sum(hitEnergy > 2.0)")
         .Define("bigHits", "hitEnergy[hitEnergy > 2.0]")
         .Define("maxHit", "Max(hitEnergy)")
         .Define("fracBig", "Sum(bigHits) / energy"))

h_n = df2.Histo1D(("h_n", "Hits above 2 GeV;Number of hits;Events", 30, 0, 30), "nBigHits")
# Histogramming a vector column fills one entry per element (per hit)
h_hits = df2.Histo1D(("h_hits", "Energy of every hit;Hit energy (GeV);Hits", 60, 0, 30), "hitEnergy")

df2.Display(["nHits", "nBigHits", "maxHit", "fracBig"], 5).Print()

c = ROOT.TCanvas("c", "Vectors", 1200, 500)
c.Divide(2, 1)
c.cd(1)
h_n.Draw()
c.cd(2)
ROOT.gPad.SetLogy()
h_hits.Draw()
c.SaveAs("rdf_vectors.png")
