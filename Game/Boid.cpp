#include "Boid.h"
#include "VBGO.h"

Boid::Boid(int _size, ID3D11Device * _pd3dDevice) : VBGO()
{
	m_alive = false;
	m_predator = false;
	SetVelocity(Vector3::Zero);
	m_fudge = Matrix::CreateRotationY(XM_PIDIV2);//gives local rotation
	m_up = Vector3::Transform(Vector3::Up, m_fudge.Invert() * m_worldMat) - m_pos;

	//calculate number of vertices and primatives
	m_size = _size;
	int numVerts = 12;
	m_numPrims = numVerts / 3;
	m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
	}

	//top
	m_vertices[0].Color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	m_vertices[0].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[0].Color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	m_vertices[1].Pos = Vector3(0.0f, 0.0f, 2.0f);
	m_vertices[0].Color = Color(0.1f, 0.1f, 0.1f, 1.0f);
	m_vertices[2].Pos = Vector3(2.0f, -0.5f, 1.0f);

	//back
	m_vertices[2].Color = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vertices[3].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[2].Color = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vertices[4].Pos = Vector3(0.0f, 0.0f, 2.0f);
	m_vertices[2].Color = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vertices[5].Pos = Vector3(0.0f, -1.0f, 1.0f);

	//right
	m_vertices[2].Color = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vertices[6].Pos = Vector3(0.0f, 0.0f, 2.0f);
	m_vertices[2].Color = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vertices[7].Pos = Vector3(0.0f, -1.0f, 1.0f);
	m_vertices[2].Color = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vertices[8].Pos = Vector3(2.0f, -0.5f, 1.0f);

	//left
	m_vertices[2].Color = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vertices[9].Pos = Vector3(0.0f, 0.0f, 0.0f);
	m_vertices[2].Color = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vertices[10].Pos = Vector3(0.0f, -1.0f, 1.0f);
	m_vertices[2].Color = Color(0.0f, 0.0f, 0.0f, 0.0f);
	m_vertices[11].Pos = Vector3(2.0f, -0.5f, 1.0f);

	//carry out some kind of transform on these vertices to make this object more interesting
	Transform();

	//calculate the normals for the basic lighting in the base shader
	for (int i = 0; i<m_numPrims; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Pos - m_vertices[V2].Pos;
		Vector3 vec2 = m_vertices[V3].Pos - m_vertices[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}


	BuildIB(_pd3dDevice, indices);
	BuildVB(_pd3dDevice, numVerts, m_vertices);

	delete[] indices;    //this is no longer needed as this is now in the index Buffer
	delete[] m_vertices; //this is no longer needed as this is now in the Vertex Buffer
	m_vertices = nullptr;

}

Boid::~Boid()
{
}

void Boid::Spawn(Vector3 _pos)
{
	m_alive = true;
	m_pos = _pos;
}

void Boid::Tick(GameData * _GD)
{
	if (m_alive)
	{
		Matrix scaleMat = Matrix::CreateScale(m_scale);
		Matrix rotTransMat = Matrix::CreateWorld(m_pos, m_vel, m_up);
		Matrix transMat = Matrix::CreateTranslation(m_pos);
		m_worldMat = m_fudge * scaleMat * rotTransMat * transMat;

		//m_acc.x = 1.0f;
		//m_acc.y = 1.0f;
		//m_acc.z = 1.0f;
	}
}

void Boid::Draw(DrawData * _DD)
{
	if (m_alive)
	{
		VBGO::Draw(_DD);
	}
}
