#pragma once
#include "VBGO.h"
#include "vertex.h"
#include "GameData.h"

class Boid : public VBGO
{
public:
	Boid(int _size, ID3D11Device* _pd3dDevice);
	~Boid();

	void Spawn(Vector3 _pos);

	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;

	void SetDirection(Vector3 _direction) { m_direction = _direction; }
	void SetTarget(Vector3 _target_locaiton) { m_target_location = _target_locaiton; }
	
	float getSpeed() { return m_speed; }
	Vector3 GetDirection() { return m_direction;  }
	Vector3 GetTarget() { return m_target_location; }
	void setPerching(bool state) { perching = state; };
	bool isPerching() { return perching; };
	bool isAlive() { return m_alive; }
	float getPerchTimer() { return perch_timer; }
	void setPerchTimer(float time) { perch_timer = time; }
	bool isPredator() { return predator; }
	void setPredator(bool _state) { predator = _state;  }

private:
	bool m_alive;
	float m_speed = 0.1;
	bool perching = false;
	float perch_timer = 0.0f;
	Vector3 m_direction;
	Vector3 m_up;
	Vector3 m_target_location;
	bool predator = false;
	virtual void Transform() {};
	int m_size;
	myVertex* m_vertices;
	ID3D11Device* GD;
};