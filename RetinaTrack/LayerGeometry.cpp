/*
 * Geometry.cpp
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 */

#include "LayerGeometry.h"

LayerGeometry::LayerGeometry() {
	// TODO Auto-generated constructor stub
	setLayersR();
	setLayersPlain();
	setLayersCylinder();
}

LayerGeometry::~LayerGeometry() {
	// TODO Auto-generated destructor stub
}

void LayerGeometry::setLayersR() {
	barrel_layer_r.push_back(23.0);
	barrel_layer_r.push_back(35.7);
	barrel_layer_r.push_back(50.8);
	barrel_layer_r.push_back(68.6);
	barrel_layer_r.push_back(88.8);
	barrel_layer_r.push_back(108.0);
}

void LayerGeometry::setLayersCylinder() {
	unsigned int size = barrel_layer_r.size();
	for (unsigned int i = 0; i < size; i++) {
		circle c;
		c.a = 0.;
		c.b = 0.;
		c.R = barrel_layer_r[i];
		barrel_layers_cylinder.push_back(c);
	}
}

void LayerGeometry::setLayersPlain() {
	unsigned int size = barrel_layer_r.size();
	for (unsigned int i = 0; i < size; i++) {
		line l;
		l.p = 0.;
		l.q = barrel_layer_r[i];
		barrel_layers_plain.push_back(l);
	}
}
