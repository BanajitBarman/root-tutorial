# Extra task (chapter 10) -- fit BOTH peaks: two Gaussians + exponential background
import ROOT

ROOT.gStyle.SetOptStat(0)
df = ROOT.RDataFrame("Events", "../../macros/ch10_project/dimuon_toy.root")
df_mass = (df.Filter("nMuon == 2")
             .Filter("Muon_charge[0] != Muon_charge[1]")
             .Define("m", "ROOT::VecOps::InvariantMass(Muon_pt, Muon_eta, Muon_phi, Muon_mass)"))
h = df_mass.Histo1D(("h", ";m_{#mu#mu} (GeV);Events / 20 MeV", 75, 2.6, 4.1), "m").GetValue()

model = ROOT.TF1("model", "gaus(0) + gaus(3) + expo(6)", 2.6, 4.1)
model.SetParNames("H1", "M1", "S1", "H2", "M2", "S2", "B0", "B1")
model.SetParameters(1000, 3.10, 0.03, 50, 3.69, 0.04, 6.5, -0.3)
model.SetNpx(600)

c = ROOT.TCanvas("c", "Two peaks", 800, 600)
h.Fit(model, "R")

bw = h.GetBinWidth(1)
root2pi = (2 * ROOT.TMath.Pi()) ** 0.5
n1 = model.GetParameter(0) * abs(model.GetParameter(2)) * root2pi / bw
n2 = model.GetParameter(3) * abs(model.GetParameter(5)) * root2pi / bw
print(f"Peak 1: m = {1000*model.GetParameter(1):.1f} MeV, N = {n1:.0f}")
print(f"Peak 2: m = {1000*model.GetParameter(4):.1f} MeV, N = {n2:.0f}")
print(f"Ratio N2/N1 = {n2/n1:.3f}")

h.SetMarkerStyle(20)
h.SetMarkerSize(0.7)
h.Draw("E")
c.SaveAs("psi2s_fit.png")
