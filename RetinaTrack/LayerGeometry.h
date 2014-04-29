/*
 * Geometry.h
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 */

#ifndef LAYERGEOMETRY_H_
#define LAYERGEOMETRY_H_

#include <vector>

using namespace std;

struct circle {
	double a;
	double b;
	double R;
};

struct line {
	double p;
	double q;
};

class LayerGeometry {
public:
	LayerGeometry();
	virtual ~LayerGeometry();

	void setLayersR();

	vector <line> get_barrel_layers_plain() {return barrel_layers_plain;};
	vector <circle> get_barrel_layers_cylinder() {return barrel_layers_cylinder;};
	unsigned int get_layer_size() {return barrel_layer_r.size();};
	double get_max_z() {return max_z;};

protected:

private:

	// max z of barrel layers
	double max_z;

	void setLayersPlain();
	void setLayersCylinder();

	vector <line> barrel_layers_plain;
	vector <circle> barrel_layers_cylinder;
	vector <double> barrel_layer_r;

};

#endif /* LAYERGEOMETRY_H_ */
