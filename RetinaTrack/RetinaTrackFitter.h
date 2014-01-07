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
#include "boost/multi_array.hpp"
#include "TRandom3.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TF1.h"

using namespace std;

typedef boost::multi_array<double, 2> array_type;
typedef array_type::index indexe;

struct hit {
	double x;
	double y;
};

struct hitConf {
	double u;
	double v;
};

class RetinaTrackFitter {
public:
	RetinaTrackFitter();
	virtual ~RetinaTrackFitter();

	void setHits();
	void cleanHits(double ymin, double ymax, double xmin, double xmax);
	void drawHits();
	void setConfHits();
	void drawHitsConf();
	void GetFromConfToCircle();


private:
  array_type * A;

  vector <hit> HitCollection;
  vector <hitConf> HitConfCollection;

  double a_gen;
  double b_gen;
  double a_L1f;
  double b_L1f;
  double p;
  double q;
  TGraph * hitsConf_h;
  TH2D * pq;

};

#endif /* RETINATRACKFITTER_H_ */
