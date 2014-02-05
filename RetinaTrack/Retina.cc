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

#include "LayerGeometry.h"
#include "HitCollection.h"
#include "ConformalTransf.h"
#include "RetinaTrackFitter.h"

void drawCircles(HitCollection hitCollection, vector <circlePoint> circleCollection, double R_gen_) {

	unsigned int hits_n = hitCollection.size();
	TGraph hits_h(hits_n);
	double pt = 0.3 * 3.8 * R_gen_ / 100.;
	hits_h.SetTitle(Form("Hits and retina circles, p_{T} = %.f GeV", pt));
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hits_h.SetPoint(hit_i, hitcollref->at(hit_i).x, hitcollref->at(hit_i).y);
	}
	TCanvas c("c", "c", 600, 600);
	hits_h.GetXaxis()->SetLimits(0., hits_h.GetXaxis()->GetXmax());
	hits_h.GetYaxis()->SetRangeUser(0., hits_h.GetYaxis()->GetXmax());
	hits_h.GetXaxis()->SetTitle("x[cm]");
	hits_h.GetYaxis()->SetTitle("y[cm]");
	hits_h.Draw("A*");

	unsigned int circles_size = circleCollection.size();
	vector <TEllipse*> circle_f;
	circle_f.resize(circles_size);
	for (unsigned int circle_i = 0; circle_i < circles_size; circle_i++) {
		double a = circleCollection[circle_i].a;
		double b = circleCollection[circle_i].b;
		double R = sqrt(a*a + b*b);
		circle_f[circle_i] = new TEllipse(a, b, R);
		circle_f[circle_i]->SetNoEdges();
		circle_f[circle_i]->SetLineColor(kBlue + circle_i);
		circle_f[circle_i]->SetFillStyle(0);
		circle_f[circle_i]->Draw("same");
	}

	c.Print(Form("figs/hitsRetinaCircles.pdf"));
}


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

	unsigned int pbins(20);
	unsigned int qbins(20);
  double pmin(0.);
  double pmax(0.5);
  double qmin(0.0005);
  double qmax(0.002);
	double qstep = (qmax-qmin)/(double)qbins;
	double pstep = (pmax-pmin)/(double)pbins;
	double sigma = 0.0003;
	double minWeight = 0.5;
	cout << "qstep = " << qstep << " /cm; pstep = " << pstep << endl;
	TH1D p_res("circle_p_res_circle", "p resolution / p step;[p]", 100, -1, 1);
	TH1D q_res("circle_q_res_circle", "q resolution / q step;[q]", 100, -1, 1);
	TH2D p_q_res("circle_p_q_res_circle", "reduced p q resolution;[p];[q]", 20, -1., 1., 20, -1., 1.);

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

		RetinaTrackFitter rtf(confhitscollection, pbins, qbins, pmin, pmax, qmin, qmax, sigma, minWeight, name);

		rtf.fillPQGrid();

		rtf.findMaxima();

		vector <circlePoint> cp =	rtf.getCircles();

		pqPoint truepq = confhitscollection.drawHits(true, draw);
		if (draw) {
			rtf.printMaxima();

			rtf.drawPQGrid();
			rtf.drawTracks();

			CF.setConfHitCollection(confhitscollection);
			CF.from_conf_to_norm(true);
			HitCollection normhitscollection = CF.getNormHitCollection();
			normhitscollection.drawHits(false);

			drawCircles(normhitscollection, cp , r_rnd);
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


