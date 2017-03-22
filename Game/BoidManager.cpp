#include "BoidManager.h"
#include <ctime>
#include <iostream>

BoidManager::BoidManager(int _boidCount, int _enemyCount, int _size, ID3D11Device * _pd3dDevice)
{
	boids_spawned = 0;
	enemy_count = _enemyCount;

	for (int i = 0; i < _boidCount; i++)
	{
		m_Boids.push_back(new Boid(_size, _pd3dDevice));
	}

	if (enemy_count > _boidCount)
	{
		enemy_count = (_boidCount / 30) + 1;
	}

	for (int i = 0; i < enemy_count; i++)
	{
		m_Boids[i]->setPredator(true);
		m_Boids[i]->SetScale(3.0f);
	}
}

BoidManager::~BoidManager()
{
	//for( auto& boid : m_Boids)
	//{
	//	delete boid;
	//}
}

void BoidManager::Tick(GameData * _GD)
{
	for (auto& boid : m_Boids)
	{
		if (!boid->isAlive())
		{
			boid->Spawn({ (float)(rand() % 50) - 50, (float)(rand() % 50) - 50, (float)(rand() % 40) - 40 });
			boids_spawned++;
			break;
		}

		if (boid->isAlive())
		{
			//boid->addPos(Vector3::Forward / 10);

			if (boids_spawned > 1)
			{
				ApplyRules(_GD);
			}

			(boid)->Tick(_GD);
		}
	}
}

void BoidManager::Draw(DrawData * _DD)
{
	for (auto& boid : m_Boids)
	{
		if (boid->isAlive())
		{
			boid->Draw(_DD);
		}
	}
}

Vector3 BoidManager::Cohesion(Boid* _boid)
{
	Vector3 _cohesion = Vector3::Zero;
	Vector3 _center_of_mass = Vector3::Zero;

	int nearby_boids = 0;
	float radius = 0.0f;

	for (auto& boid : m_Boids)
	{
		if (boids_spawned > 1)
		{
			if (boid != _boid && boid->isPredator() == false)
			{
				if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < cohesion_prey_radius))
				{
					_center_of_mass += boid->GetPos();
					nearby_boids++;
				}
			}
			if (boid != _boid && boid->isPredator() == true)
			{
				if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < cohesion_predator_radius))
				{
					_center_of_mass += boid->GetPos();
				}
			}
		}
	}

	if (_boid->isPredator() == false)
	{
		if (nearby_boids > 0)
		{
			_center_of_mass /= (nearby_boids);
			_cohesion = (_center_of_mass - _boid->GetPos());
		}
	}

	return _cohesion;
}

Vector3 BoidManager::Separation(Boid * _boid)
{
	Vector3 _separation = Vector3::Zero;
	float radius = 0.0f;

	for (auto& boid : m_Boids)
	{
		if (boid != _boid && boid->isPredator() == false)
		{
			if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < separation_prey_radius))
			{
				_separation -= (boid->GetPos() - _boid->GetPos());
			}
		}
		if (boid != _boid && boid->isPredator() == true)
		{
			if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < separation_predator_radius))
			{
				_separation -= (boid->GetPos() - _boid->GetPos());
			}
		}

	}
	return _separation;
}

Vector3 BoidManager::Alignment(Boid * _boid)
{
	Vector3 _alignment;
	for (auto& boid : m_Boids)
	{
		if (boid != _boid && boid->isPredator() == false)
		{
			if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < alignment_prey_radius))
			{
				_alignment += boid->GetVelocity();
			}
		}
		if (boid != _boid && boid->isPredator() == true)
		{
			if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < alignment_predator_radius))
			{
				_alignment += boid->GetVelocity();
			}
		}
	}
	_alignment /= (m_Boids.size() - 1);
	return (_alignment - _boid->GetVelocity());	
}

Vector3 BoidManager::Bind_Position(Boid * _boid)
{
	int Xmin = -80, Xmax = 80, Ymin = - 40, Ymax = 40, Zmin = -20, Zmax = 20;
	float xBoundary = 10.0f;
	float yBoundary = 10.0f;
	float zBoundary = 10.0f;
	Vector3 position;

	if (_boid->GetPos().x < Xmin)
	{
		position.x += xBoundary;
	}
	else if (_boid->GetPos().x > Xmax)
	{
		position.x -= xBoundary;
	}

	if (_boid->GetPos().y < Ymin)
	{
		position.y += yBoundary;
	}
	else if (_boid->GetPos().y > Ymax)
	{
		position.y -= yBoundary;
	}

	if (_boid->GetPos().z < Zmin)
	{
		position.z += zBoundary;
	}
	else if (_boid->GetPos().z > Zmax)
	{
		position.z -= zBoundary;
	}
	return position;
}

Vector3 BoidManager::Scatter(Boid * _boid)
{
	Vector3 _scatter = Vector3::Zero;

	for (auto& boid : m_Boids)
	{
		if (boid != _boid && boid->isPredator())
		{
			if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < scatter_radius))
			{
				_scatter -= (boid->GetPos() - _boid->GetPos());
			}
		}

	}
	return _scatter;
}

void BoidManager::Limit_Speed(Boid * _boid)
{
	float limit = 0.0f;

	if (_boid->isPredator() == true)
	{
		limit = predator_speed_limit;
	}
	else if(_boid->isPredator() == false)
	{
		limit = prey_speed_limit;
	}

	if ((fabs(_boid->GetVelocity().x) + fabs(_boid->GetVelocity().y) + fabs(_boid->GetVelocity().z) > limit))
	{
		_boid->SetVelocity((_boid->GetVelocity() / (fabs(_boid->GetVelocity().x) + fabs(_boid->GetVelocity().y) + fabs(_boid->GetVelocity().z))) * limit);
	}
}

void BoidManager::ApplyRules(GameData* _GD)
{
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 v4;
	Vector3 v5;

	for (auto& boid : m_Boids)
	{
			v4 = Bind_Position(boid);
			if (boid->isPredator() == true)
			{
				v1 = (Cohesion(boid) / cohesion_predator_modifier) * _GD->m_dt;
				v2 = (Separation(boid) / separation_predator_modifier) * _GD->m_dt;
				v3 = (Alignment(boid) / alignment_predator_modifier) * _GD->m_dt;
				boid->SetVelocity(boid->GetVelocity() + v1 + v2 + v3 + v4);
			}
			else if (boid->isPredator() == false)
			{
				v1 = (Cohesion(boid) / cohesion_prey_modifier) * _GD->m_dt;
				v2 = (Separation(boid) / separation_prey_modifier) * _GD->m_dt;
				v3 = (Alignment(boid) / alignment_prey_modifier) * _GD->m_dt;
				v5 = (Scatter(boid) / scatter_modifier) * _GD->m_dt;
				boid->SetVelocity(boid->GetVelocity() + v1 + v2 + v3 + v4 + v5);
			}
			Limit_Speed(boid);
			boid->addPos(boid->GetVelocity());
	}
}

float* BoidManager::get_cohesion_prey_mod()
{
	return &cohesion_prey_modifier;
}

float * BoidManager::get_separation_prey_mod()
{
	return &separation_prey_modifier;
}

float * BoidManager::get_alignment_prey_mod()
{
	return &alignment_prey_modifier;
}

float * BoidManager::get_scatter_mod()
{
	return &scatter_modifier;
}

float * BoidManager::get_cohesion_prey_radius()
{
	return &cohesion_prey_radius;
}

float * BoidManager::get_separation_prey_radius()
{
	return &separation_prey_radius;
}

float * BoidManager::get_alignment_prey_radius()
{
	return &alignment_prey_radius;
}

float * BoidManager::get_scatter_radius()
{
	return &scatter_radius;
}

float * BoidManager::get_prey_speed_limit()
{
	return &prey_speed_limit;
}

float * BoidManager::get_cohesion_pred_mod()
{
	return &cohesion_predator_modifier;
}

float * BoidManager::get_separation_pred_mod()
{
	return &separation_predator_modifier;
}

float * BoidManager::get_alignment_pred_mod()
{
	return &alignment_predator_modifier;
}

float * BoidManager::get_cohesion_pred_radius()
{
	return &cohesion_predator_radius;
}

float * BoidManager::get_separation_pred_radius()
{
	return &separation_predator_radius;
}

float * BoidManager::get_alignment_pred_radius()
{
	return &alignment_predator_radius;
}

float * BoidManager::get_pred_speed_limit()
{
	return &predator_speed_limit;
}

int* BoidManager::get_boids_spawned()
{
	return &boids_spawned;
}
