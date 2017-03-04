#pragma once
#include "CMOGO.h"
#include "GameData.h"

class Boid : public CMOGO
{
public:
	Boid(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	~Boid();

	void Spawn(Vector3 _pos);

	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;

	void SetDirection(Vector3 _direction) { m_direction = _direction; }
	void SetTarget(Vector3 _target_locaiton) { m_target_location = _target_locaiton; }
	
	float getSpeed() { return m_speed; }
	Vector3 GetDirection() { return m_direction;  }
	Vector3 GetTarget() { return m_target_location; }
	bool isAlive() { return m_alive; }

private:
	bool m_alive;
	float m_speed = 0.1;
	Vector3 m_direction;
	Vector3 m_target_location;
};