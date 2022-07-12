#include "effectors.h"
#include "ball.h"
#include <iostream>

void EffGravity::apply(std::vector<Ball *> &balls, float d)
{
	for(auto b : balls)
		b->accelerate(g);
}
void EffCollision::apply(std::vector<Ball *> &balls, float d)
{
	for(auto b1 = balls.begin(); b1 != balls.end(); b1++)
		for(auto b2 = b1; b2 != balls.end(); b2++)
		{
			if(b1==b2) continue;
			auto _b1 = *b1.base();
			auto _b2 = *b2.base();

			const float rs = _b1->r + _b2->r;
			const v2f n = _b1->pos - _b2->pos;
			const float nl = n.getLength();

			if(nl<rs)
			{
				const float b1m = _b1->getMass(), b2m = _b2->getMass();
				const float ms = b1m + b2m;
				if(!b1m)
				{
					if(!b2m) return;
					_b2->pos = _b1->pos-n;
					return;
				}
				else if(!b2m)
				{
					_b1->pos = _b2->pos+n;
					return;
				}

				_b1->pos += b2m/ms*n/nl*(rs-nl);
				_b2->pos -= b1m/ms*n/nl*(rs-nl);
			}
		}
}
