#include "ball.h"

void Ball::update(float d)
{
	v2f dp = pos-p_pos;
	p_pos = pos;
	pos += dp + acc*d*d/2;
	acc *= 0;
}
void Ball::accelerate(const v2f &a) { acc += a; }
void Ball::teleport(const v2f &np) { pos = p_pos = np; }
