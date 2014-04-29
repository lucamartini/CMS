/*
 * HitCollection.cpp
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 */

#include "HitCollection.h"

HitCollection::HitCollection(string name_) : name(name_) {
}

HitCollection::~HitCollection() {
}

HitCollection::HitCollection( const HitCollection& other ) {
	name = other.name;
	hitCollection = other.hitCollection;
}

pqPoint HitCollection::drawHits(bool fit, bool draw, bool rz) {
	unsigned int hits_n = hitCollection.size();
	TGraph hits_h(hits_n);
	hits_h.SetTitle("Hits");
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hits_h.SetPoint(hit_i, hitCollection[hit_i].x, hitCollection[hit_i].y);
	}
	TCanvas c("c", "c", 600, 600);
	hits_h.GetXaxis()->SetLimits(-1.1*zmax, 1.1*zmax);
	hits_h.GetYaxis()->SetRangeUser(0., 1.1*rmax);
	hits_h.GetXaxis()->SetTitle("x[cm]");
	hits_h.GetYaxis()->SetTitle("y[cm]");
	hits_h.Draw("A*");
	pqPoint pqpoint;
	if (fit) {
		gStyle->SetOptFit(10001);
		string pol1("[1]*x + [0]");
		if (rz) pol1 = "1./[1]*x - [0]/[1]";
		TF1 * fitfun = new TF1("fitfun", pol1.c_str());
		fitfun->SetParNames("q","p");
		fitfun->SetParameters(1., 1.);
		hits_h.Fit(fitfun, draw ? "" : "q");
		c.Update();

		pqpoint.p = fitfun->GetParameter("p");
//		pqpoint.p = atan(fun->GetParameter("p"));
		pqpoint.q = fitfun->GetParameter("q");
		pqpoint.w = 1.;
	}
	else pqpoint.w = -1.;
	TPaveStats *st = (TPaveStats*)hits_h.FindObject("stats");
	st->SetY1NDC(0.72);
	st->SetY2NDC(0.87);
	st->SetX1NDC(0.13);
	st->SetX2NDC(0.28);

	if (draw) c.Print(Form("figs/hits_%s.pdf", name.c_str()));

	return pqpoint;
}

void HitCollection::printHits(){
	unsigned int hits_n = hitCollection.size();
	for (unsigned int i = 0; i < hits_n; i++) {
		cout << "hit # " << i+1 << " / " << hits_n << " : x = " << hitCollection[i].x << "; y = " << hitCollection[i].y << endl;
	}
}

double HitCollection::highestX(){
	double maxX = 0.;
	unsigned int hits_n = hitCollection.size();
	for (unsigned int i = 0; i < hits_n; i++) {
		if (hitCollection.at(i).x > maxX) {
			maxX = hitCollection.at(i).x;
		}
	}
	return maxX;
}

double HitCollection::highestY(){
	double maxY = 0.;
	unsigned int hits_n = hitCollection.size();
	for (unsigned int i = 0; i < hits_n; i++) {
		if (hitCollection.at(i).y > maxY) {
			maxY = hitCollection.at(i).y;
		}
	}
	return maxY;
}

double HitCollection::lowestX(){
	double minX = 9999999.;
	unsigned int hits_n = hitCollection.size();
	for (unsigned int i = 0; i < hits_n; i++) {
		if (hitCollection.at(i).x < minX) {
			minX = hitCollection.at(i).x;
		}
	}
	return minX;
}

double HitCollection::lowestY(){
	double minY = 0.;
	unsigned int hits_n = hitCollection.size();
	for (unsigned int i = 0; i < hits_n; i++) {
		if (hitCollection.at(i).y < minY) {
			minY = hitCollection.at(i).y;
		}
	}
	return minY;
}

unsigned int HitCollection::getClosestHit(){
	unsigned int hit = 99999;
	double minY = 99999.;
	unsigned int hits_n = hitCollection.size();
	for (unsigned int i = 0; i < hits_n; i++) {
		if (hitCollection.at(i).y < minY) {
			minY = hitCollection.at(i).y;
			hit = i;
		}
	}
	return hit;
}

unsigned int HitCollection::getFarthestHit(){
	unsigned int hit = 99999;
	double maxY = 0.;
	unsigned int hits_n = hitCollection.size();
	for (unsigned int i = 0; i < hits_n; i++) {
		if (hitCollection.at(i).y > maxY) {
			maxY = hitCollection.at(i).y;
			hit = i;
		}
	}
	return hit;
}
