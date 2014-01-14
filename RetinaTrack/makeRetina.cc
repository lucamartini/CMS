/*
 * makeRetina.cc
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 *
 *
 */

#include "HitsGenerator.h"
#include "RetinaTrackFitter.h"

void DrawCanvas(TH1D h) {
	TCanvas c("c", "c", 600, 600);
	h.Draw();
	c.Print(Form("figs/%s.pdf", h.GetName()));
}

void DrawCanvas2(TH2D h) {
	gStyle->SetPalette(54);
	TCanvas c("c", "c", 600, 600);
	h.SetStats(false);
	h.Draw("COLZ");
	c.Print(Form("figs/%s.pdf", h.GetName()));
}

int main(int argc, char* argv[]) {
//	double pi;
//	pi = acos(-1);

	// pt = 2 GeV
	// B = 3.8 T
	// sqrt(a^2 + b^2) = R = pt / (0.3 * B) = 1.8 m = 180 cm
	// pt = 2 GeV --> R = 1.8 m = 180 cm
	// pt = 3 GeV --> R = 2.6 m = 260 cm
	// pt = 4 GeV --> R = 3.5 m = 350 cm
	// pt = 20 GeV --> R = 17.5 m = 1750 cm
	// pt = 100 GeV --> R = 87.7 m = 8770 cm
	// r = 2R cos (phi - phi0) origin lies on circle

	unsigned int pbins(32);
	unsigned int qbins(12);
  double pmin(0.5);
  double pmax(10.);
  double qmin(0.);
  double qmax(0.03);
	double qstep = (qmax-qmin)/(double)qbins;
	double pstep = (pmax-pmin)/(double)pbins;
	cout << "qstep = " << qstep << " /cm; pstep = " << pstep << endl;
	TH1D pt_res("pt_res", "p_{T} resolution;[GeV]", 100, -10., 10.);
	TH1D phi_res("phi_res", "#phi resolution;[rad]", 100, -.2, .2);
	TH1D p_res("p_res", "p resolution / p step;[p]", 100, -1, 1);
	TH1D q_res("q_res", "q resolution / q step[q]", 100, -1, 1);
	TH2D pt_phi_res("pt_phi_res", "pt_phi_res;p_{T}[GeV];#phi", 20, -10., 10., 20, -2., 2.);
	TH2D p_q_res("p_q_res", "reduced p q resolution;[p];[q]", 20, -1., 1., 20, -1., 1.);

  vector <double> phi0;
  // let's take second phi sector: [pi/4, pi/2]
  double phi_i = 3*pi/4.;
  // phi_i += pi/24;
  while (phi_i < pi) {
  	phi0.push_back(phi_i);
  	phi_i += pi/48.;
  }

  vector <double> R;
  R.push_back(180.);
  R.push_back(260.);
  R.push_back(350.);
//  R.push_back(1750.);
//  R.push_back(8770.);

	int size_phi = phi0.size();
	int size_R = R.size();

	for (int i = 0; i < size_phi; i++) {
		for (int j = 0; j < size_R; j++) {
//			if (i != 1 || j != 0) continue;
			cout << endl << "r_gen, phi_gen = " << R[j] << ", " << phi0[i] << endl;
			HitsGenerator HG;
			HG.addCircle(R[j], phi0[i], false);
//			HG.addCircle(R[j], phi0[i+1], false);
			HG.cleanHitsRPhi(0., 100000., pi/4., pi/2.);
			vector <hit> hitscollection = HG.getHitCollection();
			unsigned int hits = hitscollection.size();
			cout << "hits = " << hits << endl;
			if (hits == 0) continue;

			RetinaTrackFitter rtf(hitscollection, pbins, qbins, pmin, pmax, qmin, qmax, false, Form("%d_%d", i, j));
			rtf.setR(R[j]);
			rtf.setPhi(phi0[i]);
			rtf.setA(R[j], phi0[i]);
			rtf.setB(R[j], phi0[i]);

			rtf.drawHits();

			rtf.setConfHits();
			pqPoint pqtrue = rtf.drawHitsConf();

			rtf.fillPQGrid();
			rtf.drawPQGrid();

			rtf.findMaxima();
			rtf.printMaxima();
			rtf.drawHitsConfRetina();

			rtf.getCircles();
			rtf.drawCircles();

			track track_ij = rtf.getBestTrack();
			double pt_reco = track_ij.pt;
			if (pt_reco < 0.) continue;
			double phi_reco = track_ij.phi;

			double pt_res_d = pt_reco - R[j]/100.*0.3*3.8;
			double phi_res_d = phi_reco - phi0[i];
			pt_res.Fill(pt_res_d);
			phi_res.Fill(phi_res_d);
			pt_phi_res.Fill(pt_res_d, phi_res_d);
			pqPoint bestpq = rtf.getBestPQ();
			double p_res_d = (bestpq.p - pqtrue.p)/pstep;
			double q_res_d = (bestpq.q - pqtrue.q)/qstep;
			p_res.Fill(p_res_d);
			q_res.Fill(q_res_d);
			p_q_res.Fill(p_res_d, q_res_d);

		}
	}

	DrawCanvas(pt_res);
	DrawCanvas(phi_res);
	DrawCanvas2(pt_phi_res);
	DrawCanvas(p_res);
	DrawCanvas(q_res);
	DrawCanvas2(p_q_res);

	return EXIT_SUCCESS;
}


