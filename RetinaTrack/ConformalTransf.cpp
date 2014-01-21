/*
 * ConformalTransf.cpp
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 */

#include "ConformalTransf.h"

ConformalTransf::ConformalTransf(HitCollection hitCollection_) : hitCollection(hitCollection_) {
}

ConformalTransf::~ConformalTransf() {
}

void ConformalTransf::from_cart_to_polar(bool do_it) {
	if (!do_it) return;
	unsigned int hits_n = hitCollection.size();
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		double x =  hitcollref->at(hit_i).x;
		double y =  hitcollref->at(hit_i).y;
		double u = get_u(x, y);
		double v = get_v(x, y);
		hit hitConf_i;
		hitConf_i.x = u;
		hitConf_i.y = v;
		hitCollection.addHit(hitConf_i);
	}
}
