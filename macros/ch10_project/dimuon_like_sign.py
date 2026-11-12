# dimuon_like_sign.py -- extra task: compare opposite-charge and same-charge pairs
# Run with:   python dimuon_like_sign.py     (run make_dimuon_toy.C first)
#
# A real particle decays to mu+ mu- (opposite charge). Random pairs of
# unrelated muons can have any charges. So same-charge pairs show what the
# random background looks like, and the difference shows the real pairs.
import ROOT

ROOT.gStyle.SetOptStat(0)

df = ROOT.RDataFrame("Events", "dimuon_toy.root")
df_2mu = (df.Filter("nMuon == 2")
            .Define("Dimuon_mass",
                    "ROOT::VecOps::InvariantMass(Muon_pt, Muon_eta, Muon_phi, Muon_mass)"))

bins = ("", ";m_{#mu#mu} (GeV);Pairs / 40 MeV", 150, 0.0, 6.0)
h_os = df_2mu.Filter("Muon_charge[0] != Muon_charge[1]").Histo1D(("h_os",) + bins[1:], "Dimuon_mass")
h_ss = df_2mu.Filter("Muon_charge[0] == Muon_charge[1]").Histo1D(("h_ss",) + bins[1:], "Dimuon_mass")

h_diff = h_os.Clone("h_diff")
h_diff.Add(h_ss.GetPtr(), -1)        # opposite-charge minus same-charge

c = ROOT.TCanvas("c", "Like-sign", 1200, 500)
c.Divide(2, 1)

c.cd(1)
h_os.SetLineColor(ROOT.kBlue + 1)
h_ss.SetLineColor(ROOT.kRed)
h_ss.SetFillColorAlpha(ROOT.kRed, 0.25)
h_os.SetTitle("All muon pairs")
h_os.Draw("HIST")
h_ss.Draw("HIST SAME")
leg = ROOT.TLegend(0.55, 0.72, 0.88, 0.86)
leg.SetBorderSize(0)
leg.AddEntry(h_os.GetPtr(), "Opposite charge (+ -)", "l")
leg.AddEntry(h_ss.GetPtr(), "Same charge (+ + or - -)", "f")
leg.Draw()

c.cd(2)
h_diff.SetTitle("Opposite minus same charge")
h_diff.SetLineColor(ROOT.kBlack)
h_diff.Draw("HIST")

c.SaveAs("dimuon_like_sign.png")
print("Opposite-charge pairs:", int(h_os.GetEntries()))
print("Same-charge pairs    :", int(h_ss.GetEntries()))
