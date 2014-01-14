/*
 * RetinaTrackFitter.cpp
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 */

#include "RetinaTrackFitter.h"

RetinaTrackFitter::RetinaTrackFitter(vector <hit> hitCollection_, unsigned int pbins_, unsigned int qbins_, double pmin_, double pmax_, double qmin_, double qmax_, bool parabola, string name) :
  hitCollection(hitCollection_),
	pbins(pbins_),
  qbins(qbins_),
  pmin(pmin_),
  pmax(pmax_),
  qmin(qmin_),
  qmax(qmax_),
  sigma(0.001),
  parabola_b(parabola),
  name_(name)
{
	hitsConf_h = new TGraph(0);
//	cout << "CIRCLE GENERATED: a = " << a_gen_ << "  b = " << b_gen_ << endl;
	makeGrid();
}

RetinaTrackFitter::~RetinaTrackFitter() {
}

void RetinaTrackFitter::drawHits() {
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
	c.Update();
	c.Print(Form("figs/hits_%s.pdf", name_.c_str()));
}

void RetinaTrackFitter::setConfHits() {
	unsigned int hits_n = hitCollection.size();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		double x =  hitCollection[hit_i].x;
		double y =  hitCollection[hit_i].y;
		double u = get_u(x, y);
		double v = get_v(x, y);
		hitConf hitConf_i;
		hitConf_i.u = u;
		hitConf_i.v = v;
		hitConfCollection.push_back(hitConf_i);
	}
}

pqPoint RetinaTrackFitter::drawHitsConf() {
	unsigned int hits_n = hitConfCollection.size();
	hitsConf_h->Set(hits_n);
	double pt = 0.3 * 3.8 * R_gen_ / 100.;
	hitsConf_h->SetTitle(Form("Hits in conformal space, p_{T} = %.f GeV", pt));
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hitsConf_h->SetPoint(hit_i, hitConfCollection[hit_i].u, hitConfCollection[hit_i].v);
	}
	TCanvas c("c", "c", 600, 600);
	gStyle->SetOptFit(1111);
	hitsConf_h->GetXaxis()->SetTitle("u[cm^{-1}]");
	hitsConf_h->GetYaxis()->SetTitle("v[cm^{-1}]");
	hitsConf_h->Draw("A*");
	hitsConf_h->Fit("pol1");
	TF1* fun = hitsConf_h->GetFunction("pol1");
	fun->SetParNames("q","p");
	c.Print(Form("figs/hitsConf_%s.pdf", name_.c_str()));
	pqPoint truepq;
	truepq.p = fun->GetParameter("p");
	truepq.q = fun->GetParameter("q");
	return truepq;
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
			double p_value = (pmax-pmin)/pbins * i + pmin;
			double q_value = (qmax-qmin)/qbins * j + qmin;
			Grid[i][j] = getResponse(p_value, q_value);
//			cout << i << " " << j << " p = " << p_value << " q = " << q_value << " w = " << Grid[i][j] << endl;
		}
	}
}

double RetinaTrackFitter::getResponse(double p_temp, double q_temp) {
//	cout << p_temp << " " << q_temp << endl;
	double Rij = 0.;
	unsigned int hits_tot = hitConfCollection.size();
	for (unsigned int kr = 0; kr < hits_tot; kr++) {
		double u_l = (hitConfCollection[kr].v - q_temp) / p_temp;
		double Sijkr = hitConfCollection[kr].u - u_l;
		double term = exp(Sijkr*Sijkr/(-2.*sigma*sigma));
//		cout << term;
		Rij += term;
//		cout << " v = pu + q =>" << hitConfCollection[kr].v << " = " << p_temp << " * " << u_l << " + " << q_temp << endl;
		//cout <<  "u = " << hitConfCollection[kr].u << " u_line = " << u_line <<  endl;
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
//				if (Grid[i][j] < 0.25) continue; // cleaning
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
	cout << "p = " << (pmax-pmin)/pbins * (p_i) + pmin << "   q = " <<  (qmax-qmin)/qbins * (q_i) + qmin << "  w = " << w << endl;
	p_mean =  ((pmax-pmin)/pbins * (p_i-1) + pmin) * pow(Grid[p_i-1][q_i],2) +
						((pmax-pmin)/pbins * (p_i) + pmin) * pow(Grid[p_i][q_i],2) +
						((pmax-pmin)/pbins * (p_i+1) + pmin) * pow(Grid[p_i+1][q_i],2);
	p_mean /= pow(Grid[p_i-1][q_i],2) + pow(Grid[p_i][q_i],2) + pow(Grid[p_i+1][q_i],2);


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
	cout << "MAXIMA p , q:" << endl;
	for (unsigned int i = 0; i < size; i++) {
		cout << pqCollection[i].p << " , " << pqCollection[i].q << endl;
	}
}

void RetinaTrackFitter::drawHitsConfRetina() {
	unsigned int hits_n = hitConfCollection.size();
	TGraph hitsConf_g(hits_n);
	double pt = 0.3 * 3.8 * R_gen_ / 100.;
	hitsConf_g.SetTitle(Form("Hits in conformal space and retina lines, p_{T} = %.f GeV", pt));
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hitsConf_g.SetPoint(hit_i, hitConfCollection[hit_i].u, hitConfCollection[hit_i].v);
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
		lines[line_i] = new TF1(Form("line_%d", line_i), "pol1", -5., 5.);
		lines[line_i]->SetParameters(q_i, p_i);
		lines[line_i]->SetLineColor(kBlue + line_i);
		lines[line_i]->Draw("same");
	}
	cc.Print(Form("figs/hitsConfRetina_%s.pdf", name_.c_str()));
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

	unsigned int hits_n = hitCollection.size();
	TGraph hits_h(hits_n);
	double pt = 0.3 * 3.8 * R_gen_ / 100.;
	hits_h.SetTitle(Form("Hits and retina circles, p_{T} = %.f GeV", pt));
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hits_h.SetPoint(hit_i, hitCollection[hit_i].x, hitCollection[hit_i].y);
	}
	TCanvas c("c", "c", 600, 600);
	hits_h.GetXaxis()->SetLimits(0., hits_h.GetXaxis()->GetXmax());
	hits_h.GetYaxis()->SetRangeUser(0., hits_h.GetYaxis()->GetXmax());
	hits_h.GetXaxis()->SetTitle("x[cm]");
	hits_h.GetYaxis()->SetTitle("y[cm]");
	hits_h.Draw("A*");

	string circle_leg;
	TEllipse circle_origin(a_gen_, b_gen_, R_gen_);
	circle_origin.SetNoEdges();
	circle_origin.SetLineColor(kRed);
	circle_origin.SetLineStyle(2);
	circle_origin.SetFillStyle(0);
	circle_origin.Draw("same");

	unsigned int circles_size = circleCollection.size();
	vector <TEllipse*> circle_f;
	circle_f.resize(circles_size);
	for (unsigned int circle_i = 0; circle_i < circles_size; circle_i++) {
		double a = circleCollection[circle_i].a;
		double b = circleCollection[circle_i].b;
		double R = sqrt(a*a + b*b);
		circle_f[circle_i] = new TEllipse(a, b, R);
		circle_f[circle_i]->SetNoEdges();
		circle_f[circle_i]->SetLineColor(kBlue + circle_i);
		circle_f[circle_i]->SetFillStyle(0);
		circle_f[circle_i]->Draw("same");
	}

	c.Print(Form("figs/hitsRetina_%s.pdf", name_.c_str()));
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
	double max = 0.1;
	pqPoint bestPQ;
	for (unsigned int i = 0; i < size; i++) {
		if (pqCollection[i].w > max) {
			max = pqCollection[i].w;
			bestPQ = pqCollection[i];
		}
	}
	return bestPQ;
}
