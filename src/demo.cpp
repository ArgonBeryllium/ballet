#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include "shitrndr/src/shitrndr.h"

#include "ballet.h"

inline float frand() { return float(std::rand()) / float(RAND_MAX); }

constexpr bool FIX_PACE = true;
constexpr float EXPECTED_FPS = 60, FIXED_STEP = 1/EXPECTED_FPS;

int PS = 10;
vec2<int> CP = {};
int main()
{
	World world;
	for(auto i = 2; i; i--)
		world.balls.push_back(new Ball(v2f(frand(), frand()), frand()+1));
	world.balls.push_back(new Ball({0,9}, 5, 0));
	//world.effectors.push_back(new EffGravity());
	world.joints.push_back(new SpringJoint(world.balls[0], world.balls[1], 5, 3));
	world.effectors.push_back(new EffCollision);

	using namespace shitrndr;
	init("ballet demo", 480, 480, 1, 0);
	silentDefs();
	bg_col = {5,8,15,255};
	onRender = [&world](double d, double t)
	{
		int xo = WindowProps::getWidth()/2-CP.x;
		int yo = WindowProps::getWidth()/2-CP.y;
		v2f mp = vec2<int>(Input::getMP().x-xo, Input::getMP().y-yo).to<float>()/PS;
		if(Input::getMB(1))
			world.balls[0]->teleport(mp);
			//world.balls[0]->accelerate((mp-world.balls[0]->pos).normalised()*1000);

		world.update(FIX_PACE?FIXED_STEP:d, 1);

		SetColour({255,150,150});
		for(auto b : world.balls)
			FillCircle(b->pos.x*PS+xo, b->pos.y*PS+yo, b->r*PS);

		if(FIX_PACE && d < FIXED_STEP)
			SDL_Delay(1000*(FIXED_STEP-d));
	};
	onKeyDown = [](const SDL_Keycode& k) { if(k==SDLK_q) std::exit(0); };
	loop();
	return 0;
}
