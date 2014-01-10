/*
 * RetinaTrackFitter.cpp
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 */

#include "RetinaTrackFitter.h"

RetinaTrackFitter::RetinaTrackFitter(double a_gen, double b_gen, bool parabola, string name) :
	a_gen_(a_gen),
	b_gen_(b_gen),
	pbins(21),
  qbins(21),
  pmin(-0.008),
  pmax(0.008),
  qmin(0.01),
  qmax(0.05),
  sigma(0.02),
  parabola_b(parabola),
  name_(name)
{
	cout << "CIRCLE GENERATED: a = " << a_gen_ << "  b = " << b_gen_ << endl;
	hitsConf_h = new TGraph(0);

	setGeometry();
	makeGrid();
}

RetinaTrackFitter::~RetinaTrackFitter() {
}

void RetinaTrackFitter::setGeometry() {
	barrel_layer_r.push_back(23.0);
	barrel_layer_r.push_back(35.7);
	barrel_layer_r.push_back(50.8);
	barrel_layer_r.push_back(68.6);
	barrel_layer_r.push_back(88.8);
	barrel_layer_r.push_back(108.0);
}

unsigned int RetinaTrackFitter::setHits() {
	TRandom3 rand;

	unsigned int  barrel_layer_size = barrel_layer_r.size();
	for (unsigned int i = 0; i < barrel_layer_size; i++) {
		double spread = 0;
		spread = rand.Gaus(0, 0.01);
		spread = 0;
		double x = 0;
		double y = 0;
//		double R_sq = a_gen_*a_gen_ + b_gen_*b_gen_;
		double radicand = 4*b_gen_*b_gen_ - barrel_layer_r[i]*barrel_layer_r[i] + 4*a_gen_*a_gen_;
		if (radicand < 0) continue;
		y = (b_gen_*barrel_layer_r[i]*barrel_layer_r[i] + fabs(a_gen_)*barrel_layer_r[i]*sqrt(radicand))/(2*a_gen_*a_gen_ + 2*b_gen_*b_gen_); // higher
		x = (barrel_layer_r[i]*barrel_layer_r[i] - 2*b_gen_*y)/(2*a_gen_);
		hit hit_i;
		hit_i.x = x + spread;
		hit_i.y = y;
//		if (x >0.) HitCollection.push_back(hit_i);
//		cout << "x,y = " << x << " " << y << ":  ";
//		cout << barrel_layer_r[i] << " = " << sqrt(x*x + y*y) << " ?  ";
//		cout << sqrt(a_gen_*a_gen_ + b_gen_*b_gen_) << " = " << sqrt((x-a_gen_)*(x-a_gen_) + (y-b_gen_)*(y-b_gen_)) << " ?" << endl;

  	y = (b_gen_*barrel_layer_r[i]*barrel_layer_r[i] - fabs(a_gen_)*barrel_layer_r[i]*sqrt(radicand))/(2*a_gen_*a_gen_ + 2*b_gen_*b_gen_); // higher
  	x = (barrel_layer_r[i]*barrel_layer_r[i] - 2*b_gen_*y)/(2*a_gen_);
  	hit_i.x = x + spread;
  	hit_i.y = y;
  	HitCollection.push_back(hit_i);
  	cout << "x,y = " << x << " " << y << ":  ";
  	cout << barrel_layer_r[i] << " = " << sqrt(x*x + y*y) << " ?  ";
  	cout << sqrt(a_gen_*a_gen_ + b_gen_*b_gen_) << " = " << sqrt((x-a_gen_)*(x-a_gen_) + (y-b_gen_)*(y-b_gen_)) << " ?" << endl;

	}

	return HitCollection.size();
}

unsigned int RetinaTrackFitter::cleanHits(double ymin, double ymax, double xmin, double xmax) {
	vector <hit> HitCollectionNew;
	unsigned int hits_n = HitCollection.size();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		if (HitCollection[hit_i].y > ymin && HitCollection[hit_i].y <= ymax
				&& HitCollection[hit_i].x > xmin && HitCollection[hit_i].x <= xmax) {
			HitCollectionNew.push_back(HitCollection[hit_i]);
		}
	}
	HitCollection = HitCollectionNew;
	return HitCollection.size();
}

void RetinaTrackFitter::drawHits() {
	unsigned int hits_n = HitCollection.size();
	TGraph hits_h(hits_n);
	hits_h.SetTitle("Hits");
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hits_h.SetPoint(hit_i, HitCollection[hit_i].x, HitCollection[hit_i].y);
	}
	TCanvas c("c", "c", 600, 600);
//	hits_h.GetXaxis()->SetLimits(0.,150.);
//	hits_h.GetYaxis()->SetRangeUser(0.,150.);
	hits_h.GetXaxis()->SetTitle("x[cm]");
	hits_h.GetYaxis()->SetTitle("y[cm]");
	hits_h.Draw("A*");
	c.Update();
	c.Print(Form("hits_%s.pdf", name_.c_str()));
}

void RetinaTrackFitter::setConfHits() {
	unsigned int hits_n = HitCollection.size();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		double x =  HitCollection[hit_i].x;
		double y =  HitCollection[hit_i].y;
		double u = get_u(x, y);
		double v = get_v(x, y);
		hitConf hitConf_i;
		hitConf_i.u = u;
		hitConf_i.v = v;
		HitConfCollection.push_back(hitConf_i);
	}
}

void RetinaTrackFitter::drawHitsConf() {
	unsigned int hits_n = HitConfCollection.size();
	hitsConf_h->Set(hits_n);
	hitsConf_h->SetTitle("Hits in conformal space");
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hitsConf_h->SetPoint(hit_i, HitConfCollection[hit_i].u, HitConfCollection[hit_i].v);
	}
	TCanvas c("c", "c", 600, 600);
	gStyle->SetOptFit(1111);
	hitsConf_h->GetXaxis()->SetTitle("u[cm^{-1}]");
	hitsConf_h->GetYaxis()->SetTitle("v[cm^{-1}]");
	hitsConf_h->Draw("A*");
	hitsConf_h->Fit("pol1");
	TF1* fun = hitsConf_h->GetFunction("pol1");
	fun->SetParNames("q","p");
	c.Print(Form("hitsConf_%s.pdf", name_.c_str()));
}

void RetinaTrackFitter::GetFromConfToCircle() {
	TF1* fun = hitsConf_h->GetFunction("pol1");
	q = fun->GetParameter(0);
	p = fun->GetParameter(1);
	a_L1f = get_a(p, q);
	b_L1f = get_b(p, q);
//	cout << "a = " << a_L1f << "  b = " << b_L1f << endl;
//	cout << "p = " << get_p(a_L1f, b_L1f) << "  q = " << get_q(a_L1f, b_L1f) << endl;
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
			double p_value = (pmax-pmin)/pbins * (i) + pmin;
			double q_value = (qmax-qmin)/qbins * (j) + qmin;
			Grid[i][j] = getResponse(p_value, q_value);
//			cout << p_value << " " << q_value << endl;
		}
	}
}

double RetinaTrackFitter::getResponse(double p_temp, double q_temp) {
//	cout << p_temp << " " << q_temp << endl;
	double Rij = 0.;
	unsigned int hits_tot = HitConfCollection.size();
	for (unsigned int kr = 0; kr < hits_tot; kr++) {
		double u_l = (HitConfCollection[kr].v - q_temp) / p_temp;
		double Sijkr = HitConfCollection[kr].u - u_l;
		double term = exp(Sijkr*Sijkr/(-2.*sigma*sigma));
//		cout << term;
		Rij += term;
//		cout << " v = pu + q =>" << HitConfCollection[kr].v << " = " << p_temp << " * " << u_l << " + " << q_temp << endl;
		//cout <<  "u = " << HitConfCollection[kr].u << " u_line = " << u_line <<  endl;
	}
	Rij /= hits_tot;
	if (Rij < 1e-6) return 1e-6;
	else return Rij;
}

void RetinaTrackFitter::drawPQGrid() {
//  TH2D pq_h("pq_h", "pq grid;p;q", pbins-1, pmin - (pmax-pmin)/pbins/2, pmax - (pmax-pmin)/pbins*3/2, qbins-1, qmin - (qmax-qmin)/qbins/2, qmax - (qmax-qmin)/qbins*3/2);
  TH2D pq_h("pq_h", "pq grid;p;q[cm^{-1}]", pbins-1, pmin - (pmax-pmin)/pbins/2, pmax - (pmax-pmin)/pbins*3/2, qbins-1, qmin - (qmax-qmin)/qbins/2, qmax - (qmax-qmin)/qbins*3/2);
	for (unsigned int i = 0; i < pbins; i++) {
		for (unsigned int j = 0; j < qbins; j++) {
			pq_h.SetBinContent(i+1, j+1, Grid[i][j]);
		}
	}
	gStyle->SetPalette(53);
	gStyle->SetPaintTextFormat("5.2f");
	TCanvas c("c", "c", 600, 600);
	pq_h.SetStats(false);
	pq_h.Draw("COLZtext");
	c.Print(Form("PQgrid_%s.pdf", name_.c_str()));
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
				if (Grid[i][j] < 0.1) continue; // cleaning
				pqPoint_i point_i;
				point_i.p = i;
				point_i.q = j;
//				cout << "not interpolated " << Grid[i][j] << endl;
				pqPoint point_interpolated = findMaximumInterpolated(point_i);
				pqCollection.push_back(point_interpolated);
			}
		}
	}
}

pqPoint RetinaTrackFitter::findMaximumInterpolated(pqPoint_i point_i) {
	int p_i = point_i.p;
	int q_i = point_i.q;

	double p_mean = 0.;
	double q_mean = 0.;
	cout << "p = " << (pmax-pmin)/pbins * (p_i) + pmin << "   q = " <<  (qmax-qmin)/qbins * (q_i) + qmin << endl;
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
//	cout << "interpolated " << Grid[p_mean][j] << endl;
	return point_o;
}

void RetinaTrackFitter::printMaxima() {
	unsigned int size = pqCollection.size();
	cout << "MAXIMA p , q:" << endl;
	for (unsigned int i = 0; i < size; i++) {
		cout << pqCollection[i].p << " , " << pqCollection[i].q << endl;
	}
}

void RetinaTrackFitter::drawHitsConfRetina() {
	unsigned int hits_n = HitConfCollection.size();
	TGraph hitsConf_g(hits_n);
	hitsConf_g.SetTitle("Hits in conformal space and retina lines");
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hitsConf_g.SetPoint(hit_i, HitConfCollection[hit_i].u, HitConfCollection[hit_i].v);
	}
	TCanvas cc("cc", "cc", 600, 600);
//		gStyle->SetOptFit(1111);
	hitsConf_g.GetXaxis()->SetTitle("u[cm^{-1}]");
	hitsConf_g.GetYaxis()->SetTitle("v[cm^{-1}]");
	hitsConf_g.Draw("A*");

	TF1* fun = hitsConf_h->GetFunction("pol1");
	fun->SetLineStyle(2);
	fun->Draw("same");

	vector < TF1* > lines;
	unsigned int lines_size = pqCollection.size();
	lines.resize(lines_size);
	for (unsigned int line_i = 0; line_i < lines_size; line_i++) {
		double p_i = pqCollection[line_i].p;
		double q_i = pqCollection[line_i].q;
		lines[line_i] = new TF1(Form("line_%d", line_i), "pol1", 0., 5.);
		lines[line_i]->SetParameters(q_i, p_i);
		lines[line_i]->SetLineColor(kBlue + line_i);
		lines[line_i]->Draw("same");
	}
	cc.Print(Form("hitsConfRetina_%s.pdf", name_.c_str()));
}

void RetinaTrackFitter::getCircles() {
	unsigned int lines_size = pqCollection.size();
	for (unsigned int line_i = 0; line_i < lines_size; line_i++) {
		circlePoint circle_i;
		circle_i.a = get_a(pqCollection[line_i].p, pqCollection[line_i].q);
		circle_i.b = get_b(pqCollection[line_i].p, pqCollection[line_i].q);
		circleCollection.push_back(circle_i);
		cout << "CIRCLE with RETINA: a = " << circle_i.a << "  b = " << circle_i.b << endl;
	}
}

void RetinaTrackFitter::drawCircles() {

	unsigned int hits_n = HitCollection.size();
	TGraph hits_h(hits_n);
	hits_h.SetTitle("Hits and retina circles");
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hits_h.SetPoint(hit_i, HitCollection[hit_i].x, HitCollection[hit_i].y);
	}
	TCanvas c("c", "c", 600, 600);
	hits_h.GetXaxis()->SetLimits(0.,5.);
	hits_h.GetYaxis()->SetRangeUser(0.,5.);
	hits_h.GetXaxis()->SetTitle("x[cm]");
	hits_h.GetYaxis()->SetTitle("y[cm]");
	hits_h.Draw("A*");

	TF1 * circle_origin = new TF1( "circle_origin", "[1] - sqrt([2] - (x - [0])*(x - [0]) )", 0., 5.);
	circle_origin->SetParameters(a_gen_, b_gen_, a_gen_*a_gen_ + b_gen_*b_gen_);
	circle_origin->SetLineStyle(2);
	circle_origin->Draw("same");

	unsigned int circles_size = circleCollection.size();
	vector <TF1 *> circle_f;
	circle_f.resize(circles_size);
	for (unsigned int circle_i = 0; circle_i < circles_size; circle_i++) {
		double a = circleCollection[circle_i].a;
		double b = circleCollection[circle_i].b;
		double R_sq = a*a + b*b;
		circle_f[circle_i] = new TF1( Form("circle_%i", circle_i), "[1] - sqrt([2] - (x - [0])*(x - [0]) )", 0., 5.);
		circle_f[circle_i]->SetParameters(a, b, R_sq);
		circle_f[circle_i]->SetLineColor(kBlue + circle_i);
		circle_f[circle_i]->Draw("same");
	}
	c.Print(Form("hitsRetina_%s.pdf", name_.c_str()));
}
