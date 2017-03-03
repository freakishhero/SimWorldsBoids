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
		for (vector<Boid*>::iterator iter = m_Boids.begin(); iter != m_Boids.end(); iter++)
		{
			if (!(*iter)->isAlive())
			{
				(*iter)->Spawn({ (float)(rand() % 80) - 40, 10, (float)(rand() % 80) - 40 });
				boidsSpawned++;
				//(*iter)->Spawn({ 10, 10, 10 });
				std::cout << "Boid: " << boidsSpawned << endl;
				std::cout << "PosX: " << (*iter)->GetPos().x << endl;
				std::cout << "PosY: " << (*iter)->GetPos().y << endl;
				std::cout << "PosZ: " << (*iter)->GetPos().z << endl;
				std::cout << "Pitch: " << (*iter)->GetPitch() << endl;
				std::cout << ::endl;
				break;
			}
		}
	}

	for (vector<Boid*>::iterator iter = m_Boids.begin(); iter != m_Boids.end(); iter++)
	{
		if ((*iter)->isAlive())
		{
			(*iter)->Tick(_GD);
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
