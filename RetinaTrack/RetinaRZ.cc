/*
 * RetinaRZ.cc
 *
 *  Created on: 03/apr/2014
 *      Author: lucamartini
 */

/*
 * RetinaSimple.cc
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 *
 *      Retina in the rz plane, that is straight lines and parallel detectors
 *
 */

#include <cstdlib>
#include <fstream>

#include "LayerGeometry.h"
#include "HitCollection.h"
#include "ConformalTransf.h"
#include "RetinaTrackFitter.h"

#include "print.h"


int main(int argc, char* argv[]) {

	int events = 1;
	// 0: y = mx + b
	// 1: Ristori
	// 2: polar
	int para = 1;
	bool draw = false;
	bool twosteps = false;
	unsigned int xminus_bins(31);
	unsigned int xplus_bins(31);
	for (int i = 1; i < argc; i++) {
	  if (!strcmp(argv[i],"-d")) {
	  	draw = true;
	    cout << "drawing plots for each event" << endl;
	  }
	  if (!strcmp(argv[i],"-n")) {
	  	events = atoi(argv[i+1]);
	  }
	  if (!strcmp(argv[i],"-p")) {
	  	para = atoi(argv[i+1]);
	  }
	  if (!strcmp(argv[i],"-xm")) {
	  	xminus_bins = atoi(argv[i+1]);
	  	cout << "x_minus bins = " << xminus_bins << endl;
	  }
	  if (!strcmp(argv[i],"-xp")) {
	  	xplus_bins = atoi(argv[i+1]);
	  	cout << "x_plus bins = " << xplus_bins << endl;
	  }
	  if (!strcmp(argv[i],"-2")) {
	  	twosteps = true;
	  	cout << "make two steps" << endl;
	  }
	  if (!strcmp(argv[i],"-h")) {
	  	cout << "-h \t print this help and exit" << endl <<
	  			"-d \t drawing plots for each event \t\t default = " << draw << endl <<
	  			"-2 \t two steps \t\t default = " << twosteps << endl <<
	  			"-xp # \t number of bins in x_plus \t\t default = " << xplus_bins << endl <<
	  			"-xm # \t number of bins in x_minus \t\t default = " << xminus_bins << endl <<
	  			"-n # \t set number of events \t\t\t default = " << events << endl <<
	  			"-p # \t set parametrization of the grid: \t default = " << para << endl <<
	  			"\t\t 0 : y = mx + b" << endl <<
	  			"\t\t 1 : Ristori x_+ x_-" << endl <<
	  			"\t\t 2 : polar" << endl;
	  	return EXIT_SUCCESS;
	  }
	}
	if (para != 1) {
		cout << "specialized to Ristori variables, sorry..." << endl;
		return EXIT_FAILURE;
	}

	cout << "events = " << events << endl;

	string s_name (Form("lineRZ_%ix%i", xminus_bins, xplus_bins));
	if (twosteps) s_name += "_2steps";

	double xminus_min(-20.);
	double xminus_max(80.);
	double xminus_step = (xminus_max-xminus_min)/(double)xminus_bins;
	double xplus_min(0.);
	double xplus_max(100.);
	double xplus_step = (xplus_max-xplus_min)/(double)xplus_bins;

	double sigma = 6;
	sigma = sqrt(xminus_step*xplus_step);
	sigma = 1.;

	double minWeight = 3.;
	cout << "xminus step = " << xminus_step << " cm; xplus step = " << xplus_step << " cm; sigma = " << sigma << endl;

	TH1D e_h(Form("e_h_%s", s_name.c_str()), "gen #eta;#eta", 100, -2.5, 2.5);
	TH1D t_h(Form("t_h_%s", s_name.c_str()), "gen #theta;#theta[rad]", 100, 0, 2*TMath::Pi());
	TH1D z_h(Form("z_h_%s", s_name.c_str()), "gen z;z[cm]", 50, -20., 20.);
	TH1D m_h(Form("m_h_%s", s_name.c_str()), "gen m = tg(#theta)", 100, -10., 10.);
	TH1D xp_h(Form("xp_h_%s", s_name.c_str()), "gen x_{+}", 100, xplus_min, xplus_max);
	TH1D xm_h(Form("xm_h_%s", s_name.c_str()), "gen x_{-}", 100, xminus_min, xminus_max);

	TH1I multiplicity_h(Form("multiplicity_h_%s", s_name.c_str()), "multiplicity_h", 10, -0.5, 9.5);
	
	double z_limit = 4.;
	double eta_limit = 0.04;
	if (twosteps) {
		z_limit = 2.;
		eta_limit = 0.02;
	}
	TH1D z_res(Form("z_res_%s", s_name.c_str()), "z resolution;#Delta z[cm]", 100, -z_limit, z_limit);
	TH1D eta_res(Form("eta_res_%s", s_name.c_str()), "#eta resolution;#Delta#eta", 100, -eta_limit, eta_limit);
	TH2D z_eta_res(Form("z_eta_res_%s", s_name.c_str()), "z #eta resolutions;#Delta#eta;#Delta z[cm]", 100, -eta_limit, eta_limit, 100, -z_limit, z_limit);
	TH2D z_gen_reco(Form("z_gen_reco_%s", s_name.c_str()), "z gen / reco;gen z;reco z", 100, -10., 10., 100, -10., 10.);
	TH2D eta_gen_reco(Form("eta_gen_reco_%s", s_name.c_str()), "#eta gen / reco;gen #eta;reco #eta", 100, -2.4, 2.4, 100, -2.4, 2.4);
	
	TH2D xminus_gen_reco(Form("xminus_gen_reco_%s", s_name.c_str()), "x_{-} gen / reco;gen x_{-};reco x_{-}", xminus_bins*10, xminus_min, xminus_max, xminus_bins*10, xminus_min, xminus_max);
	TH2D xplus_gen_reco(Form("xplus_gen_reco_%s", s_name.c_str()), "x_{+} gen / reco;gen x_{+};reco x_{+}", xplus_bins*10, xplus_min, xplus_max, xplus_bins*10, xplus_min, xplus_max);
		
	TH1D xplus_res_step(Form("xplus_res_step_%s", s_name.c_str()), "x_{+} resolution / step;[x_{+}]", 100, -1., 1.);
	TH1D xminus_res_step(Form("xminus_res_step_%s", s_name.c_str()), "x_{-} resolution / step;[x_{-}]", 100, -1., 1.);
	TH2D xplus_xminus_res_step(Form("xplus_xminus_res_%s", s_name.c_str()), "x_{-} x_{+} resolutions / steps;[x_{-}];[x_{-}]", 50, -1., 1., 50, -1., 1.);
	

	LayerGeometry LG;
	TRandom3 rand;
	cout << "seed: " << rand.GetSeed() << endl; //4357
	double theta_max = TMath::PiOver2(); // 1.571
	double theta_min = atan(1100./900.); // 0.885

	double m_min = tan(theta_min);
	double m_max = tan(theta_max);

	double eta_max = -log(tan(theta_min/2.)); // 0.747
	double eta_min = -log(tan(theta_max/2.)); // 0.

	for (int i = 0; i < events; i++) {
		if (i%1000 == 0) cout << i << " / " << events << endl;
		Double_t eta_rnd = rand.Uniform(-0.5, 1.5);
		Double_t theta_rnd = 2*atan(exp(-eta_rnd));
		Double_t z_rnd = 0;
		z_rnd = rand.Gaus(0., 5.); // sigma_z = 7.5 cm

		string name(Form("%s_%d", s_name.c_str(), i));
		HitsGenerator HG(LG, plain, name); //plain
		HG.addLineRZ(theta_rnd, z_rnd); // p = 1; q = 0
		HG.cleanHitsXY(0., 999999., 0., LG.get_max_z());

		HitCollection hitscollection = HG.getHitCollection();
		if (hitscollection.size() < 6) {
			i--;
			continue;
		}

		e_h.Fill(eta_rnd);
		t_h.Fill(theta_rnd);
		z_h.Fill(z_rnd);
		m_h.Fill(tan(theta_rnd));

		double x2 = hitscollection.getHit(hitscollection.getClosestHit())->x;
		double x1 = hitscollection.getHit(hitscollection.getFarthestHit())->x;
		double x_plus = (x1 + x2)/2.;
		double x_minus = (x1 - x2)/2.;

		xp_h.Fill(x_plus);
		xm_h.Fill(x_minus);

		pqPoint truepq = hitscollection.drawHits(true, draw, false);

		// first iteration to find point

		xminus_min = -20.;
		xminus_max = 80.;
		xplus_min = 0.;
		xplus_max = 100.;
		xminus_step = (xminus_max-xminus_min)/(double)xminus_bins;
		xplus_step = (xplus_max-xplus_min)/(double)xplus_bins;

		if (twosteps) {

			RetinaTrackFitter rtf0(hitscollection, xplus_bins, xminus_bins, xplus_min, xplus_max, xminus_min, xminus_max, sigma, minWeight, name, para);

			rtf0.fillPQGrid();

			rtf0.findMaxima();

			pqPoint bestpq0 = rtf0.getBestPQ();
			if (bestpq0.w < 0.) continue;

		// zooming in 20%

//		xminus_min = 0.8*x_minus;
//		xminus_max = 1.2*x_minus;
//		xplus_min = 0.8*x_plus;
//		xplus_max = 1.2*x_plus;

			xminus_min = 0.8*bestpq0.q;
			xminus_max = 1.2*bestpq0.q;
			xplus_min = 0.8*bestpq0.p;
			xplus_max = 1.2*bestpq0.p;
			xminus_step = (xminus_max-xminus_min)/(double)xminus_bins;
			xplus_step = (xplus_max-xplus_min)/(double)xplus_bins;
		}

		RetinaTrackFitter rtf(hitscollection, xplus_bins, xminus_bins, xplus_min, xplus_max, xminus_min, xminus_max, sigma, minWeight, name, para);

		rtf.fillPQGrid();

		rtf.findMaxima();

		multiplicity_h.Fill(rtf.getPQCollection().size());

		pqPoint bestpq = rtf.getBestPQ();
		if (bestpq.w < 0.) continue;

		if (draw) {
			cout << "x_plus = " << x_plus << "   x_minus = " << x_minus << endl;
			rtf.printMaxima();

			rtf.drawPQGrid();

			// get best track. Best = highest w
			double w = -1.;
			unsigned int best;
			vector <pqPoint> pqCollection = rtf.getPQCollection();
			for (unsigned int i = 0; i < pqCollection.size(); i++) {
				if (pqCollection[i].w > w) {
					w = pqCollection[i].w;
					best = i;
				}
			}
			drawTracks(hitscollection, rtf.getPQCollection(), truepq, best, i, s_name, &LG, para);
		}

		double best_x1 = bestpq.q + bestpq.p;
		double best_x2 = bestpq.p - bestpq.q;

		xminus_gen_reco.Fill(x_minus, bestpq.q);
		xplus_gen_reco.Fill(x_plus, bestpq.p);
		double delta_x_minus = bestpq.q - x_minus;
		double delta_x_plus = bestpq.p - x_plus;
		xminus_res_step.Fill(delta_x_minus/xminus_step);
		xplus_res_step.Fill(delta_x_plus/xplus_step);

		double m, b;
		vector <line> layers = LG.get_barrel_layers_plain();
		double y2 = layers.at(0).q;
		double y1 = layers.at(layers.size()-1).q;

		m = (y2 - y1) / (best_x2 - best_x1);
		b = y2 - m*best_x2;

		double best_dz = -b/m;
		double best_eta = -log(tan(atan(m)/2.));
		double z_res_d = best_dz - z_rnd;
		double eta_res_d = best_eta - eta_rnd;
		z_res.Fill(z_res_d);
		eta_res.Fill(eta_res_d);
		z_eta_res.Fill(eta_res_d, z_res_d);
		z_gen_reco.Fill(z_rnd, best_dz);
		eta_gen_reco.Fill(eta_rnd, best_eta);
	}

	gStyle->SetOptStat(111111);

	DrawCanvas(z_res);
	DrawCanvas(eta_res);
	DrawCanvas2(z_eta_res);
	DrawCanvas2(z_gen_reco);
	DrawCanvas2(eta_gen_reco);

	DrawCanvas(t_h);
	DrawCanvas(z_h);
	DrawCanvas(m_h);
	DrawCanvas(e_h);
	DrawCanvas(xp_h);
	DrawCanvas(xm_h);
	DrawCanvas(multiplicity_h);

	DrawCanvas2(xminus_gen_reco);
	DrawCanvas2(xplus_gen_reco);
	DrawCanvas(xminus_res_step);
	DrawCanvas(xplus_res_step);

	// z resolution
	fstream fs;
	fs.open("data/z_eta_resolutions.csv", std::fstream::in | std::fstream::out | std::fstream::app);
	fs << xminus_bins << "," << xplus_bins << "," << z_res.GetRMS() << "," << z_res.GetRMSError() << "," << eta_res.GetRMS() << "," << eta_res.GetRMSError() << endl;
	fs.close();

	return EXIT_SUCCESS;
}



