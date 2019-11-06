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

MyOctant::MyOctant(vector3 a_v3Center, float a_fSize) //done
{
	Init();
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

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

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
	Release();
}

void MyOctant::Swap(MyOctant& other) //done?
{
	//swaping every variable with the other octant
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_fSize, other.m_fSize);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_uChildren, other.m_uChildren);
	for (int i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
}

float MyOctant::GetSize(void) //done
{
	return m_fSize;
}

vector3 MyOctant::GetCenterGlobal(void) 
{
	return m_v3Center;
}

vector3 MyOctant::GetMinGlobal(void)
{
	return m_v3Min;
}

vector3 MyOctant::GetMaxGlobal(void)
{
	return m_v3Max;
}

bool MyOctant::IsColliding(uint a_uRBIndex) //done
{
	uint entityCount = m_pEntityMngr->GetEntityCount();
	if (entityCount <= a_uRBIndex)
		return false;
	
	//aabb checking basically
	//need to get global max and min
	MyEntity* entityTemp = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* rigidBodyTemp = entityTemp->GetRigidBody();
	vector3 entityMax = rigidBodyTemp->GetMaxGlobal();
	vector3 entityMin = rigidBodyTemp->GetMinGlobal();

	if (entityMin.x > m_v3Max.x || entityMax.x < m_v3Min.x) //x check
		return false;
	if (entityMin.y > m_v3Max.y || entityMax.y < m_v3Min.y) //y check
		return false;
	if (entityMin.z > m_v3Max.z || entityMax.z < m_v3Min.z) //z check
		return false;

	return true;
}

void MyOctant::Display(uint a_nIndex, vector3 a_v3Color) //done?
{

	if (m_uID == a_nIndex) //if its the right index render it
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE);
		return;
	}
	for (int i = 0; i < m_uChildren; i++) //if not, go through all of them recursively
	{
		m_pChild[i]->Display(a_nIndex);
	}
}

void MyOctant::Display(vector3 a_v3Color) //done
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color); //recursion to cover all octants
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)),a_v3Color, RENDER_WIRE); //renders with proper size and location on screen
}

void MyOctant::DisplayLeafs(vector3 a_v3Color) //done
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->Display(a_v3Color); //recursion to cover all octants
	}
	if(IsLeaf())//only adds to render list if childless
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color, RENDER_WIRE); //renders with proper size and location on screen
}

void MyOctant::ClearEntityList(void) //done
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ClearEntityList(); //recursion to get all octants down the tree
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

	for (int i = 0; i < 8; i++)
	{
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel++;
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount))
			m_pChild[i]->Subdivide();
	}
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
	int counter = 0;
	int entityCount = m_pEntityMngr->GetEntityCount();
	for (int i = 0; i < entityCount; i++)
	{
		if (IsColliding(i))
			counter++;
		if(counter > m_nEntities)
			return true;
	}
	return false;
}

void MyOctant::KillBranches(void) //done?
{

	for (int i = 0; i < m_uChildren; i++)
	{
		if (!m_pChild[i]->IsLeaf())
			m_pChild[i]->KillBranches();
		else 
		{
			delete m_pChild[i];
			m_pChild[i] = nullptr;
		}
	}
	m_uChildren = 0;
}

void MyOctant::ConstructTree(uint a_nMaxLevel) //done
{
	if (a_nMaxLevel != 0)
		return;
	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;
	m_EntityList.clear();
	KillBranches();
	m_lChild.clear();
	if (ContainsMoreThan(m_uIdealEntityCount))
		Subdivide();
	AssignIDEntity();
	ConstructList();
}

void MyOctant::AssignIDEntity(void) //not sure if right
{
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->AssignIDEntity(); //recursion to reach leaves
	}
	if (m_uChildren == 0)
	{
		uint entityCount = m_pEntityMngr->GetEntityCount();
		for (uint i = 0; i < entityCount; i++)
		{
			if (IsColliding(i)) { //if its in the octants box, add it to list
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
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

void MyOctant::Init(void) //done
{
	//sets all the variables back to original states
	m_pRoot = nullptr;
	m_uChildren = 0;
	for (int i = 0; i < 8; i++)
	{
		m_pChild[i] = nullptr;
	}
	m_pParent = nullptr;
	m_uID = m_uOctantCount;
	m_uLevel = 0;
	m_fSize = 0.0f;
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
	m_v3Center = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();
}

void MyOctant::ConstructList(void) //done
{
	//recursion to add to list
	for (int i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->ConstructList();
	}
	if (m_EntityList.size() > 0)
		m_pRoot->m_lChild.push_back(this); //adds all the leaves and branches to the roots child list
}
