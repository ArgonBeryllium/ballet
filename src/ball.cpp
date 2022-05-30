#include "ball.h"

v2f Ball::getVel() const { return pos-p_pos; }

void Ball::update(float d)
{
	v2f vel = getVel();
	p_pos = pos;
	pos += vel + acc*d*d/2;
	acc *= 0;
}
void Ball::accelerate(const v2f &a) { acc += a; }
void Ball::teleport(const v2f &np) { pos = p_pos = np; }
