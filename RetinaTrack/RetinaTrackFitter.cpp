/*
 * RetinaTrackFitter.cpp
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 */

#include "RetinaTrackFitter.h"

RetinaTrackFitter::RetinaTrackFitter(HitCollection hitCollection_, unsigned int pbins_, unsigned int qbins_, double pmin_, double pmax_, double qmin_, double qmax_, double sigma_, double minWeight_, string name, int para_) :
  hitCollection(hitCollection_),
	pbins(pbins_),
  qbins(qbins_),
  pmin(pmin_),
  pmax(pmax_),
  qmin(qmin_),
  qmax(qmax_),
  sigma(sigma_),
  minWeight(minWeight_),
  name_(name),
  para(para_)
{
	pbinsize = (pmax-pmin)/pbins;
	qbinsize = (qmax-qmin)/qbins;
	makeGrid();

	LayerGeometry LG;
	layers = LG.get_barrel_layers_plain();

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
			double p_value = pmin + pbinsize * (i + 0.5);
			double q_value = qmin + qbinsize * (j + 0.5);
			if (para == 0) Grid[i][j] = getResponse(p_value, q_value);
			if (para == 1) Grid[i][j] = getResponseRistori(p_value, q_value);
			if (para == 2) Grid[i][j] = getResponsePol(p_value, q_value);
//			cout << i << " " << j << " p = " << p_value << " q = " << q_value << " w = " << Grid[i][j] << endl;
		}
	}
}

double RetinaTrackFitter::getResponse(double m_temp, double b_temp) {
//	cout << p_temp << " " << q_temp << endl;
	double Rij = 0.;
	unsigned int hits_tot = hitCollection.size();
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	for (unsigned int kr = 0; kr < hits_tot; kr++) {

		double y_k = (hitcollref->at(kr).y - b_temp) / m_temp;  // <-- y = mx + b --> x = (y-b)/m
		double Sijkr = hitcollref->at(kr).x - y_k;

//		if ( Sijkr / sigma > 3. ) continue; // cut outliers

//		Sijkr = fabs((hitcollref->at(kr).y - m_temp*hitcollref->at(kr).x - b_temp) / sqrt(1. + m_temp*m_temp));

		double term = exp(Sijkr*Sijkr/(-2.*sigma*sigma));

//		cout << y_k << "  " << Sijkr << "  " << term << endl;
		Rij += term;
//		cout << " y = px + q =>" << hitcollref->at(kr).y << " = " << p_temp << " * " << y_k << " + " << q_temp;
//		cout <<  "   u = " << hitcollref->at(kr).x  <<  endl;
	}
//	Rij /= hits_tot;
//	if (Rij < minWeight) Rij = 1e-6; // cleaning
	if (Rij < 1e-6) return 1e-6;
	else return Rij;
}

double RetinaTrackFitter::getResponseRistori(double x_plus, double x_minus) {
//	cout << p_temp << " " << q_temp << endl;
	double Rij = 0.;

	double y2 = layers.at(0).q;
	double y1 = layers.at(layers.size()-1).q;
	double x2 = x_plus - x_minus;
	double x1 = x_plus + x_minus;
	double m = (y1 - y2) / (x1 - x2);
	double b = y2 - m*x2;

	unsigned int hits_tot = hitCollection.size();
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	for (unsigned int kr = 0; kr < hits_tot; kr++) {

		double y_k = (hitcollref->at(kr).y - b) / m;  // <-- y = mx + b --> x = (y-b)/m
		double Sijkr = hitcollref->at(kr).x - y_k;

//		if ( Sijkr / sigma > 3. ) continue; // cut outliers

//		Sijkr = fabs((hitcollref->at(kr).y - m_temp*hitcollref->at(kr).x - b_temp) / sqrt(1. + m_temp*m_temp));

		double term = exp(Sijkr*Sijkr/(-2.*sigma*sigma));

//		cout << y_k << "  " << Sijkr << "  " << term << endl;
		Rij += term;
//		cout << " y = px + q =>" << hitcollref->at(kr).y << " = " << p_temp << " * " << y_k << " + " << q_temp;
//		cout <<  "   u = " << hitcollref->at(kr).x  <<  endl;
	}
//	Rij /= hits_tot;
//	if (Rij < minWeight) Rij = 1e-6; // cleaning
	if (Rij < 1e-6) return 1e-6;
	else return Rij;
}

double RetinaTrackFitter::getResponsePol(double gamma_temp, double b_temp) {
//	cout << p_temp << " " << q_temp << endl;
	double Rij = 0.;
	unsigned int hits_tot = hitCollection.size();
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	for (unsigned int kr = 0; kr < hits_tot; kr++) {

		double theta = atan2(hitcollref->at(kr).y, hitcollref->at(kr).x);

		double y_k = (b_temp) * cos(gamma_temp) * cos(theta) / sin(theta - gamma_temp); // shift away from origin
//		cout << "theta hit = " << theta <<  "   x test = " << y_k << endl;
		double Sijkr = hitcollref->at(kr).x - y_k;
		double term = exp(Sijkr*Sijkr/(-2.*sigma*sigma));
//		cout << y_k << "  " << Sijkr << "  " << term << endl;
		Rij += term;
//		cout << " y = px + q =>" << hitcollref->at(kr).y << " = " << p_temp << " * " << y_k << " + " << q_temp;
//		cout <<  "   u = " << hitcollref->at(kr).x  <<  endl;
	}
//	Rij /= hits_tot;
	if (Rij < 1e-6) return 1e-6;
	else return Rij;
}

void RetinaTrackFitter::drawPQGrid() {
  TH2D pq_h("pq_h", "pq grid;p;q", pbins, pmin, pmax, qbins, qmin, qmax);
//  TH2D pq_h("pq_h", "pq grid;p;q[cm]", pbins-1, pmin - (pmax-pmin)/pbins/2, pmax - (pmax-pmin)/pbins*3/2, qbins-1, qmin - (qmax-qmin)/qbins/2 - offset, qmax - (qmax-qmin)/qbins*3/2 - offset);
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
	pq_h.SetMaximum(6.);
	pq_h.SetMinimum(0.);
	string draw_s("COLZTEXT");
	if (qbins > 30 || pbins > 30) draw_s = "COLZ";
	pq_h.Draw(draw_s.c_str());
	c.Print(Form("figs/PQgrid_%s.png", name_.c_str()));

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
				if (Grid[i][j] < minWeight) continue; // cleaning
				pqPoint_i point_i;
				point_i.p = i;
				point_i.q = j;
//				cout << "not interpolated: " << i << ", " << j << Grid[i][j] << endl;
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
	p_mean = (pmin + pbinsize * (p_i - 0.5)) * Grid[p_i-1][q_i] +
					 (pmin + pbinsize * (p_i + 0.5)) * Grid[p_i][q_i] +
					 (pmin + pbinsize * (p_i + 1.5)) * Grid[p_i+1][q_i];

	p_mean += (pmin + pbinsize * (p_i - 0.5)) * Grid[p_i-1][q_i-1] +
						(pmin + pbinsize * (p_i + 0.5)) * Grid[p_i][q_i-1] +
						(pmin + pbinsize * (p_i + 1.5)) * Grid[p_i+1][q_i-1];

	p_mean += (pmin + pbinsize * (p_i - 0.5)) * Grid[p_i-1][q_i+1] +
						(pmin + pbinsize * (p_i + 0.5)) * Grid[p_i][q_i+1] +
						(pmin + pbinsize * (p_i + 1.5)) * Grid[p_i+1][q_i+1];

	p_mean /= (Grid[p_i-1][q_i] + Grid[p_i][q_i] + Grid[p_i+1][q_i] +
						 Grid[p_i-1][q_i-1] + Grid[p_i][q_i-1] + Grid[p_i+1][q_i-1] +
						 Grid[p_i-1][q_i+1] + Grid[p_i][q_i+1] + Grid[p_i+1][q_i+1]);


	q_mean =  (qmin + qbinsize * (q_i - 0.5)) * Grid[p_i][q_i-1] +
						(qmin + qbinsize * (q_i + 0.5)) * Grid[p_i][q_i] +
						(qmin + qbinsize * (q_i + 1.5)) * Grid[p_i][q_i+1];

	q_mean += (qmin + qbinsize * (q_i - 0.5)) * Grid[p_i-1][q_i-1] +
						(qmin + qbinsize * (q_i + 0.5)) * Grid[p_i-1][q_i] +
						(qmin + qbinsize * (q_i + 1.5)) * Grid[p_i-1][q_i+1];

	q_mean += (qmin + qbinsize * (q_i - 0.5)) * Grid[p_i+1][q_i-1] +
						(qmin + qbinsize * (q_i + 0.5)) * Grid[p_i+1][q_i] +
						(qmin + qbinsize * (q_i + 1.5)) * Grid[p_i+1][q_i+1];

	q_mean /= (Grid[p_i][q_i-1] + Grid[p_i][q_i] + Grid[p_i][q_i+1] +
						 Grid[p_i-1][q_i-1] + Grid[p_i-1][q_i] + Grid[p_i-1][q_i+1] +
						 Grid[p_i+1][q_i-1] + Grid[p_i+1][q_i] + Grid[p_i+1][q_i+1]);

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

vector <circle> RetinaTrackFitter::getCircles() {
	unsigned int lines_size = pqCollection.size();
	for (unsigned int line_i = 0; line_i < lines_size; line_i++) {
		circle circle_i;
		circle_i.a = get_a(pqCollection[line_i].p, pqCollection[line_i].q);
		circle_i.b = get_b(pqCollection[line_i].p, pqCollection[line_i].q);
		circleCollection.push_back(circle_i);
//		cout << "CIRCLE with RETINA: a = " << circle_i.a << "  b = " << circle_i.b << endl;
	}
	return circleCollection;
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
//	cout << " bestPQp " << bestPQ.p << "   bestPQq = " << bestPQ.q << " best w " << bestPQ.w << endl;
	return bestPQ;
}

circle RetinaTrackFitter::getBestCircle() {
	pqPoint bestPQ = getBestPQ();
	circle circle_i;
	circle_i.a = get_a(bestPQ.p, bestPQ.q);
	circle_i.b = get_b(bestPQ.p, bestPQ.q);
//	cout << " best_a " << circle_i.a << "   best_b = " << circle_i.b << endl;
	return circle_i;
}
