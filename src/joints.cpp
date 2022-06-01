#include "joints.h"
#include "ball.h"

void SpringJoint::apply(float delta)
{
	v2f pd = b->pos-a->pos;
	float pdl = pd.getLength();
	float ld = pdl-target_length;
	v2f pdn = pd/pdl;

	v2f t = pdn*ld*stiffness;
	a->pos += t*delta*a->inv_mass;
	b->pos -= t*delta*b->inv_mass;
}
