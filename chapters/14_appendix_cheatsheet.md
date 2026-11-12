# Appendix B: Quick reference

A summary of the commands used in this manual. C++ is shown; for Python, see the
translation table at the end.

## Starting ROOT

| Command | Meaning |
|---------|---------|
| `root -l` | start ROOT without the banner |
| `root -l macro.C` | start ROOT and run `macro.C` |
| `root -l -b -q 'macro.C(5)'` | run with an argument, no graphics, then quit |
| `root -l file.root` | start ROOT and open a file (as `_file0`) |
| `.x macro.C` / `.x macro.C+` | run a macro / compile and run it |
| `.L macro.C` | load a macro, then call its functions |
| `.ls` / `.q` | list objects / quit |
| `rootls -l file.root`, `rootbrowse file.root` | look inside a file |
| `hadd out.root in1.root in2.root` | merge files |

## Histograms

```cpp
auto h = new TH1D("h", "title;x title;y title", 100, 0., 10.);
h->Fill(x);                      h->Fill(x, weight);
h->Draw();     h->Draw("E");     h->Draw("HIST SAME");
h->GetEntries();  h->GetMean();  h->GetStdDev();  h->Integral();
h->GetBinContent(i);  h->FindBin(x);  h->GetBinCenter(i);
h->Scale(1. / h->Integral());    h->Sumw2();   h->Rebin(2);
h->Add(h2);   h->Divide(h1, h2, 1, 1, "B");    auto h3 = (TH1D*)h->Clone("h3");
h->SetLineColor(kRed);  h->SetFillColor(kAzure-9);  h->SetMarkerStyle(20);
h->SetMinimum(0);  h->SetMaximum(100);  h->GetXaxis()->SetRangeUser(2, 5);
auto h2 = new TH2D("h2", "t;x;y", 50, 0, 10, 50, 0, 10);  h2->Fill(x, y);  h2->Draw("COLZ");
```

## Functions and graphs

```cpp
auto f = new TF1("f", "[0]*exp(-x/[1])", 0, 10);   // also "gaus", "expo", "pol2"
f->SetParameters(1, 3);  f->Eval(2.);  f->Integral(0, 5);  f->Draw();
auto g = new TGraph(n, xArray, yArray);       g->Draw("APL");
auto ge = new TGraphErrors("data.txt");       ge->Draw("AP");   // x y ex ey
auto mg = new TMultiGraph();  mg->Add(g, "PL");  mg->Draw("A");
```

## Fitting

```cpp
h->Fit("gaus");                                  // built-in function
h->Fit("gaus", "", "", 3., 7.);                  // only between 3 and 7
auto f = new TF1("f", "gaus(0) + expo(3)", 0, 10);
f->SetParameters(100, 4, 0.5, 5, -0.3);          // starting values!
TFitResultPtr r = h->Fit(f, "S R");              // S: result, R: function range
f->GetParameter(1);  f->GetParError(1);  f->GetChisquare();  f->GetNDF();
gStyle->SetOptFit(1111);                         // fit results in the stats box
```

Options: `Q` quiet, `L` likelihood, `R` range, `S` result, `0` no drawing, `+` keep old fits.

## Canvas, style and text

```cpp
auto c = new TCanvas("c", "title", 800, 600);
c->Divide(2, 2);  c->cd(1);  gPad->SetLogy();  gPad->SetGrid();
c->SaveAs("plot.pdf");  c->SaveAs("plot.png");
gStyle->SetOptStat(0);                           // no statistics boxes
auto leg = new TLegend(0.6, 0.7, 0.88, 0.88);
leg->AddEntry(h, "Data", "pe");  leg->AddEntry(f, "Fit", "l");  leg->Draw();
auto t = new TLatex();  t->SetNDC();  t->SetTextFont(42);  t->DrawLatex(0.2, 0.8, "#sigma = 1");
```

LaTeX in ROOT text: `#alpha #beta #mu #sigma #pi #Upsilon`, `x^{2}`, `p_{T}`,
`#sqrt{s}`, `#pm`, `#rightarrow`, `#bf{bold}`, `#it{italic}`.

## Files

```cpp
TFile *f = TFile::Open("out.root", "RECREATE");   // or "READ", "UPDATE"
h->Write();   g->Write("gName");   f->Write();   f->Close();
TFile *in = TFile::Open("out.root");
auto h = in->Get<TH1D>("h");   if (!h) { /* not found! */ }
auto h2 = in->Get<TH1D>("folder/h2");
h->SetDirectory(nullptr);                          // keep h after in->Close()
```

## Trees

```cpp
// Write
auto t = new TTree("events", "title");
double e;  std::vector<double> v;
t->Branch("e", &e);  t->Branch("v", &v);
/* in the loop: */  e = ...;  v.clear(); v.push_back(...);  t->Fill();
t->Write();

// Look
t->Print();  t->Scan("e:nHits", "e > 10", "", 20);  t->GetEntries("e > 10");
t->Draw("e>>h(100,0,100)", "nHits > 5");  t->Draw("y:x", "", "COLZ");

// Loop
TTreeReader reader("events", file);
TTreeReaderValue<double> e(reader, "e");  TTreeReaderArray<double> v(reader, "v");
while (reader.Next()) { double val = *e;  for (double x : v) { /* ... */ } }

TChain chain("events");  chain.Add("run*.root");  // many files as one tree
```

## RDataFrame (Python)

```python
ROOT.EnableImplicitMT()                              # optional: all cores
df = ROOT.RDataFrame("events", "file.root")
sel = (df.Filter("energy > 30", "High energy")
         .Define("r", "sqrt(x*x + y*y)"))
n = sel.Count()                                      # lazy: booked, not run yet
h = sel.Histo1D(("h", "title;r;Events", 40, 0, 40), "r")
m = sel.Mean("r")
print(n.GetValue(), m.GetValue())                    # one loop fills everything
h.Draw()
df.Report().Print()                                  # cut flow
sel.Display(["energy", "r"], 5).Print()
sel.Snapshot("tree", "skim.root", ["energy", "r"])
arrays = sel.AsNumpy(["energy", "r"])
```

RVec in strings: `Sum(v)`, `Max(v)`, `v[v > 2]`, `Sum(v > 2)`, `v.size()`,
`ROOT::VecOps::InvariantMass(pt, eta, phi, m)`.

## C++ → Python

| C++ | Python |
|-----|--------|
| `auto h = new TH1D(...);` | `h = ROOT.TH1D(...)` |
| `h->Draw();` | `h.Draw()` |
| `gStyle->SetOptStat(0);` | `ROOT.gStyle.SetOptStat(0)` |
| `TMath::Pi()`, `kRed` | `ROOT.TMath.Pi()`, `ROOT.kRed` |
| `nullptr`, `true` | `ROOT.nullptr`, `True` |
| `*result` (RDataFrame) | `result.GetValue()` |
| `{"h", "t", 10, 0, 1}` (histogram model) | `("h", "t", 10, 0, 1)` |
