#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TClonesArray.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "THStack.h"
#include "TLatex.h"
#include "TF1.h"
#include "TFitResult.h"

#include <iostream> 
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

static bool tight = false;
double eta = 1.4;

enum particle { muon, Bs, Bd };

vector <double> plot(TH1D * h_5314, TH1D * h_620SLHC7, string label, bool fit = false, bool stack = false, particle p = muon) {

  vector <double> sigmas;
  double from = 0., to = 0.;
  if (p == muon) {
    from = -0.2;
    to = 0.2;
  }
  if (p == Bs) {
    from = 5.3;
    to = 5.5;
  }
  if (p == Bd) {
    from = 5.2;
    to = 5.4;
  }

	TCanvas * c = new TCanvas("c", "c", 600, 600);
	h_5314->SetStats(0);
	h_5314->SetMaximum(1.1*max(h_5314->GetMaximum(), h_620SLHC7->GetMaximum()));
	h_5314->GetYaxis()->SetTitleOffset(1.4);
	h_5314->SetLineColor(kBlue);
	h_5314->SetFillColor(kBlue);
	h_5314->SetFillStyle(3004);
	TFitResultPtr r_5314;
	TF1 *fitfunc_5314;
	if (fit) {
	  fitfunc_5314 = new TF1 ("gaus_5314", "gaus", from, to);
	  fitfunc_5314->SetLineColor(kBlue);
	  r_5314 = h_5314->Fit("gaus_5314", "0SR");
	  if (p == muon || 1) h_5314->GetFunction("gaus_5314")->ResetBit(TF1::kNotDraw);
	}
	h_5314->Draw();

	h_620SLHC7->SetStats(0);
	h_620SLHC7->SetLineColor(kRed);
	h_620SLHC7->SetFillColor(kRed);
	h_620SLHC7->SetFillStyle(3005);
	TFitResultPtr r_620SLHC7;
	TF1 *fitfunc_620SLHC7;
	if (fit) {
	  fitfunc_620SLHC7 = new TF1 ("gaus_620SLHC7", "gaus", from, to);
	  r_620SLHC7 = h_620SLHC7->Fit("gaus_620SLHC7", "0SR");
	  if (p == muon || 1) h_620SLHC7->GetFunction("gaus_620SLHC7")->ResetBit(TF1::kNotDraw);
	}
	h_620SLHC7->Draw("same");

	TLatex * st_5314 = new TLatex();
	st_5314->SetTextSize(0.023);
	st_5314->SetTextColor(kBlue);
	st_5314->DrawLatexNDC(0.6, 0.8, h_5314->GetName());
	st_5314->DrawLatexNDC(0.6, 0.76, Form("mean = %f", h_5314->GetMean()));
	st_5314->DrawLatexNDC(0.6, 0.72, Form("RMS = %f #pm %f", h_5314->GetRMS(), h_5314->GetRMSError()));
	if (fit) {
	  if (p == muon || 1) {
	   st_5314->DrawLatexNDC(0.6, 0.68, Form("#mu = %f #pm %f", r_5314->Parameter(1), r_5314->ParError(1)));
	   st_5314->DrawLatexNDC(0.6, 0.64, Form("#sigma = %f #pm %f", r_5314->Parameter(2), r_5314->ParError(2)));
	  }
	   sigmas.push_back(r_5314->Parameter(2));
	}
	TLatex * st_620SLHC7 = new TLatex();
	st_620SLHC7->SetTextSize(0.023);
	st_620SLHC7->SetTextColor(kRed);
	st_620SLHC7->DrawLatexNDC(0.6, 0.6, h_620SLHC7->GetName());
	st_620SLHC7->DrawLatexNDC(0.6, 0.56, Form("mean = %f", h_620SLHC7->GetMean()));
	st_620SLHC7->DrawLatexNDC(0.6, 0.52, Form("RMS = %f #pm %f", h_620SLHC7->GetRMS(), h_620SLHC7->GetRMSError()));
	if (fit) {
	  if (p == muon || 1) {
	   st_620SLHC7->DrawLatexNDC(0.6, 0.48, Form("#mu = %f #pm %f", r_620SLHC7->Parameter(1), r_620SLHC7->ParError(1)));
	   st_620SLHC7->DrawLatexNDC(0.6, 0.44, Form("#sigma = %f #pm %f", r_620SLHC7->Parameter(2), r_620SLHC7->ParError(2)));
	  }
	   sigmas.push_back(r_620SLHC7->Parameter(2));
	}

	string name(h_5314->GetName());
	std::size_t found = name.find("_h_");
	name.erase(found+2, std::string::npos);
	c->Print(Form("%s%s_%s.pdf", tight? "tight/" : "./", name.c_str(), label.c_str()));

	delete c;

	if (stack) {
	  THStack * hs = new THStack("hs", Form("%s_%s", name.c_str(), label.c_str()));
	  hs->Add(h_5314);
	  hs->Add(h_620SLHC7);
	  TCanvas *cst = new TCanvas("cst", "stacked hists", 600, 600);
	  hs->Draw();
	  cst->Print(Form("%s%s_%s_stacked.pdf", tight? "tight/" : "./", name.c_str(), label.c_str()));
	  delete cst;
	}
	return sigmas;
}

void fill(string tail, int mom_pdgId) {
  TFile* input = new TFile(Form("muontree_%s.root", tail.c_str()));
  TTree* tree = (TTree*)input->Get("tree");

  TH1D * mass_barrel_h = new TH1D(Form("mass_barrel_h_%s", tail.c_str()), Form("barrel mass (|#eta_{#mu^{1}}| < %.1f AND |#eta_{#mu^{2}}| < %.1f);mass (GeV); Normalized", eta, eta), 100, 4.9, 5.9);
  TH1D * mass_endcap_h = new TH1D(Form("mass_endcap_h_%s", tail.c_str()), Form("endcap mass (|#eta_{#mu^{1}}| > %.1f OR |#eta_{#mu^{2}}| > %.1f);mass (GeV); Normalized", eta, eta), 100, 4.9, 5.9);
  TH1D * pt_res_barrel_h = new TH1D(Form("pt_res_barrel_h_%s", tail.c_str()), "barrel p_{T} resolution (|#eta_{#mu}| < 1.2);p_{T}^{gen}-p_{T}^{reco} (GeV); Normalized", 100, -1., 1.);
  TH1D * pt_res_intermediate_h = new TH1D(Form("pt_res_intermediate_h_%s", tail.c_str()), "intermediate p_{T} resolution (1.2 < |#eta_{#mu}| < 1.6);p_{T}^{gen}-p_{T}^{reco} (GeV); Normalized", 100, -1., 1.);
  TH1D * pt_res_endcap_h = new TH1D(Form("pt_res_endcap_h_%s", tail.c_str()), "endcap p_{T} resolution (1.6 < |#eta_{#mu}| < 2.4);p_{T}^{gen}-p_{T}^{reco} (GeV); Normalized", 100, -1., 1.);

  // Declaration of leaf types
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

  for (Long64_t i = 0; i < nentries; i++) {
      tree->GetEntry(i);
      for (int j = 0; j < nmuons; j++) {
          TLorentzVector * mu_j = (TLorentzVector*)reco_muon_4mom->At(j);
          if (abs(reco_muon_pdgId[j]) == 13 && abs(reco_muon_pdgId_mother[j]) == mom_pdgId && (reco_muon_isTightMuon[j] == true || !tight) && mu_j->Pt() > 4.) {
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
              if (deltar_min < 0.1 || true) {
                  double delta_pt = pt_gen - pt_reco;
                  if (fabs(eta_reco) < 1.2) pt_res_barrel_h->Fill(delta_pt);
                  else if (fabs(eta_reco) < 1.6) pt_res_intermediate_h->Fill(delta_pt);
                  else if (fabs(eta_reco) < 2.4) pt_res_endcap_h->Fill(delta_pt);
              }
              for (int k = j+1; k < nmuons; k++) {
                  TLorentzVector * mu_k = (TLorentzVector*)reco_muon_4mom->At(k);
                  if (abs(reco_muon_pdgId[k]) == 13 && abs(reco_muon_pdgId_mother[k]) == mom_pdgId  && (reco_muon_isTightMuon[k] == true || !tight) && mu_k->Pt() > 4.) {
                      if (reco_muon_charge[j] + reco_muon_charge[k] == 0){
                          TLorentzVector dimuon(0.,0.,0.,0.);
                          dimuon = *mu_j + *mu_k;
                          if (dimuon.Pt()>5) {
                              if ((abs(mu_j->Eta()) < eta && abs(mu_k->Eta()) < eta)) {
                                  mass_barrel_h->Fill(dimuon.M());
                              }
                              else {
                                  mass_endcap_h->Fill(dimuon.M());
                              }
                          }
                      }
                  }
              }
          }
      }
  }
  mass_barrel_h->Scale(1./mass_barrel_h->GetEntries());
  mass_endcap_h->Scale(1./mass_endcap_h->GetEntries());

  pt_res_barrel_h->Scale(1./pt_res_barrel_h->GetEntries());
  pt_res_intermediate_h->Scale(1./pt_res_intermediate_h->GetEntries());
  pt_res_endcap_h->Scale(1./pt_res_endcap_h->GetEntries());

  TFile * output = new TFile(Form("h_%s.root", tail.c_str()), "recreate");
  mass_barrel_h->Write();
  mass_endcap_h->Write();
  pt_res_barrel_h->Write();
  pt_res_intermediate_h->Write();
  pt_res_endcap_h->Write();
  output->Close();

}

int main(int argc, char* argv[]) {

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i],"-t")) {
			tight = true;
			cout << "tight muons" << endl;
		}
	}

	system("rm effSLHC.csv effsigmaSLHC.csv");

    ofstream outfile;
    outfile.open("effSLHC.csv");
    outfile << "Type,old,new\n";

	ofstream sigmafile;
	sigmafile.open("effsigmaSLHC.csv");
	sigmafile << "Type,old,new\n";

	for (int i = 0; i < 2; i++) { // for each particle

	  int pdgId_mom = 511;
	  if (i == 1) pdgId_mom = 531;

	  string coda;
	  if (pdgId_mom == 511) coda = "_Bd";
	  else coda = "";
	  string old_c(Form("5314%s", coda.c_str()));
	  string new_c(Form("620SLHC7%s", coda.c_str()));

	  fill(old_c, pdgId_mom);
	  fill(new_c, pdgId_mom);
		
	  TFile * in_5314 = new TFile(Form("h_%s.root", old_c.c_str()));
	  TH1D * mass_barrel_h_5314 = (TH1D*)in_5314->Get(Form("mass_barrel_h_%s", old_c.c_str()));
	  TH1D * mass_endcap_h_5314 = (TH1D*)in_5314->Get(Form("mass_endcap_h_%s", old_c.c_str()));
	  TH1D * pt_res_barrel_h_5314 = (TH1D*)in_5314->Get(Form("pt_res_barrel_h_%s", old_c.c_str()));
	  TH1D * pt_res_intermediate_h_5314 = (TH1D*)in_5314->Get(Form("pt_res_intermediate_h_%s", old_c.c_str()));
	  TH1D * pt_res_endcap_h_5314 = (TH1D*)in_5314->Get(Form("pt_res_endcap_h_%s", old_c.c_str()));

	  TFile * in_620SLHC7 = new TFile(Form("h_%s.root", new_c.c_str()));
	  TH1D * mass_barrel_h_620SLHC7 = (TH1D*)in_620SLHC7->Get(Form("mass_barrel_h_%s", new_c.c_str()));
	  TH1D * mass_endcap_h_620SLHC7 = (TH1D*)in_620SLHC7->Get(Form("mass_endcap_h_%s", new_c.c_str()));
	  TH1D * pt_res_barrel_h_620SLHC7 = (TH1D*)in_620SLHC7->Get(Form("pt_res_barrel_h_%s", new_c.c_str()));
	  TH1D * pt_res_intermediate_h_620SLHC7 = (TH1D*)in_620SLHC7->Get(Form("pt_res_intermediate_h_%s", new_c.c_str()));
	  TH1D * pt_res_endcap_h_620SLHC7 = (TH1D*)in_620SLHC7->Get(Form("pt_res_endcap_h_%s", new_c.c_str()));

	  outfile << (i == 0 ? "Bd " : "Bs ") << "muon barrel," << setiosflags(ios::fixed) << setprecision(3) << pt_res_barrel_h_620SLHC7->GetRMS() << ",\n";
	  outfile << (i == 0 ? "Bd " : "Bs ") << "muon intermediate," << setiosflags(ios::fixed) << setprecision(3) << pt_res_intermediate_h_620SLHC7->GetRMS() << ",\n";
	  outfile << (i == 0 ? "Bd " : "Bs ") << "muon endcap," << setiosflags(ios::fixed) << setprecision(3) << pt_res_endcap_h_620SLHC7->GetRMS() << ",\n";
	  outfile << (i == 0 ? "Bd " : "Bs ") << "barrel," << setiosflags(ios::fixed) << setprecision(3) << mass_barrel_h_620SLHC7->GetRMS() << ",\n";
	  outfile << (i == 0 ? "Bd " : "Bs ") << "endcap," << setiosflags(ios::fixed) << setprecision(3) << mass_endcap_h_620SLHC7->GetRMS() << ",\n";

	  string label(Form("%d", pdgId_mom));

	  vector <double> sigmas = plot(mass_barrel_h_5314, mass_barrel_h_620SLHC7, label, true, false, i == 0 ? Bd : Bs);
	  sigmafile << (i == 0 ? "Bd " : "Bs ") << "barrel," << setiosflags(ios::fixed) << setprecision(3) << sigmas.at(1) << ",\n";

	  sigmas = plot(mass_endcap_h_5314, mass_endcap_h_620SLHC7, label, true, false, i == 0 ? Bd : Bs);
	  sigmafile << (i == 0 ? "Bd " : "Bs ") << "endcap," << setiosflags(ios::fixed) << setprecision(3) << sigmas.at(1) << ",\n";

	  sigmas = plot(pt_res_barrel_h_5314, pt_res_barrel_h_620SLHC7, label, true);
	  sigmafile << (i == 0 ? "Bd " : "Bs ") << "muon barrel," << setiosflags(ios::fixed) << setprecision(3) << sigmas.at(1) << ",\n";

	  sigmas = plot(pt_res_intermediate_h_5314, pt_res_intermediate_h_620SLHC7, label, true);
	  sigmafile << (i == 0 ? "Bd " : "Bs ") << "muon intermediate," << setiosflags(ios::fixed) << setprecision(3) << sigmas.at(1) << ",\n";

	  sigmas = plot(pt_res_endcap_h_5314, pt_res_endcap_h_620SLHC7, label, true);
	  sigmafile << (i == 0 ? "Bd " : "Bs ") << "muon endcap," << setiosflags(ios::fixed) << setprecision(3) << sigmas.at(1) << ",\n";
	}
	outfile << "\n" << endl;
	outfile.close();

	sigmafile << "\n" << endl;
	sigmafile.close();

	for (int i = 0; i < 2; i++) { // for each release

	  string release;
	  if (i == 0) release = "5314";
	  else release = "620SLHC7";

	  string bs_c(release);
	  string bd_c(Form("%s_Bd", release.c_str()));

	  TFile * in_bs = new TFile(Form("h_%s.root", bs_c.c_str()));
	  TH1D * mass_barrel_h_bs = (TH1D*)in_bs->Get(Form("mass_barrel_h_%s", bs_c.c_str()));
	  TH1D * mass_endcap_h_bs = (TH1D*)in_bs->Get(Form("mass_endcap_h_%s", bs_c.c_str()));

	  TFile * in_bd = new TFile(Form("h_%s.root", bd_c.c_str()));
	  TH1D * mass_barrel_h_bd = (TH1D*)in_bd->Get(Form("mass_barrel_h_%s", bd_c.c_str()));
	  TH1D * mass_endcap_h_bd = (TH1D*)in_bd->Get(Form("mass_endcap_h_%s", bd_c.c_str()));
	  mass_barrel_h_bd->Scale(1./9.);
	  mass_endcap_h_bd->Scale(1./9.);

	  string label(Form("511531_%s", release.c_str()));
	  plot(mass_barrel_h_bs, mass_barrel_h_bd, label);
	  plot(mass_endcap_h_bs, mass_endcap_h_bd, label);

	}

	return 1;
}
