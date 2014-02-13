/*
 * makeRetina.cc
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 *
 *
 *      Test retina with circles transformed to lines with a conformal transformation
 *
 *
 */

#include <cstdlib>

#include "LayerGeometry.h"
#include "HitCollection.h"
#include "ConformalTransf.h"
#include "RetinaTrackFitter.h"

#include "print.h"


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

	// pt = 2 GeV
	// B = 3.8 T
	// sqrt(a^2 + b^2) = R = pt / (0.3 * B) = 1.8 m = 180 cm
	// pt = 2 GeV --> R = 1.8 m = 180 cm
	// pt = 3 GeV --> R = 2.6 m = 260 cm
	// pt = 4 GeV --> R = 3.5 m = 350 cm
	// pt = 20 GeV --> R = 17.5 m = 1750 cm
	// pt = 100 GeV --> R = 87.7 m = 8770 cm
	// r = 2R cos (phi - phi0) origin lies on circle

	unsigned int pbins(21);
	unsigned int qbins(41);
	double pmin(-.2);
	double pmax(1.5);
	double qmin(-.001);
	double qmax(.004);
	double qstep = (qmax-qmin)/(double)qbins;
	double pstep = (pmax-pmin)/(double)pbins;
	double sigma = 0.001;
	double minWeight = 0.;
	cout << "qstep = " << qstep << " /cm; pstep = " << pstep << endl;
	TH1D pt_res("pt_res_circle", "p_{T} resolution", 100, -100, 100);
	TH2D p_q("p_q_circle", "p_q;p;q", pbins, pmin, pmax, qbins, qmin, qmax);
	TH1D p_res("p_res_circle", "p resolution / p step;[p]", 100, -1, 1);
	TH1D q_res("q_res_circle", "q resolution / q step;[q]", 100, -1, 1);
	TH2D p_q_res("p_q_res_circle", "reduced p q resolution;[p];[q]", 20, -1., 1., 20, -1., 1.);
	TH2D pt_pt_res("pt_pt_res", "gen / reco p_{T};reco p_T [GeV];gen p_T [GeV]", 50, 0., 50., 50, 0., 50.);

	LayerGeometry LG;
	TRandom3 rand;
	cout << "seed: " << rand.GetSeed() << endl; //4357

	for (int i = 0; i < events; i++) {

		Double_t phi_rnd = rand.Uniform(pi/2.,pi) + rand.Integer(2)*pi;
		Double_t k_rnd = rand.Uniform(1e-4, 5.e-3);
		Double_t r_rnd = 1./k_rnd;
//		cout << "Curvature radius = " << r_rnd << "; Center phi = " << phi_rnd << endl;
		string name(Form("circle_%d", i));
		HitsGenerator HG(LG, cylinder, name); //plain
		HG.addCircle(r_rnd, phi_rnd);
		HG.cleanHitsRPhi(20., 120., 0., pi/4.);

		HitCollection hitscollection = HG.getHitCollection();
		unsigned int hits = hitscollection.size();
		if (hits < 6) { // skip events with few generated hits
			i--;
			continue;
		}
		if (draw) {
			cout << "hits = " << hits << endl;
			hitscollection.printHits();
		}

		ConformalTransf CF;
		CF.setNormHitCollection(hitscollection);
		CF.from_norm_to_conf(true);
		HitCollection confhitscollection = CF.getConfHitCollection();
		confhitscollection.setName(Form("circleconf_%d", i));
		pqPoint truepq = confhitscollection.drawHits(true, draw);
		p_q.Fill(truepq.p, truepq.q);

		RetinaTrackFitter rtf(confhitscollection, pbins, qbins, pmin, pmax, qmin, qmax, sigma, minWeight, name);

		rtf.fillPQGrid();

		rtf.findMaxima();

		vector <circle> cp =	rtf.getCircles();


		if (draw) {
			rtf.printMaxima();

			rtf.drawPQGrid();

			double w = -1.;
			unsigned int best;
			vector <pqPoint> pqCollection = rtf.getPQCollection();
			for (unsigned int i = 0; i < pqCollection.size(); i++) {
				if (pqCollection[i].w > w) {
					w = pqCollection[i].w;
					best = i;
				}
			}
			drawTracks(confhitscollection, rtf.getPQCollection(), truepq, best, i, "circle");

			CF.from_conf_to_norm(true);
			HitCollection normhitscollection = CF.getNormHitCollection();
			normhitscollection.drawHits(false);

			drawCircles(normhitscollection, cp, r_rnd, best, i);
		}

		pqPoint bestpq = rtf.getBestPQ();
		if (bestpq.w < 0.) continue;
		double p_res_d = (bestpq.p - truepq.p)/pstep;
		double q_res_d = (bestpq.q - truepq.q)/qstep;
		p_res.Fill(p_res_d);
		q_res.Fill(q_res_d);
		p_q_res.Fill(p_res_d, q_res_d);

		circle bestcircle = rtf.getBestCircle();
		double best_r = sqrt(bestcircle.a*bestcircle.a + bestcircle.b*bestcircle.b);
		double pt_res_d = (best_r - r_rnd) * 0.3 / 100. * 3.8;
		pt_res.Fill(pt_res_d);
		pt_pt_res.Fill(best_r * 0.3 / 100. * 3.8, r_rnd * 0.3 / 100. * 3.8);
	}

	gStyle->SetOptStat(111111);

	DrawCanvas2(p_q);

	DrawCanvas(p_res);
	DrawCanvas(q_res);
	DrawCanvas2(p_q_res);

	DrawCanvas(pt_res);
	DrawCanvas2(pt_pt_res);

	return EXIT_SUCCESS;
}


