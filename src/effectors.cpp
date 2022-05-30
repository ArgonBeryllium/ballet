#include "effectors.h"
#include "ball.h"

void EffGravity::apply(std::vector<Ball *> &balls, float d)
{
	for(auto b : balls)
		b->accelerate(g*d);
}
void EffCollision::apply(std::vector<Ball *> &balls, float d)
{
	for(auto b1 = balls.begin(); b1 != balls.end(); b1++)
		for(auto b2 = b1; b2 != balls.end(); b2++)
		{
			if(b1==b2) continue;
			auto _b1 = *b1.base();
			auto _b2 = *b2.base();

			float rs = _b1->r + _b2->r;
			float ms = _b1->mass + _b2->mass;
			v2f n = _b1->pos - _b2->pos;

			if(n.getLengthSquare()<rs*rs)
			{
				n = n.normalised();
				if(!_b1->mass)
				{
					if(!_b2->mass) return;
					_b2->pos = _b1->pos-n*rs;
					return;
				}
				else if(!_b2->mass)
				{
					_b1->pos = _b2->pos+n*rs;
					return;
				}

				v2f c = (_b1->pos + _b2->pos)/2;
				_b1->pos = c + _b2->mass/ms*n*rs;
				_b2->pos = c - _b1->mass/ms*n*rs;
			}
		}
}
