// make_dimuon_toy.C -- make the toy dataset for the mini-project
// Run with:   root -l make_dimuon_toy.C        (takes a few seconds)
//
// Writes dimuon_toy.root with a tree "Events". Each event has a few muons.
// The branch names are the same as in the CMS Open Data dimuon sample, so
// the same analysis code works on real data (see the bonus section).
//
// Hidden inside: some events contain a particle that decays to mu+ mu-.
// Your job in the mini-project is to find it and measure its mass.
// (Organisers: please don't tell the participants what is inside!)

void make_dimuon_toy(int nEvents = 300000)
{
   const double muMass = 0.105658;           // GeV
   TRandom3 rng(2026);

   TFile *file = TFile::Open("dimuon_toy.root", "RECREATE");
   auto tree = new TTree("Events", "Toy dimuon events");

   UInt_t nMuon;
   std::vector<float> Muon_pt, Muon_eta, Muon_phi, Muon_mass;
   std::vector<int>   Muon_charge;
   tree->Branch("nMuon", &nMuon);
   tree->Branch("Muon_pt", &Muon_pt);
   tree->Branch("Muon_eta", &Muon_eta);
   tree->Branch("Muon_phi", &Muon_phi);
   tree->Branch("Muon_mass", &Muon_mass);
   tree->Branch("Muon_charge", &Muon_charge);

   // Detector: muons are seen if pT > 1 GeV and |eta| < 2.4;
   // the pT is measured with 1.5% resolution.
   auto addMuon = [&](const TLorentzVector &p, int charge) {
      if (p.Pt() < 1.0 || std::abs(p.Eta()) > 2.4) return;
      Muon_pt.push_back(p.Pt() * (1.0 + rng.Gaus(0, 0.015)));
      Muon_eta.push_back(p.Eta());
      Muon_phi.push_back(p.Phi());
      Muon_mass.push_back(muMass);
      Muon_charge.push_back(charge);
   };

   // A particle of mass m with random pT and rapidity, decaying to mu+ mu-
   TGenPhaseSpace decay;
   double daughterMasses[2] = {muMass, muMass};
   auto makePair = [&](double m) {
      double pt  = rng.Exp(2.0) + rng.Exp(2.0);
      double y   = rng.Uniform(-2.5, 2.5);
      double phi = rng.Uniform(-TMath::Pi(), TMath::Pi());
      double mT  = sqrt(m * m + pt * pt);
      TLorentzVector parent(pt * cos(phi), pt * sin(phi), mT * sinh(y), mT * cosh(y));
      decay.SetDecay(parent, 2, daughterMasses);
      decay.Generate();
      addMuon(*decay.GetDecay(0), +1);
      addMuon(*decay.GetDecay(1), -1);
   };

   // A single unrelated muon
   auto randomMuon = [&]() {
      TLorentzVector p;
      p.SetPtEtaPhiM(0.8 + rng.Exp(1.2), rng.Uniform(-2.6, 2.6),
                     rng.Uniform(-TMath::Pi(), TMath::Pi()), muMass);
      addMuon(p, rng.Uniform() < 0.5 ? +1 : -1);
   };

   for (int i = 0; i < nEvents; i++) {
      Muon_pt.clear(); Muon_eta.clear(); Muon_phi.clear();
      Muon_mass.clear(); Muon_charge.clear();

      double u = rng.Uniform();
      if (u < 0.030)       makePair(3.0969);             // the hidden particle
      else if (u < 0.0315) makePair(3.6861);             // a smaller one
      else if (u < 0.55)   makePair(0.25 + rng.Exp(0.9) + rng.Exp(0.9)); // smooth continuum
      else { randomMuon(); randomMuon(); }               // unrelated pair

      int nExtra = rng.Poisson(0.15);                    // extra random muons
      for (int k = 0; k < nExtra; k++) randomMuon();

      nMuon = Muon_pt.size();
      if (nMuon > 0) tree->Fill();
   }

   tree->Write();
   std::cout << "Wrote " << tree->GetEntries() << " events to dimuon_toy.root" << std::endl;
   file->Close();
}
