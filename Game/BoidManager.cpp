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

	if (_GD->m_dt * 0.8 > ((float)rand() / (float)RAND_MAX))
	{
		for (auto& boid : m_Boids)
		{
			if (!boid->isAlive())
			{
				boid->Spawn({ (float)(rand() % 80) - 40, 0, (float)(rand() % 80) - 40 });
				boidsSpawned++;
				std::cout << ::endl;
				break;
			}
		}
	}

	for (auto& boid : m_Boids)
	{
		if (boid->isAlive())
		{
			if (boidsSpawned > 1)
			{		

					boid->SetTarget(Cohesion(boid));
					boid->SetDirection((boid)->GetTarget() - (boid)->GetPos());
					boid->GetDirection().Normalize();
					boid->SetVelocity((boid->GetVelocity() + Cohesion(boid) + Separation(boid) + Alignment(boid)) / 10);
					boid->addPos(boid->GetVelocity());
					//boid->addPos(Cohesion(boid));
					//boid->addPos(Separation(boid));
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

	for (auto& boid : m_Boids)
	{
		if (boid != _boid)
		{
			_center_of_mass += boid->GetPos();
		}
	}
	_center_of_mass /= (m_Boids.size() - 1);

	_cohesion = (_center_of_mass - _boid->GetPos()) / 300;

	return _cohesion;
}

Vector3 BoidManager::Separation(Boid * _boid)
{
	Vector3 _separation = Vector3::Zero;
	for (auto& boid : m_Boids)
	{
		if (boid != _boid)
		{
			if (Vector3::Distance(boid->GetPos(), _boid->GetPos()) < 15.0f)
			{
				_separation -= (boid->GetPos() - _boid->GetPos());
			}
		}
	}
	return _separation / 100;
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
	return (_alignment - _boid->GetVelocity()) / 8;
}
