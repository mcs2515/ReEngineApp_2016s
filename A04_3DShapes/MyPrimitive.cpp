#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}

void MyPrimitive::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTop)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTop);
}


void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	std::vector<vector3> point;
	float theta = 0;
	float steps = 2*PI /a_nSubdivisions;
	//create a point to start from
	point.push_back(vector3(0, 0, -a_fHeight/2));

	//create the points
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//base
		point.push_back(vector3(cos(theta)*a_fRadius, sin(theta)*a_fRadius, a_fHeight/2));
		theta += steps;
	}

	//create a new point to start from
	point.push_back(vector3(0, 0, a_fHeight/2));

	//connect the points to make triangles
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//cone
		AddTri(point[0], point[i+1], point[i]);
		AddTri(point[a_nSubdivisions + 1], point[i], point[i+1]);
	}

	//add last piece for base
	AddTri(point[0], point[1], point[a_nSubdivisions]);
	//add the last triangle piece to the cone
	AddTri(point[a_nSubdivisions+1], point[a_nSubdivisions], point[1]);

	//Your code ends here
	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	std::vector<vector3> point;
	float theta = 0;
	float steps = 2 * PI / a_nSubdivisions;
	//BASES---------------------------------------------------------------------------
	//create a point to start from
	point.push_back(vector3(0, 0, -a_fHeight / 2));

	//Base B
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//base
		point.push_back(vector3(cos(theta)*a_fRadius, sin(theta)*a_fRadius, -a_fHeight / 2));
		theta += steps;
	}
	//Base F
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//base
		point.push_back(vector3(cos(theta)*a_fRadius, sin(theta)*a_fRadius, a_fHeight / 2));
		theta += steps;
	}

	//create a new point to start from
	point.push_back(vector3(0, 0, a_fHeight / 2));

	//connect the points to make triangles
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		AddTri(point[0], point[i + 1], point[i]); //B
		AddTri(point[(a_nSubdivisions*2) + 1], point[i+a_nSubdivisions], point[i+ a_nSubdivisions + 1]); //F
	}

	//add last piece for base
	AddTri(point[0], point[1], point[a_nSubdivisions]);
	//add the last triangle piece for sides
	AddTri(point[(a_nSubdivisions * 2)+ 1], point[(a_nSubdivisions *2)], point[a_nSubdivisions + 1]);
	//END BASES---------------------------------------------------------------------------
	
	//CYLINDER PART-----------------------------------------------------------------------

	//draw the triangles
	for (int i = 0; i <= a_nSubdivisions; i++)
	{
		AddQuad(point[i], point[i + 1], point[ a_nSubdivisions+i], point[a_nSubdivisions + i +1]);
	}

	//Your code ends here
	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	std::vector<vector3> point;
	float theta = 0;
	float steps = 2 * PI / a_nSubdivisions;

	//OUTER CYLINDER PART-----------------------------------------------------------------------

	//BASES---------------------------------------------------------------------------
	//create a point to start from
	//point.push_back(vector3(0, 0, -a_fHeight / 2));
	//OUTER
	//Base B
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//base
		point.push_back(vector3(cos(theta)*a_fOuterRadius, sin(theta)*a_fOuterRadius, -a_fHeight / 2));
		theta += steps;
	}
	//Base F
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//base
		point.push_back(vector3(cos(theta)*a_fOuterRadius, sin(theta)*a_fOuterRadius, a_fHeight / 2));
		theta += steps;
	}

	//create a new point to start from
	//point.push_back(vector3(0, 0, a_fHeight / 2));

	//INNER
	//Base B
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//base
		point.push_back(vector3(cos(theta)*a_fInnerRadius, sin(theta)*a_fInnerRadius, -a_fHeight / 2));
		theta += steps;
	}
	//Base F
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//base
		point.push_back(vector3(cos(theta)*a_fInnerRadius, sin(theta)*a_fInnerRadius, a_fHeight / 2));
		theta += steps;
	}

	//END BASES---------------------------------------------------------------------------

	//CYLINDER PART-----------------------------------------------------------------------

	//draw the Quads
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddQuad(point[i], point[i + 1], point[a_nSubdivisions + i], point[a_nSubdivisions + i + 1]); //outer
		AddQuad(point[(a_nSubdivisions * 2)+i+1], point[(a_nSubdivisions * 2) +i], point[(a_nSubdivisions*3) + i+1], point[(a_nSubdivisions*3) + i]); //inner
		AddQuad(point[i], point[(a_nSubdivisions * 2)+i], point[i+1], point[(a_nSubdivisions*2)+i+1]); //back
		AddQuad(point[a_nSubdivisions+i+1], point[(a_nSubdivisions * 3) + i +1], point[a_nSubdivisions +i], point[(a_nSubdivisions * 3) + i]);//Front
	}

	//add the last triangle piece for sides
	AddQuad(point[a_nSubdivisions-1], point[0], point[(a_nSubdivisions*2)-1], point[a_nSubdivisions]); //outer
	AddQuad(point[(a_nSubdivisions * 2)], point[(a_nSubdivisions *3) - 1], point[(a_nSubdivisions *3)], point[(a_nSubdivisions *4)-1]); //inner

	//add the last triangle piece for bases
	AddQuad(point[a_nSubdivisions - 1], point[(a_nSubdivisions * 3) - 1], point[0], point[a_nSubdivisions*2]); //B base
	AddQuad(point[a_nSubdivisions], point[a_nSubdivisions * 3], point[(a_nSubdivisions*2)-1], point[(a_nSubdivisions * 4)-1]); //F base

	//Your code ends here
	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	std::vector<vector3> point;
	float r =  a_fOuterRadius- a_fInnerRadius;
	float R = a_fInnerRadius + r;

	float thetaL = 0;
	float thetaR = 0;
	float phiL = 0;
	float phiR = 0;

	//x = cos(thea)sin(phi)*radius
	//y = sin(theta)sin(phi)*raidus 
	//z=  cos(phi) *radius

	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		phiL = ((2 * PI) * i) / a_nSubdivisionsA;
		phiR = ((2 * PI)*(i + 1)) / a_nSubdivisionsA;

		for (int k = 0; k < a_nSubdivisionsB; k++)
		{
			thetaL = ((2 * PI) * k) / a_nSubdivisionsB;
			thetaR = ((2 * PI) * (k + 1)) / a_nSubdivisionsB;

			//(x,z,y)
			point.push_back(vector3((R + (r * cos(thetaL)))*cos(phiL), sin(thetaL)*r, (R + (r * cos(thetaL))) * sin(phiL))); //p1
			point.push_back(vector3((R + (r * cos(thetaL)))*cos(phiR), sin(thetaL)*r, (R + (r * cos(thetaL))) * sin(phiR))); //p2
			point.push_back(vector3((R + (r * cos(thetaR)))*cos(phiL), sin(thetaR)*r, (R + (r * cos(thetaR))) * sin(phiL))); //p3
			point.push_back(vector3((R + (r * cos(thetaR)))*cos(phiR), sin(thetaR)*r, (R + (r * cos(thetaR))) * sin(phiR))); //p4

			//draw quads
			//AddQuad(point[0], point[1], point[2], point[3]);
			AddQuad(point[1], point[0], point[3], point[2]);
			//remove the last 4 points
			point._Pop_back_n(4);
		}
	}

	//Your code ends here
	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	//Your code starts here
	std::vector<vector3> point;
	float thetaL = 0;
	float thetaR = 0;
	float phiL = 0;
	float phiR = 0;

	//x = cos(thea)sin(phi)*radius
	//y = sin(theta)sin(phi)*raidus 
	//z=  cos(phi) *radius

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		phiL = (PI * i) / a_nSubdivisions;
		phiR = (PI*(i + 1)) / a_nSubdivisions;

		for (int k = 0; k < a_nSubdivisions; k++)
		{
			thetaL = ((2 * PI) * k) / a_nSubdivisions;
			thetaR = ((2 * PI) * (k+1)) / a_nSubdivisions;

			//(x,z,y)
			point.push_back(vector3(cos(thetaL)*sin(phiL)*a_fRadius, cos(phiL)*a_fRadius, sin(thetaL) * sin(phiL)* a_fRadius)); //p1
			point.push_back(vector3(cos(thetaL)*sin(phiR)*a_fRadius, cos(phiR)*a_fRadius, sin(thetaL) * sin(phiR)* a_fRadius)); //p2
			point.push_back(vector3(cos(thetaR)*sin(phiL)*a_fRadius, cos(phiL)*a_fRadius, sin(thetaR) * sin(phiL)* a_fRadius)); //p3
			point.push_back(vector3(cos(thetaR)*sin(phiR)*a_fRadius, cos(phiR)*a_fRadius, sin(thetaR) * sin(phiR)* a_fRadius)); //p4

			//draw quads
			//AddQuad(point[0], point[1], point[2], point[3]);
			AddQuad(point[1], point[0], point[3], point[2]);
			//remove the last 4 points
			point._Pop_back_n(4);
		}
	}

	//Your code ends here
	CompileObject(a_v3Color);
}