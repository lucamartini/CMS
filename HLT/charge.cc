#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include <TTreeReaderArray.h>
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"

#include <iostream> 
#include <vector> 
#include <string>
 
using namespace std;

const float mu_mass = 0.105658;

void print(TH1 * h) {
	TCanvas * c = new TCanvas("c", "c", 600, 600);
	h->Draw();

	c->Print(Form("plots/%s.pdf", h->GetName()));
	
	delete c;
}

void print_eff(TEfficiency * e) {
	TCanvas * c = new TCanvas("c", "c", 600, 600);
	e->Draw();

	string name(e->GetName());
	std::size_t found = name.find("eta");
  	if (found != std::string::npos) { 
		gPad->Update();    
		e->GetPaintedGraph()->GetXaxis()->SetLimits(-2.5,  2.5);
		c->Update();
	}

	c->Print(Form("plots/%s.pdf", e->GetName()));
	
	delete c;
}

void print_eff2D(TEfficiency * e) {
	TCanvas * c = new TCanvas("c", "c", 800, 600);
	e->Draw("COLZ");

	gPad->Update();    
	e->GetPaintedHistogram()->SetMaximum(1.);          
	e->GetPaintedHistogram()->SetMinimum(0.);     
	c->Update();
	c->Print(Form("plots/%s.pdf", e->GetName()));
	
	delete c;
}

void print_2eff(TEfficiency * e1, TEfficiency * e2) {

	TCanvas * c = new TCanvas("c", "c", 600, 600);
	e1->SetMarkerColor(kBlue);
	e2->SetMarkerColor(kRed);
	e1->SetLineColor(kBlue);
	e2->SetLineColor(kRed);
	e1->SetMarkerStyle(22);
	e2->SetMarkerStyle(23);
	e1->Draw("AP");
	e2->Draw("P same");

 	c->Update();

 	gPad->Update();    
	e1->GetPaintedGraph()->GetHistogram()->SetMaximum(1.);          
	e1->GetPaintedGraph()->GetHistogram()->SetMinimum(0.);     
	c->Update();

	TLegend *l = new TLegend(0.5,0.2,0.88,0.35, 0,"brNDC");
	l->AddEntry(e1,  "L1 eff", "pel");
	l -> AddEntry(e2, "L1 charge eff", "pel");
	l -> Draw("same");

	string name(e1->GetName());
	std::size_t found = name.find("eta");
  	if (found != std::string::npos) { 
		gPad->Update();    
		e1->GetPaintedGraph()->GetXaxis()->SetLimits(-2.5,  2.5);
		c->Update();
	}   
	
	c->Print(Form("plots/%s_%s.pdf", e1->GetName(), e2->GetName()));
	
	delete c;
}

void print_neff(vector <TEfficiency * > v) {

	TCanvas * c = new TCanvas("c", "c", 600, 600);
	for (size_t i = 0; i < v.size(); i++) {
		v[i]->SetMarkerColor(kRed + 5 * i);
		v[i]->SetLineColor(kRed + 5 * i);
		v[i]->SetMarkerStyle(22 + i);
		if (i == 0) v[i]->Draw("AP");
		else v[i]->Draw("P same");
	}

 	c->Update();

	gPad->Update();    
	v[0]->GetPaintedGraph()->GetHistogram()->SetMaximum(1.);          
	v[0]->GetPaintedGraph()->GetHistogram()->SetMinimum(0.);     
	c->Update();

	TLegend *l = new TLegend(0.5,0.2,0.88,0.35, 0,"brNDC");
	for (size_t i = 0; i < v.size(); i++) l->AddEntry(v[i], v[i]->GetName(), "pel");
	l -> Draw("same");

	string name(v[0]->GetName());
	std::size_t found = name.find("eta");
  	if (found != std::string::npos) { 
		gPad->Update();    
		v[0]->GetPaintedGraph()->GetXaxis()->SetLimits(-2.5,  2.5);
		c->Update();
	}

	// e1->GetPaintedHistogram()->SetMaximum(1.);          
	// e1->GetPaintedHistogram()->SetMinimum(0.);     
	
	c->Print(Form("plots/%s_same.pdf", v[0]->GetName()));
	
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

	TEfficiency * eff_pt_L1 = new TEfficiency("eff_pt_L1", "eff_pt_L1;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);
	TEfficiency * eff_pt_L1_chg = new TEfficiency("eff_pt_L1_chg", "eff_pt_L1_chg;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);

	TEfficiency * eff_pter16_L1 = new TEfficiency("eff_pter16_L1", "eff_pt_L1 for |#eta|<1.6;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);
	TEfficiency * eff_pter16_L1_chg = new TEfficiency("eff_pter16_L1_chg", "eff_pt_L1_chg for |#eta|<1.6;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);

	TEfficiency * eff_pter1_L1 = new TEfficiency("eff_pter1_L1", "eff_pt_L1 for |#eta|<1.;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);
	TEfficiency * eff_pter1_L1_chg = new TEfficiency("eff_pter1_L1_chg", "eff_pt_L1_chg for |#eta|<1.;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);

	TEfficiency * eff_ptee1_L1 = new TEfficiency("eff_ptee1_L1", "eff_pt_L1 for |#eta|>1.;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);
	TEfficiency * eff_ptee1_L1_chg = new TEfficiency("eff_ptee1_L1_chg", "eff_pt_L1_chg for |#eta|>1.;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);

	TEfficiency * eff_eta_L1 = new TEfficiency("eff_eta_L1", "eff_eta_L1;#eta^{gen};#varepsilon", 100, -2.5, 2.5);
	TEfficiency * eff_eta_L1_chg = new TEfficiency("eff_eta_L1_chg", "eff_eta_L1_chg;#eta^{gen};#varepsilon", 100, -2.5, 2.5);

	TEfficiency * eff_eta_pt_L1 = new TEfficiency("eff_eta_pt_L1", "eff_eta_pt_L1;#eta;p_{T};#varepsilon", 60, -2.5, 2.5, 45, 0., 25.);
	TEfficiency * eff_eta_pt_L1_chg = new TEfficiency("eff_eta_pt_L1_chg", "eff_eta_pt_L1_chg;#eta;p_{T};#varepsilon", 60, -2.5, 2.5, 45, 0., 25.);

	TEfficiency * eff_pt_chg = new TEfficiency("eff_pt_chg", "eff_pt_chg;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);	
	TEfficiency * eff_pter16_chg = new TEfficiency("eff_pter16_chg", "eff_pter16_chg for |#eta|<1.6;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);
	TEfficiency * eff_pter1_chg = new TEfficiency("eff_pter1_chg", "eff_pter1_chg for |#eta|<1.;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);
	TEfficiency * eff_eta_chg = new TEfficiency("eff_eta_chg", "eff_eta_chg;#eta;p_{T};#varepsilon", 100, -2.5, 2.5);
	TEfficiency * eff_ptee1_chg = new TEfficiency("eff_ptee1_chg", "eff_ptee1_chg for |#eta|>1.;p_{T}^{gen} (GeV);#varepsilon", 45, 0., 25.);

 	while (reader.Next()) {
 		// if (reader.GetCurrentEntry() > 100) break;
 		if (reader.GetCurrentEntry()%10000 == 0) cout << reader.GetCurrentEntry() << " / " << reader.GetEntries(false) << endl;

 		for (size_t g = 0; g < id->size(); g++) {
 			if (abs(id->at(g)) == 13 && status->at(g) == 1 && parent_id->at(g) == 443) {

 				TLorentzVector * gen_muon = new TLorentzVector(0., 0., 0., 0.);
 				gen_muon->SetPxPyPzE(px->at(g), py->at(g), pz->at(g), e->at(g));

 				bool b_L1 = false;
 				bool b_L1_chg = false;

 				Double_t best_deltaR = 1000.;
 				int best_m = -1;

 				for (size_t m = 0; m < Pt->size(); m++) {
 					if (CandBx->at(m) == 0 && Qual->at(m) > 6) {
 						TLorentzVector * L1_muon = new TLorentzVector(0., 0., 0., 0.);
 						L1_muon->SetPtEtaPhiM(Pt->at(m), Eta->at(m), Phi->at(m), mu_mass);
 						Double_t deltaR = gen_muon->DeltaR(*L1_muon);
 						// Double_t deltaEta = fabs(gen_muon->Eta() - L1_muon->Eta());
 						// Double_t deltaPhi = gen_muon->DeltaPhi(*L1_muon);
 						// Double_t DeltaPtRel = (L1_muon->Pt() - gen_muon->Pt()) / gen_muon->Pt();

 						if (deltaR < best_deltaR) {
 							best_deltaR = deltaR;
 							best_m = m;
 						}
 						delete L1_muon;
 					}
 				}
 				if (best_m > -1) {
					TLorentzVector * L1_muon = new TLorentzVector(0., 0., 0., 0.);
					L1_muon->SetPtEtaPhiM(Pt->at(best_m), Eta->at(best_m), Phi->at(best_m), mu_mass);
					Double_t deltaEta = fabs(gen_muon->Eta() - L1_muon->Eta());
 					Double_t DeltaPtRel = (L1_muon->Pt() - gen_muon->Pt()) / gen_muon->Pt();

					if (deltaEta < 0.2 && DeltaPtRel < 0.4) {
 						b_L1 = true;
 							
 						if ( id->at(g)*Cha->at(best_m) < 0 ) {
 							b_L1_chg = true;
 						}
 					}
 					delete L1_muon;
 				}
 	
 				eff_pt_L1->Fill(b_L1, gen_muon->Pt());
 				eff_pt_L1_chg->Fill(b_L1_chg, gen_muon->Pt());

 				eff_eta_L1->Fill(b_L1, gen_muon->Eta());
 				eff_eta_L1_chg->Fill(b_L1_chg, gen_muon->Eta());

 				if (fabs(gen_muon->Eta())<1.6) {
 					eff_pter16_L1->Fill(b_L1, gen_muon->Pt());
 					eff_pter16_L1_chg->Fill(b_L1_chg, gen_muon->Pt());
 				
 					if (fabs(gen_muon->Eta())<1.) {
 						eff_pter1_L1->Fill(b_L1, gen_muon->Pt());
 						eff_pter1_L1_chg->Fill(b_L1_chg, gen_muon->Pt());
 					}
 				}

				if (fabs(gen_muon->Eta())>1.) {
 					eff_ptee1_L1->Fill(b_L1, gen_muon->Pt());
 					eff_ptee1_L1_chg->Fill(b_L1_chg, gen_muon->Pt());
 				}

 				eff_eta_pt_L1->Fill(b_L1, gen_muon->Eta(), gen_muon->Pt());
 				eff_eta_pt_L1_chg->Fill(b_L1_chg, gen_muon->Eta(), gen_muon->Pt());

				if (b_L1) {
					eff_pt_chg->Fill(b_L1_chg, gen_muon->Pt());
					eff_eta_chg->Fill(b_L1_chg, gen_muon->Eta());
					if (fabs(gen_muon->Eta())<1.6) {
						eff_pter16_chg->Fill(b_L1_chg, gen_muon->Pt());
						if (fabs(gen_muon->Eta())<1.) {
							eff_pter1_chg->Fill(b_L1_chg, gen_muon->Pt());
						}
					}
					if (fabs(gen_muon->Eta())>1.) {
						eff_ptee1_chg->Fill(b_L1_chg, gen_muon->Pt());
					}
				}

				delete gen_muon;
 			}
 		}
	}

	print_eff(eff_pt_L1);
	print_eff(eff_pt_L1_chg);
	print_eff(eff_pter16_L1);
	print_eff(eff_pter16_L1_chg);
	print_eff(eff_pter1_L1);
	print_eff(eff_pter1_L1_chg);

	print_eff(eff_eta_L1);
	print_eff(eff_eta_L1_chg);

	print_eff(eff_pt_chg);
	print_eff(eff_pter16_chg);
	print_eff(eff_pter1_chg);
	print_eff(eff_eta_chg);
	print_eff(eff_ptee1_chg);

	print_eff2D(eff_eta_pt_L1);
	print_eff2D(eff_eta_pt_L1_chg);

	print_2eff(eff_pt_L1, eff_pt_L1_chg);
	print_2eff(eff_pter16_L1, eff_pter16_L1_chg);
	print_2eff(eff_pter1_L1, eff_pter1_L1_chg);
	print_2eff(eff_eta_L1, eff_eta_L1_chg);
	print_2eff(eff_ptee1_L1, eff_ptee1_L1_chg);

	vector <TEfficiency * > v_eff_pt_L1;
	vector <TEfficiency * > v_eff_pt_L1_chg;

	v_eff_pt_L1.push_back(eff_pt_L1);
	v_eff_pt_L1.push_back(eff_pter16_L1);
	v_eff_pt_L1.push_back(eff_pter1_L1);

	v_eff_pt_L1_chg.push_back(eff_pt_L1_chg);
	v_eff_pt_L1_chg.push_back(eff_pter16_L1_chg);
	v_eff_pt_L1_chg.push_back(eff_pter1_L1_chg);

	print_neff(v_eff_pt_L1);
	print_neff(v_eff_pt_L1_chg);


	// TH1D * eff = new TH1D("eff", "eff", 45, 0., 25.);
	// for (int i = 1; i <= 45; i++) {
	// 	if (eff_pter1_L1->GetEfficiency(i) != 0) {
	// 		Double_t eff_i = eff_pter1_L1_chg->GetEfficiency(i) / eff_pter1_L1->GetEfficiency(i);
	// 		eff->SetBinContent(i, eff_i);
	// 	}
	// }
	// print(eff);

	return true;
}