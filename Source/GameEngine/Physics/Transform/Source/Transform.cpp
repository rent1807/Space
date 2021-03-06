#include "stdafx.h"
#include "../Include/Transform.h" 
using namespace SpaceGameEngine;

REGISTERCOMPONENTCLASS(TransformComponent);

SpaceGameEngine::TransformComponent::TransformComponent()
{
	m_TypeName = "TransformComponent";
	m_Position = { 0,0,0 };
	m_Rotation = { 0,0,0 };
	m_Scale = { 1,1,1 };
}

SpaceGameEngine::TransformComponent::~TransformComponent()
{

}

void SpaceGameEngine::TransformComponent::InitFromFile(const std::string & filename, int mode)
{
	m_Mode = mode;
	File file(filename, FileMode::Read);

	file >> m_Position.x >> m_Position.y >> m_Position.z;
	file >> m_Rotation.x >> m_Rotation.y >> m_Rotation.z;
	file >> m_Scale.x >> m_Scale.y >> m_Scale.z;

	file.Close();
}

void SpaceGameEngine::TransformComponent::Run(float DeltaTime)
{
	if (m_Mode&ForRenderingMode)
	{
		XMMATRIX mrebuff, mbuff;
		mbuff = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		mrebuff = mbuff;
		mbuff = XMMatrixRotationX(m_Rotation.x);
		mrebuff = XMMatrixMultiply(mrebuff, mbuff);
		mbuff = XMMatrixRotationY(m_Rotation.y);
		mrebuff = XMMatrixMultiply(mrebuff, mbuff);
		mbuff = XMMatrixRotationZ(m_Rotation.z);
		mrebuff = XMMatrixMultiply(mrebuff, mbuff);
		mbuff = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
		mrebuff = XMMatrixMultiply(mrebuff, mbuff);

		XMMATRIX result = mrebuff*XMLoadFloat4x4(&SceneData::m_ViewMatrix)*XMLoadFloat4x4(&SceneData::m_ProjectionMatrix);
		SpaceEngineWindow->GetEffectShader().m_pWorldViewProjMatrix->SetMatrix(reinterpret_cast<float*>(&result));
	}
}

void SpaceGameEngine::TransformComponent::SetPosition(const XMFLOAT3 & position)
{
	if (m_pFatherObject)
	{
		m_pFatherObject->ProduceMessage(this,Event::PositionChange);
	}
	m_Position = position;
}

void SpaceGameEngine::TransformComponent::SetRotation(const XMFLOAT3 & rotation)
{
	if (m_pFatherObject)
	{
		m_pFatherObject->ProduceMessage(this, Event::RotationChange);
	}
	m_Rotation = rotation;
}

void SpaceGameEngine::TransformComponent::SetScale(const XMFLOAT3 & scale)
{
	if (m_pFatherObject)
	{
		m_pFatherObject->ProduceMessage(this, Event::ScaleChange);
	}
	m_Scale = scale;
}

XMFLOAT3 SpaceGameEngine::TransformComponent::GetPosition()
{
	return m_Position;
}

XMFLOAT3 SpaceGameEngine::TransformComponent::GetRotation()
{
	return m_Rotation;
}

XMFLOAT3 SpaceGameEngine::TransformComponent::GetScale()
{
	return m_Scale;
}
