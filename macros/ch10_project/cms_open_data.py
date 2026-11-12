# cms_open_data.py -- BONUS: the same analysis on REAL data from the CMS experiment
# Run with:   python cms_open_data.py
#
# Needs an internet connection. The file (2.2 GB, 61.5 million events) is read
# over the network from CERN's open-data server; only the parts we use are
# downloaded. Reading the first 2 million events takes about a minute or two.
# Set N_EVENTS = 0 to use all 61.5 million events (much longer!).
import ROOT

N_EVENTS = 2_000_000
URL = ("root://eospublic.cern.ch//eos/opendata/cms/derived-data/"
       "AOD2NanoAODOutreachTool/Run2012BC_DoubleMuParked_Muons.root")

df = ROOT.RDataFrame("Events", URL)
if N_EVENTS > 0:
    df = df.Range(N_EVENTS)          # only the first N events
else:
    ROOT.EnableImplicitMT()          # (Range cannot be used with multi-threading)
    df = ROOT.RDataFrame("Events", URL)

# Exactly the same selection as for the toy data
df_mass = (df.Filter("nMuon == 2", "Exactly two muons")
             .Filter("Muon_charge[0] != Muon_charge[1]", "Opposite charge")
             .Define("Dimuon_mass",
                     "ROOT::VecOps::InvariantMass(Muon_pt, Muon_eta, Muon_phi, Muon_mass)"))

# Logarithmic bins, so that both low and high masses look good on a log axis
import numpy as np
edges = np.logspace(np.log10(0.25), np.log10(300), 1001)
h = df_mass.Histo1D(ROOT.RDF.TH1DModel("h", ";m_{#mu#mu} (GeV);Events / bin",
                                       len(edges) - 1, edges), "Dimuon_mass")
report = df_mass.Report()

ROOT.gStyle.SetOptStat(0)
c = ROOT.TCanvas("c", "CMS open data", 900, 650)
c.SetLogx()
c.SetLogy()
h.SetLineColor(ROOT.kBlue + 1)
h.SetMaximum(5 * h.GetMaximum())       # room for the labels
h.Draw("HIST")

# Name the peaks. Coordinates are (mass in GeV, height in events).
label = ROOT.TLatex()
label.SetTextFont(42)
label.SetTextSize(0.04)
label.SetTextAlign(21)                     # centred, above the point
peak_names = [(0.55, "#eta"), (0.78, "#rho,#omega"), (1.02, "#phi"), (3.10, "J/#psi"),
              (3.69, "#psi'"), (9.9, "#Upsilon"), (91.2, "Z")]
for mass, name in peak_names:
    # highest bin within +-5% of the mass
    height = max(h.GetBinContent(b) for b in range(h.FindBin(0.95 * mass), h.FindBin(1.05 * mass) + 1))
    label.DrawLatex(mass, 1.4 * height, name)

title = ROOT.TLatex()
title.SetNDC()
title.SetTextFont(42)
title.SetTextSize(0.035)
title.DrawLatex(0.12, 0.92, "#bf{CMS Open Data} 2012, #sqrt{s} = 8 TeV, "
                + (f"first {N_EVENTS/1e6:.0f} million events" if N_EVENTS else "all events"))
c.SaveAs("cms_dimuon_spectrum.png")

print("Muon pairs in the plot:", int(h.GetEntries()))
report.Print()
