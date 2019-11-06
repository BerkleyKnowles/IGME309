#pragma once
#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_

#include "MyEntityManager.h"


namespace Simplex
{

	class MyOctant
	{
		static uint m_uOctantCount;
		static uint m_uMaxLevel;
		static uint m_uIdealEntityCount;

		uint m_uID = 0;
		uint m_uLevel = 0;
		uint m_uChildren = 0;

		float m_fSize = 0.0f;

		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr;

		vector3 m_v3Center = vector3(0.0f);
		vector3 m_v3Min = vector3(0.0f);
		vector3 m_v3Max = vector3(0.0f);

		MyOctant* m_pParent = nullptr;
		MyOctant* m_pChild[8];

		std::vector<uint> m_EntityList;

		MyOctant* m_pRoot = nullptr;
		std::vector<MyOctant*> m_lChild;

	public:
		MyOctant(uint a_nMaxLevel = 2, uint a_nIdealEntityCount = 5);
		//constructor
		//creates octant containing all MagnaEntities Instances in mesh manager that is currently contained

		MyOctant(vector3 a_v3Center, float a_fSize);
		//constructor
		//center of the global space and the size of each side of octant

		MyOctant(MyOctant const& other);
		//copy constructor
		//class object to copy

		MyOctant& operator=(MyOctant const& other);
		//copy assignment Operator

		~MyOctant(void);
		//destructor

		void Swap(MyOctant& other);
		//swaps two objects contents

		float GetSize(void);
		//gets size of octant

		vector3 GetCenterGlobal(void);
		//gets center of octant in global space

		vector3 GetMinGlobal(void);
		//gets min corner of octant in global space

		vector3 GetMaxGlobal(void);
		//you guessed it, gets the max corner of octant in gloabl space

		bool IsColliding(uint a_uRBIndex);
		//checks for collision with entity at index input from bounding object manager

		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);
		//displays MyOctant volume specified by index, incuding objects underneath

		void Display(vector3 a_v3Color = C_YELLOW);
		//displays MyOctant volume in color here

		void DisplayLeafs(vector3 a_v3Color = C_YELLOW);
		//displays non-empty leaves

		void ClearEntityList(void);
		//clears the entity list (shocker) for each node

		void Subdivide(void);
		//allocates 8 smaller ocntants in child pointers


		MyOctant* GetChild(uint a_nChild);
		//returns the child at the index given

		MyOctant* GetParent(void);
		//returns the parent

		bool IsLeaf(void);
		//determines if it has any children (if not, its a leaf)

		bool ContainsMoreThan(uint m_nEntities);
		//determines if it conains more than a number of bounding boxes


		void KillBranches(void);
		//deletes children and their children

		void ConstructTree(uint a_nMaxLevel = 3);
		//creates a tree with subdivs, max num of objs and lvls

		void AssignIDEntity(void);
		//traverse tree and set objects into the index

		uint GetOctantCount(void);
		//gets total num of octants in world

	private:
		void Release(void);
		//deallocates member fields

		void Init(void);
		//allocates member fields

		void ConstructList(void);
		//creates list of all leafs containing objects


		
	};
}

#endif // !__MYOCTANTCLASS_H_
