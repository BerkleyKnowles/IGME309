#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;






MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{

}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{

}

MyOctant::MyOctant(MyOctant const& other)
{
}

MyOctant& Simplex::MyOctant::operator=(MyOctant const& other)
{
	// TODO: insert return statement here
}

MyOctant::~MyOctant(void)
{
}

void MyOctant::Swap(MyOctant& other)
{
}

float MyOctant::GetSize(void)
{
	return 0.0f;
}

vector3 MyOctant::GetCenterGlobal(void)
{
	return vector3();
}

vector3 MyOctant::GetMinGlobal(void)
{
	return vector3();
}

vector3 MyOctant::GetMaxGlobal(void)
{
	return vector3();
}

bool MyOctant::IsColliding(uint a_uRBIndex)
{
	return false;
}

void MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
}

void MyOctant::Display(vector3 a_v3Color)
{
}

void MyOctant::DisplayLeafs(vector3 a_v3Color)
{
}

void MyOctant::ClearEntityList(void)
{
}

void MyOctant::Subdivide(void)
{
}

MyOctant* MyOctant::GetChild(uint a_nChild)
{
	return m_pChild[a_nChild];
}

MyOctant* MyOctant::GetParent(void)
{
	return m_pParent;
}

bool MyOctant::IsLeaf(void)
{
	if (m_uChildren == 0)
		return true;
	return false;
}

bool MyOctant::ContainsMoreThan(uint m_nEntities)
{
	return false;
}

void MyOctant::KillBranches(void)
{
}

void MyOctant::ConstructTree(uint a_nMaxLevel)
{
}

void MyOctant::AssignIDEntity(void)
{
}

uint MyOctant::GetOctantCount(void)
{
	return m_uOctantCount;
}

void MyOctant::Release(void)
{
}

void MyOctant::Init(void)
{
}

void MyOctant::ConstructList(void)
{
}
