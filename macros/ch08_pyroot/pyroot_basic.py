# pyroot_basic.py -- histogram, fit and plot, written in Python
# Run with:   python pyroot_basic.py
#   (or  python -i pyroot_basic.py  to keep the window open)
import ROOT

ROOT.gStyle.SetOptFit(1111)
ROOT.gStyle.SetStatW(0.25)
ROOT.gStyle.SetStatH(0.20)

# Exactly the same classes and methods as in C++
h = ROOT.TH1D("h", "Gaussian fit in Python;Mass (GeV);Events", 50, 0, 10)
rng = ROOT.TRandom3(5)
for i in range(1000):
    h.Fill(rng.Gaus(5.0, 1.5))
h.SetMaximum(1.6 * h.GetMaximum())

c = ROOT.TCanvas("c", "PyROOT", 800, 600)
h.Fit("gaus")
h.SetMarkerStyle(20)
h.Draw("E")
c.SaveAs("pyroot_basic.png")

fit = h.GetFunction("gaus")
print(f"Mean  = {fit.GetParameter(1):.3f} +- {fit.GetParError(1):.3f}")
print(f"Sigma = {fit.GetParameter(2):.3f} +- {fit.GetParError(2):.3f}")
print(f"chi2/ndf = {fit.GetChisquare():.1f} / {fit.GetNDF()}")
