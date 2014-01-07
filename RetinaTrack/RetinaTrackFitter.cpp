/*
 * RetinaTrackFitter.cpp
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 */

#include "RetinaTrackFitter.h"

RetinaTrackFitter::RetinaTrackFitter() {
	A = new array_type(boost::extents[10][10]);
	a_gen = 5;
	b_gen = 1;
	hitsConf_h = new TGraph(0);
	pq = new TH2D("pq", "pq plane;p;q", 10, 0., 1., 10, -5., 0.);
}

RetinaTrackFitter::~RetinaTrackFitter() {
}

void RetinaTrackFitter::setHits() {
	TRandom3 rand;

	for (double i = -10; i <= 10; i = i + 0.25) {
		double spread = rand.Gaus(0, 0.01); // spread = 0;
		double y = i;
		double x = 0;
//	x = (y - 3) / 2 + spread;  // line
		double R_sq = a_gen*a_gen + b_gen*b_gen;
		if (R_sq < (y-b_gen)*(y-b_gen)) {cout << R_sq << "   " << (y-b_gen)*(y-b_gen) << endl;   continue;}
		double known_term = R_sq - (y-b_gen)*(y-b_gen);
		x = a_gen + sqrt(known_term) + spread; // right quadrant
		hit hit_i;
		hit_i.x = x;
		hit_i.y = y;
		HitCollection.push_back(hit_i);
		x = a_gen - sqrt(known_term) + spread; // left quadrant
		hit_i.x = x;
		HitCollection.push_back(hit_i);
	}
}

void RetinaTrackFitter::cleanHits(double ymin, double ymax, double xmin, double xmax) {
	vector <hit> HitCollectionNew;
	unsigned int hits_n = HitCollection.size();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		if (HitCollection[hit_i].y > ymin && HitCollection[hit_i].y <= ymax
				&& HitCollection[hit_i].x > xmin && HitCollection[hit_i].x <= xmax) {
			HitCollectionNew.push_back(HitCollection[hit_i]);
		}
	}
	HitCollection = HitCollectionNew;
}

void RetinaTrackFitter::drawHits() {
	unsigned int hits_n = HitCollection.size();
	TGraph hits_h(hits_n);
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hits_h.SetPoint(hit_i, HitCollection[hit_i].x, HitCollection[hit_i].y);
	}
	TCanvas c("c", "c", 600, 600);
	hits_h.GetXaxis()->SetLimits(-10.,10.);
	hits_h.GetYaxis()->SetRangeUser(-10.,10.);
	hits_h.Draw("A*");
	c.Update();
	c.Print("hits.pdf");
}

void RetinaTrackFitter::setConfHits() {
	unsigned int hits_n = HitCollection.size();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		double x =  HitCollection[hit_i].x;
		double y =  HitCollection[hit_i].y;
		double u = x / (x*x + y*y);
		double v = y / (x*x + y*y);
		hitConf hitConf_i;
		hitConf_i.u = u;
		hitConf_i.v = v;
		HitConfCollection.push_back(hitConf_i);
	}
}

void RetinaTrackFitter::drawHitsConf() {
	unsigned int hits_n = HitConfCollection.size();
	hitsConf_h->Set(hits_n);
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hitsConf_h->SetPoint(hit_i, HitConfCollection[hit_i].u, HitConfCollection[hit_i].v);
	}
	TCanvas c("c", "c", 600, 600);
	hitsConf_h->Draw("A*");
	hitsConf_h->Fit("pol1");
	c.Print("hitsConf.pdf");

}

void RetinaTrackFitter::GetFromConfToCircle() {
	TF1* fun = hitsConf_h->GetFunction("pol1");
	fun->SetParNames("p","q");
	p = fun->GetParameter(0);
	q = fun->GetParameter(1);
	b_L1f = 1. / (2 * p);
	a_L1f = -1. * b_L1f * q;
	cout << "a = " << a_L1f << "  b = " << b_L1f << endl;

}
