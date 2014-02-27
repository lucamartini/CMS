/*
 * CommonFuncs.cpp
 *
 *  Created on: 13/gen/2014
 *      Author: lucamartini
 */
#include "CommonFuncs.h"

double pi = acos(-1);

void DrawCanvas2(TH2D h, bool fit) {
	gStyle->SetPalette(54);
	TCanvas c("c", "c", 600, 600);
	c.SetRightMargin(0.1346749);
	h.SetStats(false);
	if (fit) {
		TFitResultPtr r = h.Fit("pol1", "S");
		double chi2   = r->Chi2();
		unsigned int ndof = r->Ndf();
		h.Draw("AXIS");
		TF1 * line = new TF1("line", "x", h.GetXaxis()->GetBinLowEdge(1), h.GetXaxis()->GetBinUpEdge(h.GetNbinsX()));
		line->SetLineColor(kGreen);
		line->Draw("same");
		TF1* fun = h.GetFunction("pol1");
		fun->SetParNames("q","p");
		double p = fun->GetParameter("p");
		double q = fun->GetParameter("q");
		TLatex Tl;
		Tl.SetTextSize(0.033);
		Tl.SetTextColor(kRed);
		Tl.DrawLatexNDC(0.11, 0.85, Form("y = %.2f x + %.2f", p, q));
		Tl.DrawLatexNDC(0.11, 0.80, Form("#chi^{2} / ndof = %.2f / %d", chi2, ndof));
		h.Draw("same");
	}
	else {
		h.Draw("COLZ");
	}
	c.Print(Form("figs/%s.pdf", h.GetName()));
}
