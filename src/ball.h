#pragma once
#include "v2f.h"

struct Ball
{
	v2f pos, p_pos;
	v2f acc;
	float r;
	float inv_mass;
	inline float getMass() const { return 1/inv_mass; }
	inline void setMass(float m) { inv_mass = m?1/m:0; }

	Ball(const v2f& pos_ = {0,0}, float r_ = 1, float mass_ = 1, const v2f& iv = {0,0}) :
		pos(pos_), p_pos(pos-iv), r(r_), inv_mass(mass_?1/mass_:0) {}

	v2f getVel() const;

	void update(float d);
	void accelerate(const v2f& a);
	void teleport(const v2f& np);
};
