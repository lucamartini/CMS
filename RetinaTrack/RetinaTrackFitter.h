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

#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TF1.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TEllipse.h"

#include "CommonFuncs.h"

using namespace std;

class RetinaTrackFitter {
public:
	RetinaTrackFitter(vector <hit> hitCollection_, bool parabola = false, string name = "");
	virtual ~RetinaTrackFitter();

	void setHitCollection(vector <hit> hitCollection_) {hitCollection = hitCollection_;};
	void setR(double R) {R_gen_ = R;};
	void setPhi(double phi) {phi0_gen_ = phi;};
	void setA(double R, double phi) {a_gen_ = get_x_from_pol(R, phi);};
	void setB(double R, double phi) {b_gen_ = get_y_from_pol(R, phi);};

	void drawHits();
	void setConfHits();
	void drawHitsConf();
	void GetFromConfToCircle();
	void fillPQGrid();
	void drawPQGrid();
	void findMaxima();
	void printMaxima();
	void drawHitsConfRetina();
	void getCircles();
	void drawCircles();

	track getTrackParameters();

private:
//	double pi;
	inline double get_u(double x, double y) { return x / (x*x + y*y);};
	inline double get_v(double x, double y) { return y / (x*x + y*y);};
	inline double get_x(double u, double v) { return u / (u*u + v*v);};
	inline double get_y(double u, double v) { return v / (u*u + v*v);};

	inline double get_a(double p, double q) { return -1. * p / ( 2 * q);};
	inline double get_b(double p, double q) { return 1. / (2 * q);};
	inline double get_p(double a, double b) { return -1. * a / b;};
	inline double get_q(double a, double b) { return 1. / (2 * b) ;};

//	void setGeometry();
	void makeGrid();
	double getResponse(double p_temp, double q_temp);

	pqPoint findMaximumInterpolated(pqPoint_i point_i, double w);

//	vector < double > barrel_layer_r;

  vector <vector <double> > Grid;
  vector <hit> hitCollection;
  vector <hitConf> hitConfCollection;

  vector <pqPoint> pqCollection;
  vector <circlePoint> circleCollection;

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
  double sigma;

  double a_L1f;
  double b_L1f;
  double p;
  double q;
  TGraph * hitsConf_h;

  bool parabola_b;

  string name_;

};

#endif /* RETINATRACKFITTER_H_ */
