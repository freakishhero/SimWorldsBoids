#include "Boid.h"

Boid::Boid(string _fileName, ID3D11Device * _pd3dDevice, IEffectFactory * _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_alive = false;
}

Boid::~Boid()
{
}

void Boid::Spawn(Vector3 _pos)
{
	m_alive = true;
	m_pos = _pos;
	m_target_location = Vector3::Zero;
	m_direction = m_target_location - m_pos;
	m_direction.Normalize();
}

void Boid::Tick(GameData * _GD)
{
	if (m_alive)
	{
		Matrix scaleMat = Matrix::CreateScale(m_scale);
		Matrix rotTransMat = Matrix::CreateWorld(m_pos, m_direction, Vector3::Up);
		m_worldMat = m_fudge *scaleMat * rotTransMat;
		//CMOGO::Tick(_GD);
	}
}

void Boid::Draw(DrawData * _DD)
{
	if (m_alive)
	{
		CMOGO::Draw(_DD);
	}
}
