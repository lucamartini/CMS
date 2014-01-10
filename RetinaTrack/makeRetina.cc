/*
 * makeRetina.cc
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 *
 *
 */


#include "RetinaTrackFitter.h"

int main(int argc, char* argv[]) {

	// pt = 2 GeV
	// B = 3.8 T
	// sqrt(a^2 + b^2) = R = pt / (0.3 * B) = 1.8 m = 180 cm
	// pt = 2 --> R = 1.8 m = 180 cm
	// pt = 3 --> R = 2.6 m = 260 cm
	// r = 2R cos (phi - phi0) origin lies on circle

	double pi = acos(-1);

  vector <double> phi0;
  for (int i = 1; i < 5; i++) {
  	double phi_i = pi/10. * i + pi/2. + 0.05;
  	phi0.push_back(phi_i);
  }
  for (int i = 1; i < 5; i++) {
  	double phi_i = pi/10. * i + 3*pi/2. + 0.05;
  	phi0.push_back(phi_i);
  }

  vector <double> R;
  R.push_back(180.);
  R.push_back(260.);

	int size_phi = phi0.size();
	int size_R = R.size();

	for (int i = 0; i < size_phi; i++) {
		for (int j = 0; j < size_R; j++) {
			cout << "r_gen, phi_gen = " << R[j] << ", " << phi0[i] << endl;
			RetinaTrackFitter rtf(R[j], phi0[i], false, Form("%d_%d", i, j));
			unsigned int hits = rtf.setHits();
//		cout << "hits = " << hits << endl;
//		hits = rtf.cleanHits(0., 150., -150., 150.);
			if (hits == 0) continue;
			cout << "hits = " << hits << endl;
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


