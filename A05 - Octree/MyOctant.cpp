#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 3;
uint MyOctant::m_uIdealEntityCount = 5;


MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount) //root creator, done
{
	Init();

	m_pRoot = this;
	m_uOctantCount = 0; //ensures it was zero before hand
	m_uID = m_uOctantCount;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	//everything below is for getting max, min, and center values for the octant
	std::vector<vector3> m_lMaxMinList;
	uint entityCount = m_pEntityMngr->GetEntityCount(); //all entities on screen
	for (uint i = 0; i < entityCount; i++)
	{
		MyEntity* entityTemp = m_pEntityMngr->GetEntity();
		MyRigidBody* rigidBodyTemp = entityTemp->GetRigidBody();
		m_lMaxMinList.push_back(rigidBodyTemp->GetMinGlobal());
		m_lMaxMinList.push_back(rigidBodyTemp->GetMaxGlobal());
	}
	MyRigidBody* RigidBodyTemp = new MyRigidBody(m_lMaxMinList);
	vector3 v3HalfWidth = RigidBodyTemp->GetHalfWidth();

	m_v3Center = RigidBodyTemp->GetCenterLocal(); //sets center

	//these are no longer needed
	SafeDelete(RigidBodyTemp);
	m_lMaxMinList.clear(); 

	float fMax = v3HalfWidth.x;
	for (int i = 0; i < 3; i++) //gets the max value of the halfwidths
	{
		if (fMax < v3HalfWidth[i])
			fMax = v3HalfWidth[i];
	}

	//gottem
	m_fSize = fMax * 2;
	m_v3Max = m_v3Center + vector3(fMax);
	m_v3Min = m_v3Center - vector3(fMax);

	m_uOctantCount++;
	ConstructTree(m_uMaxLevel);
}

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
	m_v3Max = m_v3Center + vector3(m_fSize/2);
	m_v3Min = m_v3Center - vector3(m_fSize/2);
	m_uOctantCount++;

}

MyOctant::MyOctant(MyOctant const& other) //done?
{
	this->m_EntityList = other.m_EntityList;
	this->m_fSize = other.m_fSize;
	this->m_lChild = other.m_lChild;
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
	this->m_pEntityMngr = other.m_pEntityMngr;
	this->m_pMeshMngr = other.m_pMeshMngr;
	this->m_pParent = other.m_pParent;
	this->m_pRoot = other.m_pRoot;
	this->m_uChildren = other.m_uChildren;
	this->m_uID = other.m_uID;
	this->m_uIdealEntityCount = other.m_uIdealEntityCount;
	this->m_uLevel = other.m_uLevel;
	this->m_uMaxLevel = other.m_uMaxLevel;
	this->m_uOctantCount = other.m_uOctantCount;
	this->m_v3Center = other.m_v3Center;
	this->m_v3Max = other.m_v3Max;
	this->m_v3Min = other.m_v3Min;

}

MyOctant& Simplex::MyOctant::operator=(MyOctant const& other) //done?
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}

MyOctant::~MyOctant(void) //done?
{
	for (int i = 0; i < m_uChildren; i++)
	{
		delete m_lChild[i];
		m_lChild[i] = nullptr;
	}
	delete[] m_pChild;
	delete m_pParent;
	delete m_pRoot;
	m_pRoot = nullptr;
	m_pParent = nullptr;
	m_pMeshMngr = nullptr;
	m_pEntityMngr = nullptr;
	
}

void MyOctant::Swap(MyOctant& other) //dont need it ??
{
	
}

float MyOctant::GetSize(void) //done
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

void MyOctant::Display(uint a_nIndex, vector3 a_v3Color) //questionable
{
	for (int i = 0; i < m_uChildren; i++)
	{
		if(i == a_nIndex)
			m_pMeshMngr->AddCubeToRenderList(glm::translate(IDENTITY_M4, m_pChild[i]->m_v3Center) * glm::scale(vector3(m_pChild[i]->m_fSize)), a_v3Color, RENDER_WIRE);
	}

}

void MyOctant::Display(vector3 a_v3Color) //done
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color); //recursion to cover all octants
	}
	m_pMeshMngr->AddCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)),a_v3Color, RENDER_WIRE); //renders with proper size and location on screen
}

void MyOctant::DisplayLeafs(vector3 a_v3Color) //done
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color); //recursion to cover all octants
	}
	if(IsLeaf())//only adds to render list if childless
		m_pMeshMngr->AddCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE); //renders with proper size and location on screen
}

void MyOctant::ClearEntityList(void) //done
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ClearEntityList();
	}
	m_EntityList.clear();
}

void MyOctant::Subdivide(void) //done
{
	//intial checks: have we reached max level or is it a parent?
	if (m_uLevel == m_uMaxLevel || m_uChildren != 0)
		return;

	//if not, Congradulations! This Octant is now a parent! plus 8 kids!
	m_uChildren = 8;

	//temp values needed to make these kids
	vector3 v3Center = m_v3Center;
	float fSize = m_fSize / 4.0f;
	float fSizeDoub = fSize * 2;

	//Bottoms~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//[0]: octant 1 - back right
	v3Center.x += fSize;
	v3Center.y -= fSize;
	v3Center.z -= fSize;
	m_pChild[0] = new MyOctant(v3Center, fSizeDoub);
	
	//[1]: octant 2 - back left
	v3Center.x -= fSizeDoub;
	m_pChild[1] = new MyOctant(v3Center, fSizeDoub);

	//[2]: octant 3 - front left
	v3Center.z += fSizeDoub;
	m_pChild[2] = new MyOctant(v3Center, fSizeDoub);

	//[3]: octant 4 - front right
	v3Center.x += fSizeDoub;
	m_pChild[3] = new MyOctant(v3Center, fSizeDoub);

	//Tops~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//[4]: octant 5 - front right
	v3Center.y += fSizeDoub;
	m_pChild[4] = new MyOctant(v3Center, fSizeDoub);

	//[5]: octant 6 - front left
	v3Center.x -= fSizeDoub;
	m_pChild[5] = new MyOctant(v3Center, fSizeDoub);

	//[6] octant 7 - back left
	v3Center.z -= fSizeDoub;
	m_pChild[6] = new MyOctant(v3Center, fSizeDoub);

	//[7] octant 8 - back right
	v3Center.x += fSizeDoub;
	m_pChild[7] = new MyOctant(v3Center, fSizeDoub);
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
	if (m_EntityList.size() > m_nEntities)
		return true;
	return false;
}

void MyOctant::KillBranches(void) //done?
{
	if (!IsLeaf()) 
	{
		for (int i = 0; i < m_uChildren; i++)
		{
			if (!m_pChild[i]->IsLeaf())
				m_pChild[i]->KillBranches();
			else
				m_pChild[i] = nullptr;
		}
	}
}

void MyOctant::ConstructTree(uint a_nMaxLevel)
{
	if (a_nMaxLevel != 0)
		return;
	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;
	m_EntityList.clear();
	KillBranches();
	m_lChild.clear();

}

void MyOctant::AssignIDEntity(void) //not sure if right
{
	m_uIdealEntityCount++;
}

uint MyOctant::GetOctantCount(void) //done
{
	return m_uOctantCount;
}

void MyOctant::Release(void) //done
{
	//root release
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_fSize = 0.0f;
	m_uChildren = 0;
	ClearEntityList();
	m_lChild.clear();

}

void MyOctant::Init(void)
{
}

void MyOctant::ConstructList(void)
{
}
