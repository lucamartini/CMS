/*
 * ConformalTransf.h
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 */

#ifndef CONFORMALTRANSF_H_
#define CONFORMALTRANSF_H_

#include "HitsGenerator.h"

class ConformalTransf {
public:
	ConformalTransf();
	virtual ~ConformalTransf();

	void setNormHitCollection(HitCollection hitCollection_) {normHitCollection = hitCollection_;};
	void setConfHitCollection(HitCollection hitCollection_) {confHitCollection = hitCollection_;};
	HitCollection getNormHitCollection() {return normHitCollection;};
	HitCollection getConfHitCollection() {return confHitCollection;};

	void from_norm_to_conf(bool do_it = false);
	void from_conf_to_norm(bool do_it = false);

private:

	inline double get_u(double x, double y) { return x / (x*x + y*y);};
	inline double get_v(double x, double y) { return y / (x*x + y*y);};
	inline double get_x(double u, double v) { return u / (u*u + v*v);};
	inline double get_y(double u, double v) { return v / (u*u + v*v);};

	HitCollection normHitCollection;
	HitCollection confHitCollection;

};

#endif /* CONFORMALTRANSF_H_ */
