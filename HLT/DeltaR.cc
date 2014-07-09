#include "TFile.h"
#include "TH1D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include <TTreeReaderArray.h>
#include "TLorentzVector.h"
#include "TCanvas.h"

#include <iostream> 
#include <vector> 
 
using namespace std;

const float mu_mass = 0.105658;

void print(TH1D * h) {
	TCanvas * c = new TCanvas("c", "c", 600, 600);
	h->Draw();
	c->Print(Form("plots/%s.png", h->GetName()));
	
	delete c;
}

int main(int argc, char* argv[]) {
	
	TFile * f_input = TFile::Open("B0ToJpsi_L1Tree.root");

	TTreeReader reader("l1NtupleProducer/L1Tree", f_input);

	TTreeReaderValue<std::vector<int> > CandBx(reader, "CandBx");
	TTreeReaderValue<std::vector<int> > Cha(reader, "Cha");
	TTreeReaderValue<std::vector<int> > Qual(reader, "Qual");
	TTreeReaderValue<std::vector<float> > Pt(reader, "Pt");
	TTreeReaderValue<std::vector<float> > Eta(reader, "Eta");
	TTreeReaderValue<std::vector<float> > Phi(reader, "Phi");
	TTreeReaderValue<Int_t > N(reader, "N");

	TTreeReaderValue<std::vector<int> > id(reader, "id");
	TTreeReaderValue<std::vector<int> > status(reader, "status");
	TTreeReaderValue<std::vector<int> > parent_id(reader, "parent_id");
	TTreeReaderValue<std::vector<float> > px(reader, "px");
	TTreeReaderValue<std::vector<float> > py(reader, "py");
	TTreeReaderValue<std::vector<float> > pz(reader, "pz");
	TTreeReaderValue<std::vector<float> > e(reader, "e");

	TH1D * h_DeltaR = new TH1D("DeltaR", "DeltaR", 100, 0., 2.);
	TH1D * h_DeltaEta = new TH1D("DeltaEta", "DeltaEta", 100, 0., 2.);
	TH1D * h_DeltaPhi = new TH1D("DeltaPhi", "DeltaPhi", 100, 0., 2.);
	TH1D * h_DeltaPtRel = new TH1D("DeltaPtRel", "DeltaPtRel", 100, -2., 2.);

 	while (reader.Next()) {
 		if (reader.GetCurrentEntry()%10000 == 0) cout << reader.GetCurrentEntry() << " / " << reader.GetEntries(false) << endl;
 		
 		float gen_pt_max = 0;
 		size_t gen_max = 0;
		for (size_t g = 0; g < id->size(); g++) {
 			if (abs(id->at(g)) == 13 && status->at(g) == 1 && parent_id->at(g) == 443) {
 				TLorentzVector * gen_muon = new TLorentzVector(0., 0., 0., 0.);
 				gen_muon->SetPxPyPzE(px->at(g), py->at(g), pz->at(g), e->at(g));
 				if (gen_muon->Pt() > gen_pt_max) {
 					gen_pt_max = gen_muon->Pt();
 					gen_max = g;
 				}
 				delete gen_muon;
 			}
 		}

 		float L1_pt_max = 0;
 		size_t L1_max = 0;
 		for (size_t m = 0; m < Pt->size(); m++) {
 			if (CandBx->at(m) == 0 && Qual->at(m) > 6) {
 				TLorentzVector * L1_muon = new TLorentzVector(0., 0., 0., 0.);
 				L1_muon->SetPtEtaPhiM(Pt->at(m), Eta->at(m), Phi->at(m), mu_mass);
 				if (L1_muon->Pt() > L1_pt_max) {
 					L1_pt_max = L1_muon->Pt();
 					L1_max = m;
 				}
 				delete L1_muon;
 			}
 		}

 		for (size_t g = 0; g < id->size(); g++) {
 			if (g != gen_max) continue;
 			if (abs(id->at(g)) == 13 && status->at(g) == 1 && parent_id->at(g) == 443) {
 				TLorentzVector * gen_muon = new TLorentzVector(0., 0., 0., 0.);
 				gen_muon->SetPxPyPzE(px->at(g), py->at(g), pz->at(g), e->at(g));
 				for (size_t m = 0; m < Pt->size(); m++) {
 					if (m != L1_max) continue;
 					if (CandBx->at(m) == 0 && Qual->at(m) > 6) {
 						TLorentzVector * L1_muon = new TLorentzVector(0., 0., 0., 0.);
 						L1_muon->SetPtEtaPhiM(Pt->at(m), Eta->at(m), Phi->at(m), mu_mass);
 						Double_t deltaR = gen_muon->DeltaR(*L1_muon);
 						Double_t deltaEta = fabs(gen_muon->Eta() - L1_muon->Eta());
 						Double_t deltaPhi = gen_muon->DeltaPhi(*L1_muon);
 						Double_t DeltaPtRel = (L1_muon->Pt() - gen_muon->Pt()) / gen_muon->Pt();
 						h_DeltaR->Fill(deltaR);
 						h_DeltaEta->Fill(deltaEta);
 						h_DeltaPhi->Fill(deltaPhi);
 						h_DeltaPtRel->Fill(DeltaPtRel);
 						delete L1_muon;
 					}
 				}
				delete gen_muon;
 			}
 		}

	}

	print(h_DeltaR);
	print(h_DeltaEta);
	print(h_DeltaPhi);
	print(h_DeltaPtRel);

	return true;
}