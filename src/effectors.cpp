#include "effectors.h"
#include "ball.h"

void EffGravity::apply(std::vector<Ball *> &balls, float d)
{
	for(auto b : balls)
		b->accelerate(g*d);
}
void EffCollision::apply(std::vector<Ball *> &balls, float d)
{
	for(auto b = balls.begin(); b != balls.end(); b++)
		for(auto b2 = b; b2 != balls.end(); b2++)
		{
			if(b==b2) continue;
			auto _b = *b.base();
			auto _b2 = *b2.base();
			float rs = _b->r+_b2->r;
			v2f n = _b->pos - _b2->pos;
			v2f c = (_b->pos + _b2->pos)/2;
			if(n.getLengthSquare()<rs*rs)
			{
				n = n.normalised();
				_b->pos = c + n*rs/2;
				_b2->pos = c - n*rs/2;
			}
		}
}
void EffSpring::apply(std::vector<Ball *> &balls, float d)
{
	v2f pd = b->pos-a->pos;
	float pdl = pd.getLength();
	float ld = pdl-target_length;
	v2f t = (pd/pdl)*ld*force;
	a->pos += t*d;
	b->pos -= t*d;
}
