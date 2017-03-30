#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("E12 - Instance Rendering");
	m_v4ClearColor = vector4(REBLACK, 1.0f);
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pMesh = new MyMesh();
	
	//startup global variables
	numRows = 4; //total of rows
	size = 1.0f;  //of a side

	//Creating the Mesh points
	m_pMesh->AddVertexPosition(vector3(-size, -size, 0.0f));
	m_pMesh->AddVertexColor(REGREEN);
	m_pMesh->AddVertexPosition(vector3(size, -size, 0.0f));
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexPosition(vector3(0.0f, size, 0.0f));
	m_pMesh->AddVertexColor(REBLUE);

	//yes http://www.geeksforgeeks.org/pascal-triangle/

	for (int row = 1; row <= numRows; row++) //row = current row number
	{
		int C = 1;  // used to represent C(row, i)
		for (int i = 1; i <= row; i++)
		{
			//for every odd number
			if (C % 2 != 0) {
				//creates a new mat4 obj with its own x,y,z
				triangles.push_back(
					//move the first triangle to the left more each time we go to next row
					//move the next triangle to the right of the previous one 
					glm::translate(vector3((-0.5f * (row - 1) + (i - 1)) * size * 2, (-row) * size * 2, 0.0f)) * glm::rotate(IDENTITY_M4, 180.0f, REAXISZ)
				);
			}
			C = C * (row - i) / i;
		}
	}

	numOfTri = triangles.size();

	m_fMatrixArray = new float[numOfTri * 16];

	for (int i = 0; i < numOfTri; i++) {
		const float* m4MVP = glm::value_ptr(triangles[i]); //get the x,y,z values of each triangle object in list
		memcpy(&m_fMatrixArray[i * 16], m4MVP, 16 * sizeof(float)); //generate the vertices
	}


	//Compiling the mesh
	m_pMesh->CompileOpenGL3X();
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();


	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	ClearScreen();
	
	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	m_pMesh->RenderList(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_fMatrixArray, numOfTri);//Rendering nObject(s)
	
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}