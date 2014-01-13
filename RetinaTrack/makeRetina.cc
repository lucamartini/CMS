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
	// pt = 20 GeV --> R = 17.5 m = 1750 cm
	// pt = 100 GeV --> R = 87.7 m = 8770 cm
	// r = 2R cos (phi - phi0) origin lies on circle

  vector <double> phi0;
//  for (int i = 1; i < 5; i++) {
//  	double phi_i = pi/10. * i + pi/2. + 0.05;
//  	phi0.push_back(phi_i);
//  }
//  for (int i = 1; i < 5; i++) {
//  	double phi_i = pi/10. * i + 3*pi/2. + 0.05;
//  	phi0.push_back(phi_i);
//  }
  double phi_i = 0 + pi/24.;
  while (phi_i < 2*pi) {
  	phi0.push_back(phi_i);
  	phi_i += pi/6.;
  }

  vector <double> R;
  R.push_back(180.);
  R.push_back(260.);
  R.push_back(1750.);
  R.push_back(8770.);

	int size_phi = phi0.size();
	int size_R = R.size();

	for (int i = 0; i < size_phi; i++) {
		for (int j = 0; j < size_R; j++) {
//			if (i != 1 || j != 0) continue;
			cout << "r_gen, phi_gen = " << R[j] << ", " << phi0[i] << endl;
			HitsGenerator HG;
			HG.addCircle(R[j], phi0[i], true);
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
		}
	}
	return EXIT_SUCCESS;
}


