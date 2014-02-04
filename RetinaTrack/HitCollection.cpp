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

pqPoint HitCollection::drawHits(bool fit, bool draw) {
	unsigned int hits_n = hitCollection.size();
	TGraph hits_h(hits_n);
	hits_h.SetTitle("Hits");
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hits_h.SetPoint(hit_i, hitCollection[hit_i].x, hitCollection[hit_i].y);
	}
	TCanvas c("c", "c", 600, 600);
	hits_h.GetXaxis()->SetLimits(0., hits_h.GetXaxis()->GetXmax());
	hits_h.GetYaxis()->SetRangeUser(0., hits_h.GetYaxis()->GetXmax());
	hits_h.GetXaxis()->SetTitle("x[cm]");
	hits_h.GetYaxis()->SetTitle("y[cm]");
	hits_h.Draw("A*");
	pqPoint pqpoint;
	if (fit) {
		gStyle->SetOptFit(1111);
		hits_h.Fit("pol1", draw ? "" : "q");
		TF1* fun = hits_h.GetFunction("pol1");
		fun->SetParNames("q","p");
		c.Update();

		pqpoint.p = fun->GetParameter("p");
		pqpoint.q = fun->GetParameter("q");
		pqpoint.w = 1.;
	}
	else pqpoint.w = -1.;
	if (draw) c.Print(Form("figs/hits_%s.pdf", name.c_str()));

	return pqpoint;
}

void HitCollection::printHits(){
	unsigned int hits_n = hitCollection.size();
	for (unsigned int i = 0; i < hits_n; i++) {
		cout << "hit # " << i+1 << " / " << hits_n << " : x = " << hitCollection[i].x << "; y = " << hitCollection[i].y << endl;
	}
}
