/*
 * RetinaTrackFitter.h
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 */

#ifndef RETINATRACKFITTER_H_
#define RETINATRACKFITTER_H_

#include <vector>
#include <iostream>
#include <math.h>

#include "TLatex.h"

#include "HitsGenerator.h"

using namespace std;

class RetinaTrackFitter {
public:
	RetinaTrackFitter(HitCollection hitCollection_, unsigned int pbins_, unsigned int qbins_, double pmin_, double pmax_, double qmin_, double qmax_, double sigma_, double minWeight_, string name = "", int para_ = 0);
	virtual ~RetinaTrackFitter();

	void setHitCollection(HitCollection hitCollection_) {hitCollection = hitCollection_;};
	void setR(double R) {R_gen_ = R;};
	void setPhi(double phi) {phi0_gen_ = phi;};
	void setA(double R, double phi) {a_gen_ = get_x_from_pol(R, phi);};
	void setB(double R, double phi) {b_gen_ = get_y_from_pol(R, phi);};

	void fillPQGrid();
	void drawPQGrid();
	void findMaxima();
	void printMaxima();
	vector <pqPoint> getPQCollection() {return pqCollection;};
	vector <circle> getCircles();

	track getBestTrack();
	circle getBestCircle();
	pqPoint getBestPQ();

private:

	inline double get_a(double p, double q) { return -1. * p / ( 2 * q);};
	inline double get_b(double p, double q) { return 1. / (2 * q);};
	inline double get_p(double a, double b) { return -1. * a / b;};
	inline double get_q(double a, double b) { return 1. / (2 * b) ;};

	void makeGrid();
	double getResponse(double p_temp, double q_temp);
	double getResponseRistori(double x_plus, double x_minus);
	double getResponsePol(double gamma_temp, double b_temp);

	pqPoint findMaximumInterpolated(pqPoint_i point_i, double w);

  vector <vector <double> > Grid;
  HitCollection hitCollection;

  vector <pqPoint> pqCollection;
  vector <circle> circleCollection;

  double R_gen_;
  double phi0_gen_;
  double a_gen_;
  double b_gen_;
  unsigned int pbins;
  unsigned int qbins;
  double pmin;
  double pmax;
  double qmin;
  double qmax;
  double pbinsize;
  double qbinsize;
  double sigma;
  double minWeight;

  string name_;
  int para;
  vector <line> layers;

};

#endif /* RETINATRACKFITTER_H_ */
