#include "BoidManager.h"
#include "Boid.h"
#include <ctime>
#include <iostream>

BoidManager::BoidManager(int _boidCount, std::string _boidModelName, ID3D11Device * _pd3dDevice, IEffectFactory* _EF)
{
	srand(unsigned int(time(NULL)));

	for (int i = 0; i < _boidCount; i++)
	{
		m_Boids.push_back(new Boid(_boidModelName, _pd3dDevice, _EF));
	}
}

BoidManager::~BoidManager()
{
	for( auto& boid : m_Boids)
	{
		delete boid;
	}
}

void BoidManager::Tick(GameData * _GD)
{
	Vector3 forwardMove = Vector3::Forward;
	float randX = rand() % 80 - 40;
	float randY = rand() % 80 - 40;

	//if (_GD->m_dt * 0.8 > ((float)rand() / (float)RAND_MAX))
	//{
		for (auto& boid : m_Boids)
		{
			if (!boid->isAlive())
			{
				boid->Spawn({ (float)(rand() % 80) - 40, (float)(rand() % 20) - 10, (float)(rand() % 80) - 40 });
				boid->SetVelocity(Vector3::Zero);
				boids_spawned++;
				break;
			}
		}
	//}

	for (auto& boid : m_Boids)
	{
		if (boid->isAlive())
		{
			//boid->addPos(Vector3::Forward / 10);

			if (boids_spawned > 1)
			{		
				/*boid->SetTarget(ApplyRules());
				boid->SetDirection((boid)->GetTarget() - (boid)->GetPos());
				boid->GetDirection().Normalize();*/
				ApplyRules();
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

	for (auto& boid : m_Boids)
	{
		if (boids_spawned > 1)
		{
			if (boid != _boid && boid->isAlive())
			{
				if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < 20.0f))
				{
					_center_of_mass += boid->GetPos();
					nearby_boids++;
				}
			}
		}
	}

	if (nearby_boids > 0)
	{
		_center_of_mass /= (nearby_boids);
		_cohesion = (_center_of_mass - _boid->GetPos());
	}

	return _cohesion;
}

Vector3 BoidManager::Separation(Boid * _boid)
{
	Vector3 _separation = Vector3::Zero;
	for (auto& boid : m_Boids)
	{
		if (boid != _boid)
		{
			if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < 7.5f))
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
		if (boid != _boid)
		{
			_alignment += boid->GetVelocity();
		}
	}
	_alignment /= (m_Boids.size() - 1);
	return (_alignment - _boid->GetVelocity());
}

Vector3 BoidManager::Bind_Position(Boid * _boid)
{
	int Xmin = -80, Xmax = 80, Ymin = - 30, Ymax = 30, Zmin = -80, Zmax = 80;
	Vector3 position;

	if (_boid->GetPos().x < Xmin)
	{
		position.x += 10;
	}
	else if (_boid->GetPos().x > Xmax)
	{
		position.x -= 10;
	}

	if (_boid->GetPos().y < Ymin)
	{
		position.y += 10;
	}
	else if (_boid->GetPos().y > Ymax)
	{
		position.y -= 10;
	}

	if (_boid->GetPos().z < Zmin)
	{
		position.z += 10;
	}
	else if (_boid->GetPos().z > Zmax)
	{
		position.z -= 10;
	}

	return position;
}

void BoidManager::Limit_Speed(Boid * _boid)
{
	float limit = 0.1f;

	if ((fabs(_boid->GetVelocity().x) + fabs(_boid->GetVelocity().y) + fabs(_boid->GetVelocity().z) > limit))
	{
		_boid->SetVelocity((_boid->GetVelocity() / (fabs(_boid->GetVelocity().x) + fabs(_boid->GetVelocity().y) + fabs(_boid->GetVelocity().z))) * limit);
	}
}

void BoidManager::ApplyRules()
{
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 v4;

	for (auto& boid : m_Boids)
	{
		v1 = Cohesion(boid) / 30000;
		v2 = Separation(boid) / 100;
		v3 = Alignment(boid) / 25;
		v3 = Bind_Position(boid);
		boid->SetVelocity(boid->GetVelocity() + v1 + v2 + v3 + v4);
		Limit_Speed(boid);
		boid->addPos(boid->GetVelocity());
	}
}
