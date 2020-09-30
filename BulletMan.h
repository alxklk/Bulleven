#ifndef BULLIMAR_BULLETMAN_H
#define BULLIMAR_BULLETMAN_H

#include "MinMath.h"
#include <vector>
#include <cfloat>
#include <algorithm>
#include <boost/lockfree/queue.hpp>

class CBulletMan
{
	struct Bullet
	{
		float3 pos;
		float3 dir; // speed in 1/s
		float startTime;
		float endTime;
		float nearestHit=-FLT_MAX;
	};

	struct Wall
	{
		float2 end0;
		float2 end1;
		float hitTime;
		int hitBullet;
	};

	std::vector<Bullet>activeBullets;
	boost::lockfree::queue<Bullet>shots{100};
	std::vector<Wall> walls;
	float prevT=0;

public:

	CBulletMan() = default;
	~CBulletMan() = default;

	const auto& GetWalls()const{return walls;}
	const auto& GetBullets()const{return activeBullets;}

	void AddWall(const Wall& w){walls.push_back(w);}


	void Fire(const float3& pos, const float3& dir, float speed, float time, float life_time)
	{
		shots.push({pos, dir * speed, time, time + life_time});
	}

	void Clear()
	{
		Bullet bullet;
		shots.consume_all([](const Bullet& b){});
		activeBullets.clear();
		walls.clear();
		prevT=0;
	}

	void Update(float t)
	{
		shots.consume_all([this](const Bullet& b)
		{
			activeBullets.push_back(b);
		});

		// Remove dead bullets
		activeBullets.erase(
			std::remove_if(
				activeBullets.begin(),
				activeBullets.end(),
				[t](const Bullet& item){return item.endTime<t;}
				),
			activeBullets.end()
		);

		// Repeat this calculation several time because bullet can hit several walls inside current time interval
		// This way of calculation makes possible to use arbitrary long time update interval
		while(true)
		{
			// Prepare walls to register hit(s)
			for(auto& wall: walls)
			{
				wall.hitBullet=-1;
				wall.hitTime=FLT_MAX;
			}

			for(int i=0;i<activeBullets.size();i++)
			{
				auto& b=activeBullets[i];
				// Filter bullets by lifespan and already calculated intersection time
				// As b.nearestHit caches previous found intersection, this nested loop
				// rarely runs. And so, overall complexity is much lower than O(Nw x Nb)
				if((b.nearestHit<=t)&&(b.startTime<t)&&(b.endTime>prevT))
				{
					float newNearestHitT=FLT_MAX;
					int nearestWallIdx=-1;
					for(int j=0;j<walls.size();j++)
					{
						const auto& w=walls[j];
						float hitT;
						if(IntersectRaySection(
							b.pos.xy(),b.pos.xy()+b.dir.xy()*(b.endTime-b.startTime),
							w.end0,w.end1,
							hitT))
						{
							float hitRT=b.startTime+(b.endTime-b.startTime)*hitT;
							if((hitRT<t)&&(newNearestHitT>hitRT))
							{
								newNearestHitT=hitRT;
								nearestWallIdx=j;
							}
						}
					}
					if(nearestWallIdx!=-1)
					{
						b.nearestHit=newNearestHitT;
						auto& w=walls[nearestWallIdx];
						if(newNearestHitT<w.hitTime)
						{
							w.hitTime=newNearestHitT;
							w.hitBullet=i;
						}
					}
				}
			}

			bool noHits=true;
			for(auto& w : walls)
			{
				if(w.hitBullet==-1)
				{
					continue;
				}
				Bullet& b=activeBullets[w.hitBullet];
				b.pos=b.pos+b.dir*(w.hitTime-b.startTime);
				b.startTime=w.hitTime;
				float2 reflDir=reflect(b.dir.xy(),(w.end1-w.end0).norm());
				b.dir.x=reflDir.x;
				b.dir.y=reflDir.y;
				noHits=false;
			}

			// If no hits registered for current time interval, it means that no further 
			// changes in scene configuration expected and thus we do not need to remove hit walls and
			// no bullets were reflected.
			if(noHits)
				break;

			// Removing hit walls
			walls.erase(
				std::remove_if(
					walls.begin(),
					walls.end(),
					[](const Wall& w){return w.hitBullet!=-1;}
					),
				walls.end()
			);
		}
		prevT=t;
	}
};

#endif //BULLIMAR_BULLETMAN_H
