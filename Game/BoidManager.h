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
	BoidManager(int _boidCount, int _enemyCount, int _size, ID3D11Device* _pd3dDevice);
	~BoidManager();

	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;

	Vector3 Cohesion(Boid* _boid);
	Vector3 Separation(Boid* _boid);
	Vector3 Alignment(Boid* _boid);
	Vector3 Bind_Position(Boid* _boid);
	Vector3 Scatter(Boid* _boid);
	Vector3 To_Location(Boid* _boid);
	void Limit_Speed(Boid* _boid);
	void ApplyRules(GameData* _GD);
	Boid* getBoid(int _index) { return m_Boids[_index]; }
	float* get_cohesion_prey_mod();
	float* get_separation_prey_mod();
	float* get_alignment_prey_mod();
	float* get_scatter_mod();
	float* get_cohesion_prey_radius();
	float* get_separation_prey_radius();
	float* get_scatter_radius();
	float* get_prey_speed_limit();

	float* get_cohesion_pred_mod();
	float* get_separation_pred_mod();
	float* get_alignment_pred_mod();
	float* get_cohesion_pred_radius();
	float* get_separation_pred_radius();
	float* get_pred_speed_limit();

	int* get_boids_spawned();

	std::vector<Boid*> getBoids() { return m_Boids;  }
private:
	std::vector<Boid*> m_Boids;
	int boids_spawned = 0;
	float cohesion_prey_modifier = 700.0f;
	float separation_prey_modifier = 200.0f;
	float scatter_modifier = 800.0f;
	float alignment_prey_modifier = 22.5f;
	float cohesion_prey_radius = 7.5f;
	float separation_prey_radius = 3.5;
	float scatter_radius = 25.0f;
	float prey_speed_limit = 0.02;

	float cohesion_predator_modifier = 7.0f;
	float separation_predator_modifier = 2.0f;
	float alignment_predator_modifier = 1.5f;
	float cohesion_predator_radius = 1.0f;
	float separation_predator_radius = 1.0f;
	float predator_speed_limit = 0.02;

	int enemy_count = 0;
};