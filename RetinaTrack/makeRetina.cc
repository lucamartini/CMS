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

	TH1D pt_res("pt_res", "p_{T} resolution;[GeV]", 100, -10., 10.);
	TH1D phi_res("phi_res", "#phi resolution;[rad]", 100, -.2, .2);

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

			RetinaTrackFitter rtf(hitscollection, false, Form("%d_%d", i, j));
			rtf.setR(R[j]);
			rtf.setPhi(phi0[i]);
			rtf.setA(R[j], phi0[i]);
			rtf.setB(R[j], phi0[i]);

			rtf.drawHits();

			rtf.setConfHits();
			rtf.drawHitsConf();
			rtf.GetFromConfToCircle();

			rtf.fillPQGrid();
			rtf.drawPQGrid();

			rtf.findMaxima();
			rtf.printMaxima();
			rtf.drawHitsConfRetina();

			rtf.getCircles();
			rtf.drawCircles();

			track track_ij = rtf.getTrackParameters();
			double pt_reco = track_ij.pt;
			if (pt_reco < 0.) continue;
			double phi_reco = track_ij.phi;

			pt_res.Fill(pt_reco - R[j]/100.*0.3*3.8);
			phi_res.Fill(phi_reco - phi0[i]);
		}
	}

	TCanvas pt_c("pt_c", "pt_c", 600, 600);
	pt_res.Draw();
	pt_c.Print("figs/pt_res.pdf");

	TCanvas phi_c("phi_c", "phi_c", 600, 600);
	phi_res.Draw();
	phi_c.Print("figs/phi_res.pdf");

	return EXIT_SUCCESS;
}


