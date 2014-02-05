/*
 * RetinaSimple.cc
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 *
 *      Test retina with straight lines
 *
 */

#include "LayerGeometry.h"
#include "HitCollection.h"
#include "ConformalTransf.h"
#include "RetinaTrackFitter.h"

int main(int argc, char* argv[]) {

	int events = 1;
	bool draw = false;
	for (int i = 1; i < argc; i++) {
	  if (!strcmp(argv[i],"-d")) {
	  	draw = true;
	    cout << "drawing plots for each event" << endl;
	  }
	  if (!strcmp(argv[i],"-n")) {
	  	events = atoi(argv[i+1]);
	  }
	}
	cout << "events = " << events << endl;
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
	unsigned int qbins(20);
	double pmin(-0.1);
	double pmax(2.0);
	double qmin(-50);
	double qmax(50);
	double qstep = (qmax-qmin)/(double)qbins;
	double pstep = (pmax-pmin)/(double)pbins;
	double sigma = 2;
	cout << "qstep = " << qstep << " cm; pstep = " << pstep << endl;
	TH1D p_res("p_res_line", "p resolution / p step;[p]", 100, -1, 1.);
	TH1D q_res("q_res_line", "q resolution / q step;[q]", 100, -1, 1.);
	TH2D p_q_res("p_q_res_line", "reduced p q resolution;[p];[q]", 20, -1., 1., 20, -0.5, 0.5);

	LayerGeometry LG;
	TRandom3 rand;
	cout << "seed: " << rand.GetSeed() << endl; //4357

	for (int i = 0; i < events; i++) {

		Double_t phi_rnd = rand.Uniform(pi/4.);
		Double_t b_rnd = 0;
		b_rnd = rand.Gaus(0., 1.);
//if (i == 0) continue;
		string name(Form("line_%d", i));
		HitsGenerator HG(LG, plain, name); //plain
		HG.addLine(phi_rnd, b_rnd); // p = 1; q = 0

		HitCollection hitscollection = HG.getHitCollection();
//		unsigned int hits = hitscollection.size();

		pqPoint truepq = hitscollection.drawHits(true, draw);

		RetinaTrackFitter rtf(hitscollection, pbins, qbins, pmin, pmax, qmin, qmax, sigma, name);

		rtf.fillPQGrid();

		rtf.findMaxima();
		if (draw) {
			rtf.printMaxima();

			rtf.drawPQGrid();
			rtf.drawTracks();
		}

		pqPoint bestpq = rtf.getBestPQ();
		if (bestpq.w < 0.) continue;
		double p_res_d = (bestpq.p - truepq.p)/pstep;
		double q_res_d = (bestpq.q - truepq.q)/qstep;
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





