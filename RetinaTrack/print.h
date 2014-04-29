/*
 * print.h
 *
 *  Created on: 13/feb/2014
 *      Author: lucamartini
 */

#ifndef PRINT_H_
#define PRINT_H_

void drawTracks(HitCollection hitCollection, vector <pqPoint> pqCollection, pqPoint truepq, unsigned int best, int event, string name, LayerGeometry * LG, int para) {

	unsigned int hits_n = hitCollection.size();
	TGraph hits_h(hits_n);
//	double pt = 0.3 * 3.8 * R_gen_ / 100.;
	hits_h.SetTitle(Form("Hits and retina lines, y = %.2f x + %.2f", truepq.p, truepq.q));
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		hits_h.SetPoint(hit_i, hitcollref->at(hit_i).x, hitcollref->at(hit_i).y);
	}
	TCanvas c("c", "c", 600, 600);
	hits_h.GetXaxis()->SetLimits(-1.1*zmax, 1.1*zmax);
	hits_h.GetYaxis()->SetRangeUser(0., 1.1*rmax);
	hits_h.GetXaxis()->SetTitle("x[cm]");
	hits_h.GetYaxis()->SetTitle("y[cm]");
	hits_h.Draw("A*");

	unsigned int line_size = pqCollection.size();
	vector <TF1*> lines(line_size);
	for (unsigned int line_i = 0; line_i < line_size; line_i++) {
		double m, b;
		if (para == 0 ) {
			m = pqCollection[line_i].p;
			b = pqCollection[line_i].q;
		}
		if (para == 1) {
			vector <line> layers = LG->get_barrel_layers_plain();
			double y0 = layers.at(0).q;
			double y1 = layers.at(layers.size()-1).q;
			m = (y1 - y0) / (2 * pqCollection[line_i].q);
			b = y0 - m*(pqCollection[line_i].p - pqCollection[line_i].q);
		}
		lines[line_i] = new TF1(Form("line_%d", line_i), Form("%f*x+%f", m, b), -1.1*zmax, 1.1*zmax);
		if (line_i == best) {
			lines[line_i]->SetLineColor(kRed);
			TLatex Tl;
			Tl.SetTextSize(0.033);
			Tl.SetTextColor(kRed);
			Tl.DrawLatexNDC(0.11, 0.85, Form("y = %.2f x + %.2f", m, b));
		}
		else lines[line_i]->SetLineColor(kBlue);
		lines[line_i]->Draw("same");
	}

	c.Print(Form("figs/hitsRetinaTracks_%d_%s.pdf", event, name.c_str()));
}

void drawCircles(HitCollection hitCollection, vector <circle> circleCollection, double R_gen_, unsigned int best, int event) {

	unsigned int hits_n = hitCollection.size();
	TGraph hits_h(hits_n);
	double pt = 0.3 * 3.8 * R_gen_ / 100.;
	hits_h.SetTitle(Form("Hits and retina circles, p_{T} = %.2f GeV", pt));
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

	unsigned int circles_size = circleCollection.size();
	vector <TEllipse*> circle_f;
	circle_f.resize(circles_size);
	for (unsigned int circle_i = 0; circle_i < circles_size; circle_i++) {
		double a = circleCollection[circle_i].a;
		double b = circleCollection[circle_i].b;
		double R = sqrt(a*a + b*b);
		circle_f[circle_i] = new TEllipse(a, b, R);
		circle_f[circle_i]->SetNoEdges();
		if (circle_i == best) {
			circle_f[circle_i]->SetLineColor(kRed);
			TLatex Tl;
			Tl.SetTextSize(0.033);
			Tl.SetTextColor(kRed);
			Tl.DrawLatexNDC(0.11, 0.85, Form("p_{T} = %.2f GeV", R * 0.3 / 100. * 3.8));
		}
		else circle_f[circle_i]->SetLineColor(kBlue);
		circle_f[circle_i]->SetFillStyle(0);
		circle_f[circle_i]->Draw("same");
	}

	c.Print(Form("figs/hitsRetinaCircles_%d.pdf", event));
}

#endif /* PRINT_H_ */
