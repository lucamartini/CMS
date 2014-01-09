/*
 * makeRetina.cc
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 */


#include "RetinaTrackFitter.h"

int main(int argc, char* argv[]) {

	double a[4] = { -0.1, -0.1, -0.1, -0.1};
	double b[4] = { 15., 20., 30., 40.};

	for (int i = 0; i < 4; i++) {
		RetinaTrackFitter rtf(a[i], b[i], 0, Form("%d", i));
		unsigned int hits = rtf.setHits();
		cout << "hits = " << hits << endl;
		hits = rtf.cleanHits(0., 5., 0., 5.);
		if (hits == 0) continue;
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

	return EXIT_SUCCESS;
}


