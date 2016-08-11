#include "object.h"


void object::init(LPDIRECT3DDEVICE9 g_pd3dDevice, int VertexSize, int IndexSize, CUSTOMVERTEX Vertices[], WORD Indices[])
{
	
	m_VertexSize = VertexSize;
	m_IndexSize = IndexSize;
	m_PrimitiveCount = m_IndexSize/3;
	g_pd3dDevice->CreateVertexBuffer(m_VertexSize*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	g_pd3dDevice->CreateIndexBuffer(m_IndexSize*sizeof(WORD), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIndexBuffer, NULL);
	WriteInVertexBuffer(Vertices);
	WriteInIndexBuffer(Indices);
}

void object::InitFromFile(LPDIRECT3DDEVICE9 g_pd3dDevice,std::string filename)
{
	std::fstream file;
	file.open(filename, std::ios::in);
	int VertexSize, IndexSize;
	file >> VertexSize;
	file >> IndexSize;
	CUSTOMVERTEX* Vertices = new CUSTOMVERTEX[VertexSize];
	WORD* Indices = new WORD[IndexSize];
	for (int i = 0;i < VertexSize;i++)
	{
		file >> Vertices[i].x >> Vertices[i].y >> Vertices[i].z >>Vertices[i].nx>> Vertices[i].ny>> Vertices[i] .nz>> Vertices[i].u >> Vertices[i].v;
	}
	for (int j = 0;j < IndexSize;j++)
	{
		file >> Indices[j];
	}
	init(g_pd3dDevice, VertexSize, IndexSize, Vertices, Indices);
	file.close();
}

void object::InitWithLightFromFile(LPDIRECT3DDEVICE9 g_pd3dDevice, std::string filename,std::string lightfilename)
{
	std::fstream file;
	file.open(filename, std::ios::in);
	int VertexSize, IndexSize;
	file >> VertexSize;
	file >> IndexSize;
	CUSTOMVERTEX* Vertices = new CUSTOMVERTEX[VertexSize];
	WORD* Indices = new WORD[IndexSize];
	for (int i = 0;i < VertexSize;i++)
	{
		file >> Vertices[i].x >> Vertices[i].y >> Vertices[i].z >> Vertices[i].nx >> Vertices[i].ny >> Vertices[i].nz >> Vertices[i].u >> Vertices[i].v;
	}
	for (int j = 0;j < IndexSize;j++)
	{
		file >> Indices[j];
	}
	init(g_pd3dDevice, VertexSize, IndexSize, Vertices, Indices);

	std::fstream lightfile;
	lightfile.open(lightfilename, std::ios::in);
	std::string true_lightfilename;
	int light_flag = 0;
	lightfile >> true_lightfilename;
	lightfile >> light_flag;
	lightfile >> m_lx >> m_ly >> m_lz;
	switch (light_flag)
	{
	case 0: {
		InitPointLightFromFile(g_pd3dDevice, true_lightfilename);
		break;
	}
	case 1: {
		InitDirectionalLightFromFile(g_pd3dDevice, true_lightfilename);
		break;
	}
	case 2: {
		InitSpotLightFromFile(g_pd3dDevice, true_lightfilename);
		break;
	}
	default: {
		InitPointLightFromFile(g_pd3dDevice, true_lightfilename);
		break;
	}
	}
	lightfile.close();

	file.close();
}

object::~object()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}

object::object()
{
	m_VertexSize = 0;
	m_IndexSize = 0;
	m_PrimitiveCount = 0;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	mpVertices = NULL;
	mpIndices = NULL;
	m_TimeNow = 0;
	m_TimeChange = 0;
	m_a = { 0,0,0 };
	m_F = { 0,0,0 };
	m_m = 0;
	m_rxa = 0;
	m_rxv = 0;
	m_rya = 0;
	m_ryv = 0;
	m_rza = 0;
	m_rzv = 0;
}

object::object(int & i)
{
	m_VertexSize = 0;
	m_IndexSize = 0;
	m_PrimitiveCount = 0;
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	mpVertices = NULL;
	mpIndices = NULL;
	m_TimeNow = 0;
	m_TimeChange = 0;
	m_a = { 0,0,0 };
	m_F = { 0,0,0 };
	m_m = 0;
	m_rxa = 0;
	m_rxv = 0;
	m_rya = 0;
	m_ryv = 0;
	m_rza = 0;
	m_rzv = 0;
	m_Light.SetLightNumber(i);
}

void object::WriteInVertexBuffer(CUSTOMVERTEX Vertices[])
{
	m_pVertexBuffer->Lock(0, 0, (void**)&mpVertices, 0);
	memcpy(mpVertices, Vertices, m_VertexSize*sizeof(CUSTOMVERTEX));
	m_pVertexBuffer->Unlock();
}

void object::WriteInIndexBuffer(WORD Indices[])
{
	m_pIndexBuffer->Lock(0, 0, (void**)&mpIndices, 0);
	memcpy(mpIndices, Indices, m_IndexSize*sizeof(WORD));
	m_pIndexBuffer->Unlock();
}

void object::ObjectPrint(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	g_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	g_pd3dDevice->SetIndices(m_pIndexBuffer);
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexSize, 0,(UINT)m_PrimitiveCount);
}

void object::InitPointLightFromFile(LPDIRECT3DDEVICE9 g_pd3dDevice, std::string filename)
{
	m_Light.SetPointLightsFromFile(filename);
	m_Light.RegisterLight(g_pd3dDevice);
}

void object::InitDirectionalLightFromFile(LPDIRECT3DDEVICE9 g_pd3dDevice, std::string filename)
{
	m_Light.SetDirectionalLightsFromFile(filename);
	m_Light.RegisterLight(g_pd3dDevice);
}

void object::InitSpotLightFromFile(LPDIRECT3DDEVICE9 g_pd3dDevice, std::string filename)
{
	m_Light.SetSpotLightFromFile(filename);
	m_Light.RegisterLight(g_pd3dDevice);
}

void object::LightPrint(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	m_Light.SetLightPosition(m_lx + m_Position.x, m_ly + m_Position.y, m_lz + m_Position.z);
	D3DXVECTOR3 buf;
	buf = m_Light.m_LightContent.Direction;
	D3DXMATRIX matrx, matry, matrz;
	D3DXMatrixRotationX(&matrx, m_rxv);
	D3DXMatrixRotationY(&matry, m_ryv);
	D3DXMatrixRotationZ(&matrz, m_rzv);
	D3DXVec3TransformCoord(&buf, &buf, &matrx);
	D3DXVec3TransformCoord(&buf, &buf, &matry);
	D3DXVec3TransformCoord(&buf, &buf, &matrz);
	m_Light.SetLightDirection(buf.x, buf.y, buf.z);
	m_Light.BeginLightPrint(g_pd3dDevice);
}

void object::GetTime()
{
	time_t TimeBuffer=0;
	if (m_TimeNow != 0)
	{
		m_TimeNow = time(NULL);
	}
	else
	{
		TimeBuffer = time(NULL);
		m_TimeChange = TimeBuffer - m_TimeNow;
		m_TimeNow = time(NULL);
	}
}

void object::SetXYZ(float x, float y, float z)
{
	m_Position = D3DXVECTOR3(x, y, z);
}

void object::InitPhysicsFromFile(std::string filename)
{
}

void object::SetMatrix(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matBuf;
	D3DXMatrixTranslation(&matWorld, m_Position.x, m_Position.y, m_Position.z);
	if (m_rxv != 0)
	{
		D3DXMatrixRotationX(&matBuf, m_rxv);
		matWorld = matBuf*matWorld;
	}
	if (m_ryv != 0)
	{
		D3DXMatrixRotationY(&matBuf, m_ryv);
		matWorld = matBuf*matWorld;
	}
	if (m_rzv != 0)
	{
		D3DXMatrixRotationZ(&matBuf, m_rzv);
		matWorld = matBuf*matWorld;
	}
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

void object::GetG()
{
}

void object::RunMovingGraphics()
{
}