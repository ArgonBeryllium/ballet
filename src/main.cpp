#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include "shitrndr/src/shitrndr.h"
#include "huh/vec2.h"

using v2f = vec2<float>;
struct Ball
{
	v2f pos, p_pos;
	v2f acc;
	float r;

	Ball(const v2f& pos_ = {0,0}, float r_ = 1) : pos(pos_), p_pos(pos), r(r_) {}

	void update(float d)
	{
		v2f dp = pos-p_pos;
		p_pos = pos;
		pos += dp + acc*d*d;
		acc *= 0;
	}

	void accelerate(const v2f& a) { acc += a; }
	void teleport(const v2f& np) { pos = p_pos = np; }
};

struct Tickle
{
	virtual void apply(std::vector<Ball*>& balls, float d) = 0;
};
struct GraviTickle : Tickle
{
	v2f g = {0, 8000};

	void apply(std::vector<Ball*>& balls, float d) override
	{
		for(auto b : balls)
			b->accelerate(g*d);
	}
};
struct LimiTickle : Tickle
{
	v2f origin = {};
	float r = 10;
	void apply(std::vector<Ball*>& balls, float d) override
	{
		for(auto b : balls)
		{
			v2f n = b->pos-origin;
			float rs = (r-b->r);
			if(n.getLengthSquare()>rs*rs)
				b->pos = origin + n.normalised()*rs;
		}
	}
};
struct ColliTickle : Tickle
{
	void apply(std::vector<Ball*>& balls, float d) override
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
};

struct Fondler
{
	std::vector<Ball*> balls;
	std::vector<Tickle*> effectors;
	void update(float d, unsigned int iters = 1)
	{
		for(unsigned int i = 0; i != iters; i++)
		{
			for(auto b : balls)
				b->update(d);
			for(auto e : effectors)
				e->apply(balls, d);
		}
	}
};

inline float frand() { return float(std::rand()) / float(RAND_MAX); }

int PS = 10;
vec2<int> CP = {};
int main()
{
	Fondler world;
	for(auto i = 32; i; i--)
		world.balls.push_back(new Ball(v2f(frand(), frand()), frand()+1));
	world.effectors.push_back(new GraviTickle());
	LimiTickle* lt = new LimiTickle();
	world.effectors.push_back(lt);
	world.effectors.push_back(new ColliTickle);

	using namespace shitrndr;
	init("ballet demo", 480, 480, 1, 0, SDL_RENDERER_PRESENTVSYNC);
	silentDefs();
	bg_col = {5,8,15,255};
	onRender = [&world, lt](double d, double t)
	{
		int xo = WindowProps::getWidth()/2-CP.x;
		int yo = WindowProps::getWidth()/2-CP.y;
		v2f mp = vec2<int>(Input::getMP().x-xo, Input::getMP().y-yo).to<float>()/PS;
		if(Input::getMB(1))
			world.balls[0]->accelerate((mp-world.balls[0]->pos).normalised()*1000);
		if(Input::getMB(3))
			lt->r = mp.getLength();

		world.update(d, 1);

		SetColour({80,80,80});
		FillCircle(lt->origin.x*PS+xo, lt->origin.y*PS+yo, lt->r*PS);
		SetColour({255,150,150});
		for(auto b : world.balls)
			FillCircle(b->pos.x*PS+xo, b->pos.y*PS+yo, b->r*PS);
	};
	onKeyDown = [](const SDL_Keycode& k) { if(k==SDLK_q) std::exit(0); };
	loop();
	return 0;
}
