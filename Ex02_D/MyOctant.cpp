#include "MyOctant.h"
using namespace ReEng;
//  MyOctant
void MyOctant::Init(void)
{
	m_v3Position = vector3(0.0f);
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_fSize = 0.0f;
	m_pBOMngr = MyBOManager::GetInstance();

	int nObjectCount = m_pBOMngr->GetObjectCount();

	
	//YOUR CODE GOES HERE
	std::vector<vector3> lMinMax; //Will hold all of the Min and Max vectors of the Bounding Object

	int nObjects = m_pBOMngr->GetObjectCount();
	for (int i = 0; i < nObjects; i++)
	{
		MyBOClass* pBO = m_pBOMngr->GetBoundingObject(i);
		lMinMax.push_back(pBO->GetMinG());
		lMinMax.push_back(pBO->GetMaxG());
	}
	MyBOClass* pBO = new MyBOClass(lMinMax);

	vector3 vHalfWidth = pBO->GetHalfWidthG();
	float fMax = vHalfWidth.x;
	for (int i = 1; i < 3; i++)
	{
		if (fMax < vHalfWidth[i])
			fMax = vHalfWidth[i];
	}
	m_v3Position = pBO->GetCenterLocal();
	lMinMax.clear();
	SafeDelete(pBO);

	m_fSize = fMax * 2.0f;
}
void MyOctant::Release(void)
{
}
//The big 3
MyOctant::MyOctant(){Init();}
MyOctant::~MyOctant(){Release();};

void MyOctant::Display(void)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(m_v3Position) * glm::scale(vector3(m_fSize)), REPURPLE, WIRE);
}


