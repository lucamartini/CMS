/*
 * ConformalTransf.cpp
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 */

#include "ConformalTransf.h"

ConformalTransf::ConformalTransf() : normHitCollection("norm"), confHitCollection("conf") {
}

ConformalTransf::~ConformalTransf() {
}

void ConformalTransf::from_norm_to_conf(bool do_it) {
	if (!do_it) return;
	unsigned int hits_n = normHitCollection.size();
	vector <hit> * hitcollref = normHitCollection.getHitCollectionRef();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		double x =  hitcollref->at(hit_i).x;
		double y =  hitcollref->at(hit_i).y;
		double u = get_u(x, y);
		double v = get_v(x, y);
		hit hitNew_i;
		hitNew_i.x = u;
		hitNew_i.y = v;
		confHitCollection.addHit(hitNew_i);
	}
}

void ConformalTransf::from_conf_to_norm(bool do_it) {
	if (!do_it) return;
	unsigned int hits_n = confHitCollection.size();
	vector <hit> * hitcollref = confHitCollection.getHitCollectionRef();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		double u =  hitcollref->at(hit_i).x;
		double v =  hitcollref->at(hit_i).y;
		double x = get_x(u, v);
		double y = get_y(u, v);
		hit hitConf_i;
		hitConf_i.x = u;
		hitConf_i.y = v;
		normHitCollection.addHit(hitConf_i);
	}
}
