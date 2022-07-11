#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include "joints.h"
#include "shitrndr/src/shitrndr.h"

#include "ballet.h"
#define DEBUG_TEXT
#include "utils.h"

constexpr bool FIX_PACE = true;
constexpr float EXPECTED_FPS = 60, FIXED_STEP = 1/EXPECTED_FPS;

int PS = 10;
v2f CP = {0,0};
inline vec2<int> toScreen(const v2f& p)
{
	using namespace shitrndr;
	int xo = WindowProps::getWidth()/2. - CP.x*PS;
	int yo = WindowProps::getHeight()/2. - CP.y*PS;
	return (p*PS+vec2<int>(xo, yo)).to<int>();
}
inline v2f toSpace(const vec2<int>& p)
{
	using namespace shitrndr;
	int xo = WindowProps::getWidth()/2. - CP.x*PS;
	int yo = WindowProps::getHeight()/2. - CP.y*PS;
	return (p-vec2<int>(xo, yo)).to<float>()/PS;
}

struct Edge
{
	v2f a, b;
	float r = 0;
	float restitution = .5;
	v2f dif() const { return b-a; }

	void collide(Ball* ball)
	{
		v2f ba = dif();
		v2f pa = ball->pos-a;
		float h = v2f::dot(pa,ba)/v2f::dot(ba,ba);
		h = std::min(1.f, std::max(0.f, h));
		v2f disp = pa - h*ba;
		float dl = disp.getLength();

		if(dl-r >= ball->r) return;

		const v2f d = disp/dl * (ball->r - dl+r);

		ball->teleport(ball->pos + d*(1-restitution));
		ball->pos += d*restitution;
	}
};
struct EffEdge : Effector
{
	std::vector<Edge> edges;
	void apply(std::vector<Ball*>& balls, float d) override
	{
		for(auto e : edges)
			for(auto b : balls)
				e.collide(b);
	}
};

std::vector<Ball*> genMesh(World* w, const std::vector<v2f>& verts, const v2f& pos = {})
{
	std::vector<Ball*> balls;
	for(auto v : verts)
	{
		Ball* b = new Ball(pos+v, .5, .5);
		w->balls.push_back(b);
		for(Ball* b_ : balls)
			w->joints.push_back(new StiffJoint{b, b_, (b->pos-b_->pos).getLength()});
		balls.push_back(b);
	}
	return balls;
}
std::vector<Ball*> genGon(World* w, size_t n = 4, const v2f& pos = {}, float s = 5)
{
	std::vector<v2f> verts;
	for(float i = 0; i != n; i++)
	{
		float a = i/n*M_PI*2+M_PI_2;
		verts.push_back({std::cos(a)*s, std::sin(a)*s});
	}
	return genMesh(w, verts, pos);
}
std::vector<Ball*> genBlock(World* w, float s = 1, const v2f& pos = {}, const v2f& dim = {5,5}, float mass = 1)
{
	std::vector<Ball*> balls;
	const float m = mass/(dim.x*dim.y/(s*s));
	const int sw = dim.x/s;
	const int sh = dim.y/s;
	int i = 0;
	for(float y = 0; y < sh; y++)
	{
		for(float x = 0; x < sw; x++)
		{
			Ball* b = new Ball(pos+v2f(x,y)*s*2.1, s, m);
			w->balls.push_back(b);
			balls.push_back(b);
			const int offsets[4] {-sw-1, -sw, -sw+1, -1};
			for(int k = 0; k != 4; k++)
			{
				int j = i+offsets[k];
				if(j<0) continue;
				if(x==0 && (k==3 || k==0)) continue;
				if(x==sw-1 && k==2) continue;
				Ball* b_ = balls[j];
				w->joints.push_back(new StiffJoint{b, b_, (b->pos-b_->pos).getLength()});
			}
			i++;
		}
	}
	return balls;
}

Ball* selb;
int main()
{
	World world;
	world.effectors.push_back(new EffGravity);
	world.effectors.push_back(new EffCollision);
	auto ee = new EffEdge;
	world.effectors.push_back(ee);
	ee->edges.push_back({{-3, 27}, {3, 27}, 2});
	constexpr float a = 64, t = 32;
	ee->edges.push_back({{-a,-a}, {a,-a}, t});
	ee->edges.push_back({{-a,a}, {a,a}, t});
	ee->edges.push_back({{-a,-a}, {-a,a}, t});
	ee->edges.push_back({{a,-a}, {a,a}, t});
	Edge& e = ee->edges[0];

	/*
	for(int i = 0; i != 12; i++)
	{
		Ball* b = new Ball(v2f{0, i*1.5f});
		world.balls.push_back(b);
		if(i) world.joints.push_back(new StiffJoint(b, world.balls[i-1], 2.5));
	}
	for(auto i = 0; i != 5; i++)
		genGon(&world, std::rand()%3+3, {i*4.f-15, -2}, std::rand()%3+2);
	*/
	genBlock(&world, 1, {}, {10, 5});

	using namespace shitrndr;
	init("ballet demo", 720, 720, 1, 0);
	silentDefs();
	bg_col = {5,8,15,255};
	onRender = [&world, &e, ee](double d, double t)
	{
		clearDebugLines();
		printDebugFPS(t);
		v2f mp = toSpace(Input::getMP());
		if(Input::getKey(SDLK_LSHIFT))
		{
			if(Input::getMB(1)) e.a = mp;
			if(Input::getMB(3)) e.b = mp;
		}
		else if(Input::getMB(1))
				selb->pos = lerp(selb->pos, mp, 5*d);
		else
		{
			float md = MAXFLOAT;
			for(auto b : world.balls)
			{
				float d = (mp-b->pos).getLengthSquare();
				if(d>md) continue;
				selb = b;
				md = d;
			}
		}

		world.update(FIX_PACE?FIXED_STEP:d, 4);

		SetColour({255,150,150,255});
		for(auto b : world.balls)
		{
			vec2<int> bp = toScreen(b->pos);
			FillCircle(bp.x, bp.y, b->r*PS);
		}
		if(selb)
		{
			SetColour({150,150,255,255});
			vec2<int> bp = toScreen(selb->pos);
			FillCircle(bp.x, bp.y, selb->r*PS);
		}
		
		for(auto j : world.joints)
		{
			auto sj = dynamic_cast<StiffJoint*>(j);
			if(!sj) continue;
			vec2<int> pa = toScreen(sj->a->pos);
			vec2<int> pb = toScreen(sj->b->pos);
			SetColour({255,0,255,150});
			SDL_RenderDrawLine(ren, pa.x, pa.y, pb.x, pb.y);

			v2f m = (sj->a->pos + sj->b->pos)/2;
			v2f n = (sj->a->pos - sj->b->pos).normalised(), t = {-n.y, n.x};
			pa = toScreen(m-n*sj->target_length/2)+t*10;
			pb = toScreen(m+n*sj->target_length/2)+t*10;

			SetColour({255,255,0,150});
			SDL_RenderDrawLine(ren, pa.x, pa.y, pb.x, pb.y);
		}
		for(auto e_ : ee->edges)
		{
			auto ea = toScreen(e_.a);
			auto eb = toScreen(e_.b);
			SetColour({255,255,255,255});
			DrawCircle(ea.x, ea.y, e_.r*PS);
			DrawCircle(eb.x, eb.y, e_.r*PS);
			DrawLine(ea.x, ea.y, eb.x, eb.y);
			auto o = e_.a-e_.b;
			o = o.normalised();
			o = {-o.y, o.x};
			o = o*e_.r*PS;
			DrawLine(ea.x+o.x, ea.y+o.y, eb.x+o.x, eb.y+o.y);
			DrawLine(ea.x-o.x, ea.y-o.y, eb.x-o.x, eb.y-o.y);
		}

		printDebugLine("balls: "+std::to_string(world.balls.size()));
		printDebugLine("joints: "+std::to_string(world.joints.size()));
		if(FIX_PACE && d < FIXED_STEP)
			SDL_Delay(1000*(FIXED_STEP-d));
	};
	onKeyDown = [&world](const SDL_Keycode& key)
	{
		if(key==SDLK_q) std::exit(0);
		if(key==SDLK_SPACE)
			genBlock(&world, 1, toSpace(Input::getMP()));
	};
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	loop();
	return 0;
}
