#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include "joints.h"
#include "shitrndr/src/shitrndr.h"

#include "ballet.h"

inline float frand() { return float(std::rand()) / float(RAND_MAX); }

constexpr bool FIX_PACE = true;
constexpr float EXPECTED_FPS = 60, FIXED_STEP = 1/EXPECTED_FPS;

int PS = 10;
v2f CP = {0,0};
inline vec2<int> toScreen(const v2f& p)
{
	using namespace shitrndr;
	int xo = WindowProps::getWidth()/2 - CP.x*PS;
	int yo = WindowProps::getHeight()/2 - CP.y*PS;
	return (p*PS+vec2<int>(xo, yo)).to<int>();
}
inline v2f toSpace(const vec2<int>& p)
{
	using namespace shitrndr;
	int xo = WindowProps::getWidth()/2 - CP.x*PS;
	int yo = WindowProps::getHeight()/2 - CP.y*PS;
	return (p-vec2<int>(xo, yo)).to<float>()/PS;
}

int main()
{
	World world;
	world.effectors.push_back(new EffGravity);
	world.effectors.push_back(new EffCollision);

	for(int i = 0; i != 12; i++)
	{
		Ball* b = new Ball(v2f{0, i*1.5f});
		world.balls.push_back(b);
		if(i) world.joints.push_back(new SpringJoint(b, world.balls[i-1], 2, 20));
	}

	using namespace shitrndr;
	init("ballet demo", 480, 480, 1, 0);
	silentDefs();
	bg_col = {5,8,15,255};
	onRender = [&world](double d, double t)
	{
		v2f mp = toSpace(Input::getMP());
		world.balls[0]->teleport({std::sin(world.time), -2});
		if(Input::getMB(1))
			world.balls[world.balls.size()-1]->teleport(mp);
			//world.balls[0]->accelerate((mp-world.balls[0]->pos).normalised()*1000);

		world.update(FIX_PACE?FIXED_STEP:d, 1);

		SetColour({255,150,150,255});
		for(auto b : world.balls)
		{
			vec2<int> bp = toScreen(b->pos);
			FillCircle(bp.x, bp.y, b->r*PS);
		}
		
		for(auto j : world.joints)
		{
			auto sj = dynamic_cast<SpringJoint*>(j);
			if(!j) continue;
			vec2<int> pa = toScreen(sj->a->pos);
			vec2<int> pb = toScreen(sj->b->pos);
			SetColour({255,0,255,150});
			SDL_RenderDrawLine(ren, pa.x, pa.y, pb.x, pb.y);

			v2f m = (sj->a->pos + sj->b->pos)/2;
			v2f n = (sj->a->pos - sj->b->pos).normalised(), t = {n.y, n.x};
			pa = toScreen(m-n*sj->target_length/2)+t*10;
			pb = toScreen(m+n*sj->target_length/2)+t*10;

			SetColour({255,255,0,150});
			SDL_RenderDrawLine(ren, pa.x, pa.y, pb.x, pb.y);
		}

		if(FIX_PACE && d < FIXED_STEP)
			SDL_Delay(1000*(FIXED_STEP-d));
	};
	onKeyDown = [](const SDL_Keycode& k) { if(k==SDLK_q) std::exit(0); };
	loop();
	return 0;
}
