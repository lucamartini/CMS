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
	// sqrt(aa + bb) = R = pt / (0.3 * B) = 1.8 m = 180 cm

	double R = 180.;

	vector <double> phi;
	double pi = acos(-1);
	for (int i = 0; i < 5; i++) phi.push_back(pi/10. * i + pi/2.);


	vector <double> a;
	vector <double> b;

//	a.push_back(0.1);
//	a.push_back(1.);
//	a.push_back(10.);
//	a.push_back(50.);
//	a.push_back(100.);
//	a.push_back(0.1);
//	a.push_back(0.1);
//	a.push_back(0.1);

//	b.push_back(15.);
//	b.push_back(20.);
//	b.push_back(30.);
//	b.push_back(40.);
//	b.push_back(15.);
//	b.push_back(20.);
//	b.push_back(30.);
//	b.push_back(40.);

	int size = phi.size();
	cout << "(a, b) = ";
	for (int i = 0; i < size; i++) {
		a.push_back(R * cos(phi[i]));
		double b_i = sqrt(R*R - a[i]*a[i]);
		b.push_back(b_i);
		cout << "(" << a[i] << ", " << b_i << "), ";
	}
	cout << endl;

	for (int i = 0; i < size; i++) {
		RetinaTrackFitter rtf(a[i], b[i], false, Form("%d", i));
		unsigned int hits = rtf.setHits();
//		cout << "hits = " << hits << endl;
		hits = rtf.cleanHits(0., 150., 0., 150.);
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

	return EXIT_SUCCESS;
}


