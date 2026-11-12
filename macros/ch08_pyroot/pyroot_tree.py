# pyroot_tree.py -- read a TTree in Python
# Run with:   python pyroot_tree.py
#   (needs ../ch07_trees/toy_tree.root: run tree_write.C first)
import ROOT

f = ROOT.TFile.Open("../ch07_trees/toy_tree.root")
tree = f.Get("events")
print("Events in tree:", tree.GetEntries())

# Branches are attributes of the event: event.energy, event.hitEnergy ...
n_high = 0
total_hits = 0
for event in tree:
    total_hits += event.nHits
    if event.energy > 50:
        n_high += 1

print("Events with energy > 50 GeV:", n_high)
print(f"Average number of hits: {total_hits / tree.GetEntries():.2f}")

# Look at the first event in detail
tree.GetEntry(0)
print("Event 0: energy =", round(tree.energy, 2), "GeV, hits:",
      [round(e, 2) for e in tree.hitEnergy])
