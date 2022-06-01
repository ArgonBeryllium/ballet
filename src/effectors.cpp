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
			float b1m = _b1->getMass(), b2m = _b2->getMass();
			float ms = b1m + b2m;
			v2f n = _b1->pos - _b2->pos;

			if(n.getLengthSquare()<rs*rs)
			{
				n = n.normalised();
				if(!b1m)
				{
					if(!b2m) return;
					_b2->pos = _b1->pos-n*rs;
					return;
				}
				else if(!b2m)
				{
					_b1->pos = _b2->pos+n*rs;
					return;
				}

				v2f c = (_b1->pos + _b2->pos)/2;
				_b1->pos = c + b2m/ms*n*rs;
				_b2->pos = c - b1m/ms*n*rs;
			}
		}
}
