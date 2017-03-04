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
	Vector3 cohesion;

	float randX = rand() % 80 - 40;
	float randY = rand() % 80 - 40;

	//if (_GD->m_dt * 0.8 > ((float)rand() / (float)RAND_MAX))
	//{
		for (vector<Boid*>::iterator iter = m_Boids.begin(); iter != m_Boids.end(); iter++)
		{
			if (!(*iter)->isAlive())
			{
				(*iter)->Spawn({ (float)(rand() % 80) - 40, 0, (float)(rand() % 80) - 40 });
				boidsSpawned++;
				std::cout << ::endl;
				break;
			}
		}
	//}

	for (vector<Boid*>::iterator iter = m_Boids.begin(); iter != m_Boids.end(); iter++)
	{
		if ((*iter)->isAlive())
		{

			if (Vector3::Distance((*iter)->GetTarget(), (*iter)->GetDirection()) < 10.0f)
			{
				(*iter)->SetTarget(Cohesion(*iter));
				cohesion = m_Boids[0]->GetTarget();
				(*iter)->SetDirection((*iter)->GetTarget() - (*iter)->GetPos());
				(*iter)->GetDirection().Normalize();
			}
			(*iter)->addPos((*iter)->getSpeed() * _GD->m_dt * (*iter)->GetDirection());
			(*iter)->Tick(_GD);
		}
	}
	std::cout << cohesion.x << endl;
	//std::cout << Cohesion().y << endl;
	std::cout << cohesion.z << endl;
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
	_center_of_mass /= m_Boids.size() - 1;

	_cohesion = (_center_of_mass - _boid->GetPos()) / 100;
	return _cohesion;
}
