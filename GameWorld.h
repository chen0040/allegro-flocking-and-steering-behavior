#ifndef _H_GAME_WORLD_H
#define _H_GAME_WORLD_H

#include "BaseEntity.h"
#include "Vehicle.h"
#include "PathS.h"
#include <allegro.h>
#include <winalleg.h>
#include <vector>

class GameWorld
{
public:
	GameWorld();
	virtual ~GameWorld();

public:
	int cxClient() const { return m_iWidth; }
	int cyClient() const { return m_iHeight; }

public:
	virtual void Update(const long& lElapsedTicks);
	virtual void Render(BITMAP* pScreen);
	virtual void HandleInput();

protected:
	void test_seek();
	void test_flee();
	void test_wander();
	void test_pursuit();
	void test_evade();
	void test_obstacle_avoidance();
	void test_hide();
	void test_path_follow();
	void test_flock();
	void test_interpose();

public:
	std::vector<Vehicle*>& get_agents() { return m_agents; }
	std::vector<BaseEntity*>& get_obstacles() { return m_obstacles; }

public:
	void AddObstacle(const VectorS& position, double bounding_radius, int color);

public:
	virtual void MouseLeftButtonDown(const int& iX, const int& iY);

protected:
	void remove_agents();
	void remove_obstacles();

protected:
	int m_iWidth;
	int m_iHeight;

protected:
	std::vector<BaseEntity*> m_obstacles;
	std::vector<Vehicle*> m_agents;
	PathS* m_pPath;
	bool m_bShowPath;
};
#endif