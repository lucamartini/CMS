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
#include "TRandom3.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TF1.h"
#include "TH2D.h"
#include "TStyle.h"

using namespace std;

struct hit {
	double x;
	double y;
};

struct hitConf {
	double u;
	double v;
};

struct pqPoint_i {
	int p;
	int q;
};

struct pqPoint {
	double p;
	double q;
};

struct circlePoint {
	double a;
	double b;
	double R;
};

class RetinaTrackFitter {
public:
	RetinaTrackFitter(double a_gen, double b_gen, bool parabola = false, string name = "");
	virtual ~RetinaTrackFitter();

	unsigned int setHits();
	unsigned int cleanHits(double ymin, double ymax, double xmin, double xmax);
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


private:

	inline double get_u(double x, double y) { return x / (x*x + y*y);};
	inline double get_v(double x, double y) { return y / (x*x + y*y);};
	inline double get_x(double u, double v) { return u / (u*u + v*v);};
	inline double get_y(double u, double v) { return v / (u*u + v*v);};

	inline double get_a(double p, double q) { return -1. * p / ( 2 * q);};
	inline double get_b(double p, double q) { return 1. / (2 * q);};
	inline double get_p(double a, double b) { return -1. * a / b;};
	inline double get_q(double a, double b) { return 1. / (2 * b) ;};

	void makeGrid();
	double getResponse(double p_temp, double q_temp);
	pqPoint findMaximumInterpolated(pqPoint_i point_i);

  vector <vector <double> > Grid;
  vector <hit> HitCollection;
  vector <hitConf> HitConfCollection;

  vector <pqPoint> pqCollection;
  vector <circlePoint> circleCollection;

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
