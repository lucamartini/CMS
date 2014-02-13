/*
 * CommonFuncs.cpp
 *
 *  Created on: 13/gen/2014
 *      Author: lucamartini
 */
#include "CommonFuncs.h"

double pi = acos(-1);

void DrawCanvas2(TH2D h) {
	gStyle->SetPalette(54);
	TCanvas c("c", "c", 600, 600);
	c.SetRightMargin(0.1346749);
	h.SetStats(false);
	h.Draw("COLZ");
	c.Print(Form("figs/%s.png", h.GetName()));
}
