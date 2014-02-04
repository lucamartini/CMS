/*
 * CommonFuncs.cpp
 *
 *  Created on: 13/gen/2014
 *      Author: lucamartini
 */
#include "CommonFuncs.h"

double pi = acos(-1);

void DrawCanvas(TH1D h) {
	TCanvas c("c", "c", 600, 600);
	h.Draw();
	c.Print(Form("figs/%s.pdf", h.GetName()));
}

void DrawCanvas2(TH2D h) {
	gStyle->SetPalette(54);
	TCanvas c("c", "c", 600, 600);
	h.SetStats(false);
	h.Draw("COLZ");
	c.Print(Form("figs/%s.pdf", h.GetName()));
}

