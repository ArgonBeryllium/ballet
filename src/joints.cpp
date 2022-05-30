#include "joints.h"
#include "ball.h"

void SpringJoint::apply(float delta)
{
	v2f pd = b->pos-a->pos;
	float pdl = pd.getLength();
	float ld = pdl-target_length;
	v2f t = (pd/pdl)*ld*stiffness;
	a->pos += t*delta/a->mass;
	b->pos -= t*delta/b->mass;
}
