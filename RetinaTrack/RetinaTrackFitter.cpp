/*
 * RetinaTrackFitter.cpp
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 */

#include "RetinaTrackFitter.h"

RetinaTrackFitter::RetinaTrackFitter(HitCollection hitCollection_, unsigned int pbins_, unsigned int qbins_, double pmin_, double pmax_, double qmin_, double qmax_, double sigma_, bool parabola, string name) :
  hitCollection(hitCollection_),
	pbins(pbins_),
  qbins(qbins_),
  pmin(pmin_),
  pmax(pmax_),
  qmin(qmin_),
  qmax(qmax_),
  sigma(sigma_),
  parabola_b(parabola),
  name_(name)
{
//	cout << "CIRCLE GENERATED: a = " << a_gen_ << "  b = " << b_gen_ << endl;
	makeGrid();
}

RetinaTrackFitter::~RetinaTrackFitter() {
}

void RetinaTrackFitter::makeGrid() {
	Grid.resize(pbins);
	for (unsigned int i = 0; i < pbins; i++) {
		Grid[i].resize(qbins);
	}
}

void RetinaTrackFitter::fillPQGrid() {
	for (unsigned int i = 0; i < pbins; i++) {
		for (unsigned int j = 0; j < qbins; j++) {
			double p_value = pmin + (pmax-pmin)/pbins * i;
			double q_value = qmin + (qmax-qmin)/qbins * j;
			Grid[i][j] = getResponse(p_value, q_value);
//			cout << i << " " << j << " p = " << p_value << " q = " << q_value << " w = " << Grid[i][j] << endl;
		}
	}
}

double RetinaTrackFitter::getResponse(double p_temp, double q_temp) {
//	cout << p_temp << " " << q_temp << endl;
	double Rij = 0.;
	unsigned int hits_tot = hitCollection.size();
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	for (unsigned int kr = 0; kr < hits_tot; kr++) {
		double y_k = (hitcollref->at(kr).y - q_temp) / p_temp;
		double Sijkr = hitcollref->at(kr).x - y_k;
		double term = exp(Sijkr*Sijkr/(-2.*sigma*sigma));
//		cout << y_k << "  " << Sijkr << "  " << term << endl;
		Rij += term;
//		cout << " y = px + q =>" << hitcollref->at(kr).y << " = " << p_temp << " * " << y_k << " + " << q_temp;
//		cout <<  "   u = " << hitcollref->at(kr).x  <<  endl;
	}
	Rij /= hits_tot;
	if (Rij < 1e-6) return 1e-6;
	else return Rij;
}

void RetinaTrackFitter::drawPQGrid() {
//  TH2D pq_h("pq_h", "pq grid;p;q", pbins-1, pmin - (pmax-pmin)/pbins/2, pmax - (pmax-pmin)/pbins*3/2, qbins-1, qmin - (qmax-qmin)/qbins/2, qmax - (qmax-qmin)/qbins*3/2);
  TH2D pq_h("pq_h", "pq grid;p;q[cm]", pbins-1, pmin - (pmax-pmin)/pbins/2, pmax - (pmax-pmin)/pbins*3/2, qbins-1, qmin - (qmax-qmin)/qbins/2, qmax - (qmax-qmin)/qbins*3/2);
	for (unsigned int i = 0; i < pbins; i++) {
		for (unsigned int j = 0; j < qbins; j++) {
			pq_h.SetBinContent(i+1, j+1, Grid[i][j]);
		}
	}
	gStyle->SetPalette(53);
	gStyle->SetPaintTextFormat("5.2f");
	TCanvas c("c", "c", 650, 600);
	c.SetRightMargin(0.1346749);
	pq_h.SetStats(false);
	pq_h.SetMaximum(1.);
	pq_h.SetMinimum(0.);
	string draw_s("COLZTEXT");
	if (qbins > 30 || pbins > 30) draw_s = "COLZ";
	pq_h.Draw(draw_s.c_str());
	c.Print(Form("figs/PQgrid_%s.pdf", name_.c_str()));
}

void RetinaTrackFitter::findMaxima() {
	for (unsigned int i = 1; i < pbins-1; i++) {
		for (unsigned int j = 1; j < qbins-1; j++) {
			if (   Grid[i][j] > Grid[i-1][j]
			    && Grid[i][j] > Grid[i+1][j]
			    && Grid[i][j] > Grid[i][j-1]
			    && Grid[i][j] > Grid[i][j+1]

			    && Grid[i][j] > Grid[i+1][j+1]
		      && Grid[i][j] > Grid[i+1][j-1]
	        && Grid[i][j] > Grid[i-1][j-1]
	        && Grid[i][j] > Grid[i-1][j+1]
	 ) {
				if (Grid[i][j] < 0.2) continue; // cleaning
				pqPoint_i point_i;
				point_i.p = i;
				point_i.q = j;
//				cout << "not interpolated " << Grid[i][j] << endl;
				pqPoint point_interpolated = findMaximumInterpolated(point_i, Grid[i][j]);
				pqCollection.push_back(point_interpolated);
			}
		}
	}
}

pqPoint RetinaTrackFitter::findMaximumInterpolated(pqPoint_i point_i, double w) {
	int p_i = point_i.p;
	int q_i = point_i.q;

	double p_mean = 0.;
	double q_mean = 0.;
//	cout << "p = " << (pmax-pmin)/pbins * (p_i) + pmin << "   q = " <<  (qmax-qmin)/qbins * (q_i) + qmin << "  w = " << w << endl;
	p_mean =  ((pmax-pmin)/pbins * (p_i-1) + pmin) * Grid[p_i-1][q_i] +
						((pmax-pmin)/pbins * (p_i) + pmin) * Grid[p_i][q_i] +
						((pmax-pmin)/pbins * (p_i+1) + pmin) * Grid[p_i+1][q_i];
	p_mean /= Grid[p_i-1][q_i] + Grid[p_i][q_i] + Grid[p_i+1][q_i];


	q_mean =  ((qmax-qmin)/qbins * (q_i-1) + qmin) * Grid[p_i][q_i-1] +
						((qmax-qmin)/qbins * (q_i) + qmin) * Grid[p_i][q_i] +
						((qmax-qmin)/qbins * (q_i+1) + qmin) * Grid[p_i][q_i+1];
	q_mean /= Grid[p_i][q_i-1] + Grid[p_i][q_i] + Grid[p_i][q_i+1];

	pqPoint point_o;
	point_o.p = p_mean;
	point_o.q = q_mean;
	point_o.w = w;
//	cout << "interpolated " << Grid[p_mean][j] << endl;
	return point_o;
}

void RetinaTrackFitter::printMaxima() {
	unsigned int size = pqCollection.size();
	cout << "MAXIMA p , q , w:" << endl;
	for (unsigned int i = 0; i < size; i++) {
		cout << pqCollection[i].p << " , " << pqCollection[i].q << " , " << pqCollection[i].w << endl;
	}
}

vector <circlePoint> RetinaTrackFitter::getCircles() {
	unsigned int lines_size = pqCollection.size();
	for (unsigned int line_i = 0; line_i < lines_size; line_i++) {
		circlePoint circle_i;
		circle_i.a = get_a(pqCollection[line_i].p, pqCollection[line_i].q);
		circle_i.b = get_b(pqCollection[line_i].p, pqCollection[line_i].q);
		circleCollection.push_back(circle_i);
		cout << "CIRCLE with RETINA: a = " << circle_i.a << "  b = " << circle_i.b << endl;
	}
	return circleCollection;
}

void RetinaTrackFitter::drawTracks() {

	unsigned int hits_n = hitCollection.size();
	TGraph hits_h(hits_n);
//	double pt = 0.3 * 3.8 * R_gen_ / 100.;
	hits_h.SetTitle(Form("Hits and retina lines"));
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hits_h.SetPoint(hit_i, hitcollref->at(hit_i).x, hitcollref->at(hit_i).y);
	}
	TCanvas c("c", "c", 600, 600);
	hits_h.GetXaxis()->SetLimits(0., hits_h.GetXaxis()->GetXmax());
	hits_h.GetYaxis()->SetRangeUser(0., hits_h.GetYaxis()->GetXmax());
	hits_h.GetXaxis()->SetTitle("x[cm]");
	hits_h.GetYaxis()->SetTitle("y[cm]");
	hits_h.Draw("A*");

	unsigned int line_size = pqCollection.size();
	vector <TF1*> lines(line_size);
	for (unsigned int line_i = 0; line_i < line_size; line_i++) {
		double p = pqCollection[line_i].p;
		double q = pqCollection[line_i].q;
		lines[line_i] = new TF1(Form("line_%d", line_i), Form("%f*x+%f", p, q), 0, 1200.);
		lines[line_i]->SetLineColor(kBlue + (short int)line_i);
		lines[line_i]->Draw("same");
	}

	c.Print(Form("figs/hitsRetinaTracks_%s.pdf", name_.c_str()));
}

track RetinaTrackFitter::getBestTrack() {
	unsigned int size = pqCollection.size();
	double max = 0.1;
	track track;
	for (unsigned int i = 0; i < size; i++) {
		if (pqCollection[i].w > max) {
			max = pqCollection[i].w;
			double a = get_a(pqCollection[i].p, pqCollection[i].q);
			double b = get_b(pqCollection[i].p, pqCollection[i].q);
			track.phi = get_phi_from_car(a, b);
			track.pt = get_r_from_car(a, b) / 100. * 0.3 * 3.8;
			cout << "a = "<< a << " b = " << b << " p = " << pqCollection[i].p << " r = " << get_r_from_car(a, b) << " ";
		}
	}
	if (max == 0.1) track.pt = -1.;
	cout << "pt = " << track.pt << " phi = " << track.phi << endl;
	return track;
}

pqPoint RetinaTrackFitter::getBestPQ() {
	unsigned int size = pqCollection.size();
	double max = 0.;
	pqPoint bestPQ;
	bestPQ.w = -1.;
	for (unsigned int i = 0; i < size; i++) {
		if (pqCollection[i].w > max) {
			max = pqCollection[i].w;
			bestPQ = pqCollection[i];
		}
	}
	return bestPQ;
}
