#pragma once
#include "VBGO.h"
#include "vertex.h"
#include "GameData.h"
#include "BoidData.h"

class Boid : public VBGO
{
public:
	Boid(int _size, ID3D11Device* _pd3dDevice);
	~Boid();

	void Spawn(Vector3 _pos);

	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;
	
	float getSpeed() { return m_speed; }
	void setPerching(bool state) { m_perching = state; };
	bool isPerching() { return m_perching; };
	bool isAlive() { return m_alive; }
	float getPerchTimer() { return m_perch_timer; }
	void setPerchTimer(float time) { m_perch_timer = time; }
	BoidType getBoidType() { return m_boid_type; }
	void setBoidType(BoidType _type) { m_boid_type = _type; }
	bool isPredator();

private:
	bool m_alive;
	int ID = 0;
	float m_speed = 0.1;
	bool m_perching = false;
	float m_perch_timer = 0.0f;
	Vector3 m_direction;
	Vector3 m_up;
	Vector3 m_target_location;
	virtual void Transform() {};
	int m_size;
	myVertex* m_vertices;
	ID3D11Device* GD;
	BoidType m_boid_type = BoidType::PREY;
};