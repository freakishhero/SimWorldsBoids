#include "BoidManager.h"
#include <ctime>
#include <iostream>

BoidManager::BoidManager(int _boid_count, int _predator_count,
	int _alpha_predator_count, int _object_count,
	int _size, ID3D11Device * _pd3dDevice)
{
	predator_count = _predator_count;
	alpha_predator_count = _alpha_predator_count;
	object_count = _object_count;

	for (int i = 0; i < _boid_count; i++)
	{
		m_Boids.push_back(new Boid(_size, _pd3dDevice));
	}

	if (predator_count > _boid_count)
	{
		predator_count = (_boid_count / 30) + 1;
	}

	if (alpha_predator_count > _boid_count)
	{
		predator_count = (_boid_count / 90) + 1;
	}


	for (int i = 0; i < predator_count; i++)
	{
		m_Boids[i]->setBoidType(BoidType::PREDATOR);
		m_Boids[i]->SetScale(3.0f);
	}

	for (int i = 0; i < alpha_predator_count; i++)
	{
		m_Boids[i]->setBoidType(BoidType::ALPHA_PREDATOR);
		m_Boids[i]->SetScale(6.0f);
	}

	for (int i = 0; i < object_count; i++)
	{
		m_Boids[i]->setBoidType(BoidType::OBJECT);
		m_Boids[i]->SetScale(9.0f);
	}
}

BoidManager::~BoidManager()
{
	for (auto& boid : m_Boids)
	{
		delete boid;
	}
}

void BoidManager::Tick(GameData * _GD)
{
	for (auto& boid : m_Boids)
	{
		if (!boid->isAlive())
		{
			boid->Spawn({ (float)(rand() % 80) - 80, (float)(rand() % 80) - 80, (float)(rand() % 80) - 80 });
			boids_spawned++;
			break;
		}

		if (boid->isAlive())
		{

			if (boids_spawned > 1 && boid->getBoidType() != BoidType::OBJECT)
			{
				ApplyRules(_GD, boid);
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
			if (_boid->getBoidType() != BoidType::OBJECT)
			{
				if (boid != _boid && boid->getBoidType() != BoidType::PREDATOR)
				{
					if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < cohesion_prey_radius))
					{
						_center_of_mass += boid->GetPos();
						nearby_boids++;
					}
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
	//float radius = 0.0f;

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

	int count = 0;
	for (auto& boid : m_Boids)
	{
		if (boid != _boid)
		{
			if (fabs(Vector3::Distance(boid->GetPos(), _boid->GetPos()) < alignment_prey_radius))
			{
				_alignment += boid->GetVelocity();
				count++;
			}
		}
	}

	if (count > 0)
	{
		_alignment / count;
		_alignment.Normalize();
		_alignment *= prey_speed_limit;

		return (_alignment - _boid->GetVelocity());
	}
	else
	{
		return Vector3::Zero;
	}
}

Vector3 BoidManager::Bind_Position(Boid * _boid)
{
	int Xmin = -70, Xmax = 70, Ymin = -70, Ymax = 70, Zmin = -70, Zmax = 70;
	Vector3 position;

	if (_boid->GetPos().x < Xmin)
	{
		_boid->SetPos(Vector3(Xmax, _boid->GetPos().y, _boid->GetPos().z));
	}
	else if (_boid->GetPos().x > Xmax)
	{
		_boid->SetPos(Vector3(Xmin, _boid->GetPos().y, _boid->GetPos().z));
	}

	if (_boid->GetPos().y < Ymin)
	{
		_boid->SetPos(Vector3(_boid->GetPos().x, Ymax, _boid->GetPos().z));
	}
	else if (_boid->GetPos().y > Ymax)
	{
		_boid->SetPos(Vector3(_boid->GetPos().x, Ymin, _boid->GetPos().z));
	}

	if (_boid->GetPos().z < Zmin)
	{
		_boid->SetPos(Vector3(_boid->GetPos().x, _boid->GetPos().y, Zmax));
	}
	else if (_boid->GetPos().z > Zmax)
	{
		_boid->SetPos(Vector3(_boid->GetPos().x, _boid->GetPos().y, Zmin));
	}
	return position;
}

Vector3 BoidManager::Scatter(Boid * _boid)
{
	Vector3 _scatter = Vector3::Zero;
	for (auto& boid : m_Boids)
	{
		if (boid != _boid && boid->isPredator() == true)
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
	else if (_boid->isPredator() == false)
	{
		limit = prey_speed_limit;
	}

	if ((fabs(_boid->GetVelocity().x) + fabs(_boid->GetVelocity().y) + fabs(_boid->GetVelocity().z) > limit))
	{
		_boid->SetVelocity((_boid->GetVelocity() / (fabs(_boid->GetVelocity().x) + fabs(_boid->GetVelocity().y) + fabs(_boid->GetVelocity().z))) * limit);
	}
}

void BoidManager::ApplyRules(GameData* _GD, Boid* _boid)
{
	Vector3 cohesion_rule = Vector3::Zero;
	Vector3 separation_rule = Vector3::Zero;
	Vector3 allignment_rule = Vector3::Zero;
	Vector3 bind_position_rule = Vector3::Zero;
	Vector3 scatter_rule = Vector3::Zero;

	switch (_boid->getBoidType())
	{
	case BoidType::PREY:
		cohesion_rule = (Cohesion(_boid) / cohesion_prey_modifier);
		separation_rule = (Separation(_boid) / separation_prey_modifier);
		allignment_rule = (Alignment(_boid) / alignment_prey_modifier);
		scatter_rule = (Scatter(_boid) / scatter_modifier);
		_boid->SetAcceleration(_boid->GetAcceleration() + cohesion_rule + separation_rule + allignment_rule + bind_position_rule + scatter_rule);
		break;
	case BoidType::PREDATOR:
		cohesion_rule = (Cohesion(_boid) / cohesion_predator_modifier);
		separation_rule = (Separation(_boid) / separation_predator_modifier);
		allignment_rule = (Alignment(_boid) / alignment_predator_modifier);
		_boid->SetAcceleration((_boid->GetAcceleration() + cohesion_rule + separation_rule + allignment_rule + bind_position_rule));
		break;
	case BoidType::ALPHA_PREDATOR:
		cohesion_rule = (Cohesion(_boid) / cohesion_predator_modifier);
		separation_rule = (Separation(_boid) / separation_predator_modifier);
		allignment_rule = (Alignment(_boid) / alignment_predator_modifier);
		_boid->SetAcceleration(_boid->GetAcceleration() + cohesion_rule + separation_rule + allignment_rule + bind_position_rule);
		break;
	case BoidType::LEADER:
		cohesion_rule = (Cohesion(_boid) / cohesion_prey_modifier);
		separation_rule = (Separation(_boid) / separation_prey_modifier);
		allignment_rule = (Alignment(_boid) / alignment_prey_modifier);
		scatter_rule = (Scatter(_boid) / scatter_modifier);
		_boid->SetAcceleration(_boid->GetAcceleration() + cohesion_rule + separation_rule + allignment_rule + bind_position_rule + scatter_rule);
		break;
	case BoidType::OBJECT:
		_boid->SetAcceleration(_boid->GetAcceleration());
		break;
	}
	bind_position_rule = Bind_Position(_boid);

	set2D(_boid);


	Limit_Speed(_boid);
	_boid->SetVelocity(_boid->GetVelocity() + _boid->GetAcceleration());
	_boid->SetAcceleration(Vector3::Zero);
	_boid->addPos(_boid->GetVelocity() * _GD->m_dt);
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

float * BoidManager::setD()
{

	return &D;
}

int* BoidManager::get_boids_spawned()
{
	return &boids_spawned;
}

void BoidManager::set2D(Boid * _boid)
{
	if (D == 0)
	{
		_boid->SetPos(Vector3(_boid->GetPos().x, 0, _boid->GetPos().z));
	}
}
