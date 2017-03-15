#pragma once
#include <vector>
#include <string>
#include "GameData.h"
#include "gameobject.h"
#include "Boid.h"
#include "Model.h"

class BoidManager : public GameObject
{
public:
	BoidManager(int _boidCount, std::string _boidModelName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	~BoidManager();

	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;

	Vector3 Cohesion(Boid* _boid);
	Vector3 Separation(Boid* _boid);
	Vector3 Alignment(Boid* _boid);
	Vector3 Bind_Position(Boid* _boid);
	Vector3 To_Location(Boid* _boid);
	void Limit_Speed(Boid* _boid);
	void ApplyRules(GameData* _GD);
	Boid* getBoid(int _index) { return m_Boids[_index]; }
	float* get_cohesion_mod();
	float* get_separation_mod();
	float* get_alignment_mod();
	float* get_cohesion_radius();
	float* get_separation_radius();
	float* get_speed_limit();
	int* get_boids_spawned();

	std::vector<Boid*> getBoids() { return m_Boids;  }
private:
	std::vector<Boid*> m_Boids;
	int boids_spawned = 0;
	float cohesion_modifier = 15000.0f;
	float separation_modifier = 200.0f;
	float alignment_modifier = 22.5f;
	float cohesion_radius = 12.5f;
	float separation_radius = 3.5;
	float speed_limit = 0.04;
};