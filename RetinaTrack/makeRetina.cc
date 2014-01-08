/*
 * makeRetina.cc
 *
 *  Created on: 07/gen/2014
 *      Author: lucamartini
 */


#include "RetinaTrackFitter.h"

int main(int argc, char* argv[]) {

	RetinaTrackFitter rtf;
	rtf.setHits();
	rtf.cleanHits(0., 4., 0., 4.);
	rtf.drawHits();
	rtf.setConfHits();
	rtf.drawHitsConf();
	rtf.GetFromConfToCircle();

	rtf.fillPQGrid();
	rtf.drawPQGrid();

	rtf.findMaxima();
	rtf.printMaxima();
	rtf.drawHitsConfRetina();

	rtf.getCircles();
	rtf.drawCircles();

	return 0;
}


