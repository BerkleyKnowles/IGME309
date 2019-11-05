#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;


MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
}

MyOctant::MyOctant(MyOctant const& other)
{
	this->m_EntityList = other.m_EntityList;
	this->m_fSize = other.m_fSize;
	this->m_lChild = other.m_lChild;
	//this->m_pChild = other.m_pChild;
	this->m_pEntityMngr = other.m_pEntityMngr;
	this->m_pMeshMngr = other.m_pMeshMngr;
	this->m_pParent = other.m_pParent;
	this->m_pRoot = other.m_pRoot;
	this->m_uChildren = other.m_uChildren;
	this->m_uID = other.m_uID;
	this->m_uIdealEntityCount = other.m_uIdealEntityCount;
	this->m_uLevel = other.m_uLevel;

}

MyOctant& Simplex::MyOctant::operator=(MyOctant const& other)
{
	
}

MyOctant::~MyOctant(void)
{
	//for (int i = 0; i < m_pChild; i++)
	//{

	//}
	delete m_pRoot;
	m_pRoot = nullptr;
	
}

void MyOctant::Swap(MyOctant& other)
{
	
}

float MyOctant::GetSize(void)
{
	return m_fSize;
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
	m_EntityList.clear();
}

void MyOctant::Subdivide(void)
{
	m_pChild[0] = new MyOctant();
	m_pChild[1] = new MyOctant();
	m_pChild[2] = new MyOctant();
	m_pChild[3] = new MyOctant();
	m_pChild[4] = new MyOctant();
	m_pChild[5] = new MyOctant();
	m_pChild[6] = new MyOctant();
	m_pChild[7] = new MyOctant();
}

MyOctant* MyOctant::GetChild(uint a_nChild) //done
{
	return m_pChild[a_nChild];
}

MyOctant* MyOctant::GetParent(void) //done
{
	return m_pParent;
}

bool MyOctant::IsLeaf(void) //done
{
	if (m_uChildren == 0)
		return true;
	return false;
}

bool MyOctant::ContainsMoreThan(uint m_nEntities) //done
{
	if (m_EntityList.size > m_nEntities)
		return true;
	return false;
}

void MyOctant::KillBranches(void)
{
	if (!IsLeaf) 
	{

	}
}

void MyOctant::ConstructTree(uint a_nMaxLevel)
{
}

void MyOctant::AssignIDEntity(void)
{
}

uint MyOctant::GetOctantCount(void) //done
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
