/*
 * RetinaSimple.cc
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 */

#include "LayerGeometry.h"
#include "HitCollection.h"
#include "HitsGenerator.h"
#include "ConformalTransf.h"
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

	unsigned int pbins(20);
	unsigned int qbins(40);
	double pmin(0.);
	double pmax(1000.);
	double qmin(-0.2);
	double qmax(0.2);
	double qstep = (qmax-qmin)/(double)qbins;
	double pstep = (pmax-pmin)/(double)pbins;
	cout << "qstep = " << qstep << " /cm; pstep = " << pstep << endl;
	TH1D p_res("p_res", "p resolution / p step;[p]", 100, -1, 1.);
	TH1D q_res("q_res", "q resolution / q step;[q]", 100, -1, 1.);
	TH2D p_q_res("p_q_res", "reduced p q resolution;[p];[q]", 20, -1., 1., 20, -0.5, 0.5);

	LayerGeometry LG;
	TRandom3 rand;
	cout << "seed: " << rand.GetSeed() << endl; //4357

	for (int i = 0; i < 10; i++) {


		Double_t phi_rnd = rand.Uniform(pi/2);
		Double_t b_rnd = rand.Gaus(0., 0.01);
//if (i == 0) continue;
		string name(Form("simple_%d", i));
		HitsGenerator HG(LG, plain, name);
		HG.addLine(phi_rnd, b_rnd); // p = 1; q = 0

		HitCollection hitscollection = HG.getHitCollection();
		unsigned int hits = hitscollection.size();
//		cout << "hits = " << hits << endl;

		ConformalTransf CF(hitscollection);
		CF.from_cart_to_polar(false);
		HitCollection confhitscollection = CF.getHitCollection();

		RetinaTrackFitter rtf(confhitscollection, pbins, qbins, pmin, pmax, qmin, qmax, 0.1, false, name);

		rtf.fillPQGrid();

		rtf.findMaxima();
		rtf.printMaxima();

		hitscollection.drawHits(1);
		rtf.drawPQGrid();
		rtf.drawTracks();

		pqPoint bestpq = rtf.getBestPQ();
		double p_res_d = (bestpq.p - tan(phi_rnd))/pstep;
		double q_res_d = (bestpq.q - b_rnd)/qstep;
		p_res.Fill(p_res_d);
		q_res.Fill(q_res_d);
		p_q_res.Fill(p_res_d, q_res_d);
	}

	gStyle->SetOptStat(111111);
	DrawCanvas(p_res);
	DrawCanvas(q_res);
	DrawCanvas2(p_q_res);

	return EXIT_SUCCESS;
}





