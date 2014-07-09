#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TClonesArray.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TPaveStats.h"
#include "TAxis.h"
 #include "TStyle.h"

#include <iostream>

using namespace std;

static bool tight = false;
double eta = 1.4;

void plot(TH1D * h, string label = "") {

	TCanvas * c = new TCanvas("c", "c", 600, 600);
	h->GetYaxis()->SetTitleOffset(1.4);
	h->SetLineColor(kBlue);
	h->SetFillColor(kBlue);
	h->SetFillStyle(3004);
	h->Draw();
	c->Update();
	TPaveStats *st = (TPaveStats*)h->FindObject("stats");
	st->SetOptStat(111111);
	st->SetLineColor(kBlue);
	string name(h->GetName());
	c->Print(Form("%s%s.pdf",  name.c_str(), label.c_str()));
	delete c;
}

void plot_eff(TH1D * h, string label = "") {

	TCanvas * c = new TCanvas("c", "c", 600, 600);
	h->GetYaxis()->SetTitleOffset(1.4);
	h->Draw("e");
	h->SetStats(false);
	c->SetGrid();
	string name(h->GetName());
	c->Print(Form("%s%s.pdf",  name.c_str(), label.c_str()));
	delete c;
}

void plot_2D(TH2D * h, string label = "") {

	TCanvas * c = new TCanvas("c", "c", 600, 600);
//	gStyle->SetPalette(53);
	h->Draw("COLZ");
	h->SetStats(false);
	string name(h->GetName());
	c->Print(Form("%s%s.pdf",  name.c_str(), label.c_str()));
	delete c;
}

int main(int argc, char* argv[]) {

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i],"-t")) {
			tight = true;
			cout << "tight muons" << endl;
		}
	}

    TFile* input = new TFile("muontree_5314.root");
    TTree* tree = (TTree*)input->Get("tree");

    int mom_pdgId = 531;

    TH1D h_deltaeta("h_deltaeta", "deltaeta;#Delta#eta;entries", 50, 0., 2.0);
    TH1D h_deltaeta_cumu("h_deltaeta_cumu", "h_deltaeta_cumu;#Delta#eta;entries", 50, 0., 2.0);
    TH1D h_deltaeta_b("h_deltaeta_b", "h_deltaeta_b;#Delta#eta;entries", 50, 0., 2.0);
    TH1D h_deltaeta_e("h_deltaeta_e", "h_deltaeta_e;#Delta#eta;entries", 50, 0., 2.0);
    TH1D h_deltaphi("h_deltaphi", "deltaphi;#Delta#phi;entries", 50, 0., 3.5);
    TH1D h_deltaphi_b("h_deltaphi_b", "h_deltaphi_b;#Delta#phi;entries", 50, 0., 3.5);
    TH1D h_deltaphi_e("h_deltaphi_e", "h_deltaphi_e;#Delta#phi;entries", 50, 0., 3.5);
    TH1D h_deltar("h_deltar", "deltar;#Delta R;entries", 50, 0., 7.);

    TH2D h_deltaeta_dipt("h_deltaeta_dipt", "h_deltaeta_dipt;#Delta#eta;p_{T}^{B_{s}}", 50, 0., 2.0, 50, 0., 30.);

    TH1D h_deltaphi_cumu("h_deltaphi_cumu", "h_deltaphi_cumu;#Delta#phi;entries", 50, 0., 3.5);
    TH2D h_deltaphi_dipt("h_deltaphi_dipt", "h_deltaphi_dipt;#Delta#phi;p_{T}^{B_{s}}", 50, 0., 3.5, 50, 0., 30.);

    Int_t           nmuons;
    Int_t           reco_muon_charge[100];   //[nmuons]
    Int_t           ndimuons;
    Int_t           reco_dimuon_charge[100];   //[ndimuons]
    Int_t           ngen;
    TClonesArray    *gen_particle_4mom;
    Int_t           gen_particle_pdgId[100];   //[ngen]
    Int_t           gen_particle_status[100];   //[ngen]
    Int_t           reco_muon_pdgId[100];   //[nmuons]
    Int_t           reco_muon_pdgId_mother[100];   //[nmuons]
    Int_t           reco_muon_pdgId_grandma[100];   //[nmuons]
    TClonesArray    *reco_muon_4mom;
    Bool_t          reco_muon_isTightMuon[100];   //[nmuons]
    Bool_t          reco_muon_isTriggerMatched[100];   //[nmuons]
    TClonesArray    *reco_dimuon_4mom;

    TBranch        *b_nmuons;   //!
    TBranch        *b_reco_muon_charge;   //!
    TBranch        *b_ndimuons;   //!
    TBranch        *b_reco_dimuon_charge;   //!
    TBranch        *b_ngen;   //!
    TBranch        *b_gen_particle_4mom;   //!
    TBranch        *b_gen_particle_pdgId;   //!
    TBranch        *b_gen_particle_status;   //!
    TBranch        *b_reco_muon_pdgId;   //!
    TBranch        *b_reco_muon_pdgId_mother;   //!
    TBranch        *b_reco_muon_pdgId_grandma;   //!
    TBranch        *b_reco_muon_4mom;   //!
    TBranch        *b_reco_muon_isTightMuon;   //!
    TBranch        *b_reco_muon_isTriggerMatched;   //!
    TBranch        *b_reco_dimuon_4mom;   //!

    gen_particle_4mom = 0;
    reco_muon_4mom = 0;
    reco_dimuon_4mom = 0;
    // Set branch addresses and branch pointers

    tree->SetBranchAddress("nmuons", &nmuons, &b_nmuons);
    tree->SetBranchAddress("reco_muon_charge", reco_muon_charge, &b_reco_muon_charge);
    tree->SetBranchAddress("ndimuons", &ndimuons, &b_ndimuons);
    tree->SetBranchAddress("reco_dimuon_charge", reco_dimuon_charge, &b_reco_dimuon_charge);
    tree->SetBranchAddress("ngen", &ngen, &b_ngen);
    tree->SetBranchAddress("gen_particle_4mom", &gen_particle_4mom, &b_gen_particle_4mom);
    tree->SetBranchAddress("gen_particle_pdgId", gen_particle_pdgId, &b_gen_particle_pdgId);
    tree->SetBranchAddress("gen_particle_status", gen_particle_status, &b_gen_particle_status);
    tree->SetBranchAddress("reco_muon_pdgId", reco_muon_pdgId, &b_reco_muon_pdgId);
    tree->SetBranchAddress("reco_muon_pdgId_mother", reco_muon_pdgId_mother, &b_reco_muon_pdgId_mother);
    tree->SetBranchAddress("reco_muon_pdgId_grandma", reco_muon_pdgId_grandma, &b_reco_muon_pdgId_grandma);
    tree->SetBranchAddress("reco_muon_4mom", &reco_muon_4mom, &b_reco_muon_4mom);
    tree->SetBranchAddress("reco_muon_isTightMuon", reco_muon_isTightMuon, &b_reco_muon_isTightMuon);
    tree->SetBranchAddress("reco_muon_isTriggerMatched", reco_muon_isTriggerMatched, &b_reco_muon_isTriggerMatched);
    tree->SetBranchAddress("reco_dimuon_4mom", &reco_dimuon_4mom, &b_reco_dimuon_4mom);

    Long64_t nentries = tree->GetEntries();

    double all[3] = { 0, 0, 0};
    double pass[3] = { 0, 0, 0};

    for (Long64_t i = 0; i < nentries; i++) {
      tree->GetEntry(i);
      for (int j = 0; j < ngen; j++) {
        for (int k = j+1; k < ngen; k++) {
          if ( ((gen_particle_pdgId[j] == 13 && gen_particle_pdgId[k] == -13) || (gen_particle_pdgId[j] == -13 && gen_particle_pdgId[k] == 13)) && abs(reco_muon_pdgId_mother[j]) == mom_pdgId && abs(reco_muon_pdgId_mother[k]) == mom_pdgId && gen_particle_status[j] == 1 && gen_particle_status[k] == 1) {
            TLorentzVector * mu_j = (TLorentzVector*)gen_particle_4mom->At(j);
            TLorentzVector * mu_k = (TLorentzVector*)gen_particle_4mom->At(k);
            TLorentzVector dimuon(0.,0.,0.,0.);
            dimuon = *mu_j + *mu_k;
            double deltaeta = abs(mu_j->Eta() - mu_k->Eta());
            if (mu_j->Pt()>3. && mu_k->Pt()>3. && abs(mu_j->Eta())<2.4 && abs(mu_k->Eta())<2.4) {

                  }
          }
        }

      }


      for (int j = 0; j < nmuons; j++) {
        TLorentzVector * mu_j = (TLorentzVector*)reco_muon_4mom->At(j);
        if (abs(reco_muon_pdgId[j]) == 13 && abs(reco_muon_pdgId_mother[j]) == mom_pdgId && (reco_muon_isTightMuon[j] == true || !tight) /*&& mu_j->Pt() > 4.*/) {
          double deltar_min = 100.;
          double pt_gen = -1000;
          double pt_reco = mu_j->Pt();
          double eta_reco = mu_j->Eta();
          for (int jj = 0; jj < ngen; jj++) {
            if (abs(gen_particle_pdgId[jj]) == 13 && gen_particle_status[jj] == 1) {
              TLorentzVector * mu_jj = (TLorentzVector*)gen_particle_4mom->At(jj);
              Double_t deltar = mu_j->DeltaR(*mu_jj);
              if (deltar < deltar_min && gen_particle_pdgId[jj]*reco_muon_charge[j] < 0) {
                deltar_min = deltar;
                pt_gen = mu_jj->Pt();
              }
            }
          }
          for (int k = j+1; k < nmuons; k++) {
            TLorentzVector * mu_k = (TLorentzVector*)reco_muon_4mom->At(k);
            if (abs(reco_muon_pdgId[k]) == 13 && abs(reco_muon_pdgId_mother[k]) == mom_pdgId  && (reco_muon_isTightMuon[k] == true || !tight) /*&& mu_k->Pt() > 4.*/) {
              if (reco_muon_charge[j] + reco_muon_charge[k] == 0){
                TLorentzVector dimuon(0.,0.,0.,0.);
                dimuon = *mu_j + *mu_k;
                double deltar = mu_j->DeltaR(*mu_k);
                double deltaphi = abs(mu_j->DeltaPhi(*mu_k));
                double deltaeta = abs(mu_j->Eta() - mu_k->Eta());

                h_deltar.Fill(deltar);
                h_deltaphi.Fill(deltaphi);
                h_deltaeta.Fill(deltaeta);
                all[0]++;
                if (deltaeta < 1.7) {
                  pass[0]++;
                }
                if (abs(mu_j->Eta()) < 1.4 && abs(mu_k->Eta()) < 1.4) {
                  all[1]++;
                  h_deltaeta_b.Fill(deltaeta);
                  h_deltaphi_b.Fill(deltaphi);
                  if (deltaeta < 1.7) {
                    pass[1]++;
                  }
                }
                else {
                  all[2]++;
                  h_deltaeta_e.Fill(deltaeta);
                  h_deltaphi_e.Fill(deltaphi);
                  if (deltaeta < 1.7) {
                    pass[2]++;
                  }
                }

                h_deltaeta_dipt.Fill(deltaeta, dimuon.Pt());
                h_deltaphi_dipt.Fill(deltaphi, dimuon.Pt());

              }
            }
          }
        }

      }
    }

    h_deltaeta.ComputeIntegral();
    Double_t *eta_integral = h_deltaeta.GetIntegral();
    h_deltaeta_cumu.SetContent(eta_integral);
    for (int i = 1; i <= h_deltaeta_cumu.GetNbinsX(); i++) {
      double eff = h_deltaeta_cumu.GetBinContent(i);
      double eff_err = sqrt(eff*(1.-eff)/h_deltaeta.GetEntries());
      h_deltaeta_cumu.SetBinError(i, eff_err);
    }

    h_deltaphi.ComputeIntegral();
    Double_t *phi_integral = h_deltaphi.GetIntegral();
    h_deltaphi_cumu.SetContent(phi_integral);
    for (int i = 1; i <= h_deltaphi_cumu.GetNbinsX(); i++) {
      double eff = h_deltaphi_cumu.GetBinContent(i);
      double eff_err = sqrt(eff*(1.-eff)/h_deltaphi.GetEntries());
      h_deltaphi_cumu.SetBinError(i, eff_err);
    }

    plot(&h_deltar);
    plot(&h_deltaphi);
    plot(&h_deltaeta);
    plot(&h_deltaeta_b);
    plot(&h_deltaeta_e);
    plot(&h_deltaphi_b);
    plot(&h_deltaphi_e);
    plot_eff(&h_deltaeta_cumu);
    plot_2D(&h_deltaeta_dipt);
    plot_eff(&h_deltaphi_cumu);
    plot_2D(&h_deltaphi_dipt);

    for (int i = 0; i < 3; i++) {
      double eff = pass[i]/all[i];
      double eff_err = sqrt(eff*(1.-eff)/all[i]);
      cout << "delta eta < 1.7 eff :  " << eff <<  " +- " << eff_err << endl;
    }



    return EXIT_SUCCESS;
}

