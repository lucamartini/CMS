/*
 * RetinaSimple.cc
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 *
 *      Test retina with straight lines
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

	unsigned int pbins(21);
	unsigned int qbins(21);
//	double pmin(0.05);
//	double pmax(1.05);
//	double qmin(-0.06);
//	double qmax(0.06);
//	double sigma = 2;
	double pmin(0.05);
	double pmax(1.1);
	double qmin(-50);
	double qmax(50);
	double sigma = 6;
	double qstep = (qmax-qmin)/(double)qbins;
	double pstep = (pmax-pmin)/(double)pbins;

	sigma = sqrt(qstep*pstep*1000);
//	sigma = 1;

	double minWeight = 2.;
	cout << "qstep = " << qstep << " cm; pstep = " << pstep << "; sigma = " << sigma << endl;
	TH1D p_res("p_res_line", "p resolution / p step;[p]", 100, -2., 2.);
	TH1D q_res("q_res_line", "q resolution / q step;[q]", 100, -2., 2.);
	TH2D p_q_res("p_q_res_line", "reduced p q resolution;[p];[q]", 50, -2., 2., 50, -2., 2.);
	TH1D m_h("m_h", "m_h", pbins, pmin, pmax);
	TH1D b_h("b_h", "b_h", qbins, qmin, qmax);
	TH1I multiplicity_h("multiplicity_h", "multiplicity_h", 10, -0.5, 9.5);
	TH2D p_q("p_q_gen_line", "gen p q;p;q", pbins*10., pmin, pmax, qbins*10., qmin, qmax);
	TH2D p_gen_reco("p_gen_reco", "p gen / reco;gen p;reco p", pbins*10., pmin, pmax, pbins*10., pmin, pmax);
	TH2D q_gen_reco("q_gen_reco", "q gen / reco;gen q;reco q", qbins*10., qmin, qmax, qbins*10., qmin, qmax);

	LayerGeometry LG;
	TRandom3 rand;
	cout << "seed: " << rand.GetSeed() << endl; //4357

	for (int i = 0; i < events; i++) {

		Double_t phi_rnd = rand.Uniform(0.1, pi/4.);
		Double_t b_rnd = 0;
		b_rnd = rand.Gaus(0., 10.);
//		b_rnd = rand.Uniform(0., 1.);

		m_h.Fill(tan(phi_rnd));
		b_h.Fill(b_rnd);

		string name(Form("line_%d", i));
		HitsGenerator HG(LG, plain, name); //plain
		HG.addLine(phi_rnd, b_rnd); // p = 1; q = 0

		HitCollection hitscollection = HG.getHitCollection();

		pqPoint truepq = hitscollection.drawHits(true, draw);
		p_q.Fill(truepq.p, truepq.q);

		RetinaTrackFitter rtf(hitscollection, pbins, qbins, pmin, pmax, qmin, qmax, sigma, minWeight, name);

		rtf.fillPQGrid();

		rtf.findMaxima();
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
			drawTracks(hitscollection, rtf.getPQCollection(), truepq, best, i, "line");
		}

		multiplicity_h.Fill(rtf.getPQCollection().size());

		pqPoint bestpq = rtf.getBestPQ();
		if (bestpq.w < 0.) continue;
		double p_res_d = (bestpq.p - truepq.p)/pstep;
		double q_res_d = (bestpq.q - truepq.q)/qstep;
		p_res.Fill(p_res_d);
		q_res.Fill(q_res_d);
		p_q_res.Fill(p_res_d, q_res_d);

		p_gen_reco.Fill(truepq.p, bestpq.p);
		q_gen_reco.Fill(truepq.q, bestpq.q);

	}

	gStyle->SetOptStat(111111);

	DrawCanvas2(p_q);

	DrawCanvas(p_res);
	DrawCanvas(q_res);
	DrawCanvas2(p_q_res);

	DrawCanvas(m_h);
	DrawCanvas(b_h);
	DrawCanvas(multiplicity_h);

	DrawCanvas2(p_gen_reco);
	DrawCanvas2(q_gen_reco);

	return EXIT_SUCCESS;
}





