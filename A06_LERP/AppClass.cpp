#include "AppClass.h"

void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	//list of locations
	locations.push_back(vector3(-4.0f, -2.0f, 5.0f));
	locations.push_back(vector3(1.0f, -2.0f, 5.0f));
	locations.push_back(vector3(-3.0f, -1.0f, 3.0f));
	locations.push_back(vector3(2.0f, -1.0f, 3.0f));
	locations.push_back(vector3(-2.0f, 0.0f, 0.0f));
	locations.push_back(vector3(3.0f, 0.0f, 0.0f));
	locations.push_back(vector3(-1.0f, 1.0f, -3.0f));
	locations.push_back(vector3(4.0f, 1.0f, -3.0f));
	locations.push_back(vector3(0.0f, 2.0f, -5.0f));
	locations.push_back(vector3(5.0f, 2.0f, -5.0f));
	locations.push_back(vector3(1.0f, 3.0f, -5.0f));
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	//m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "WallEye");

	//Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->Print(std::to_string(fRunTime)); //print the timer

	//draw spheres for each location
	for (int i = 0; i < locations.size(); i++) {
		matrix4 m4SpherePosition = glm::translate(locations[i])*glm::scale(0.1f, 0.1f, 0.1f);
		m_pMeshMngr->AddSphereToRenderList(m4SpherePosition, RERED, SOLID);
	}


	//mapping
	float percentage = MapValue(static_cast<float>(fRunTime), 0.0f, fDuration, 0.0f, 1.0f); //timer variable, between 0-5secs, map it to a 0-1 scale

	int index; 
	for (index = 0; percentage >= 1.0; index++) {
		percentage -= 1.0f; //get the percentage
	}

	index = index % locations.size(); //modulus used to determine the index num without going over the size of the list

	vector3 end = index + 1 >= locations.size() ? locations[0] : locations[index+1]; //reset to position 0 if it reaches last point 
	vector3 v3Current = glm::lerp(locations[index], end, percentage); //move across different points
	matrix4 m4WallEyePosition = glm::translate(v3Current);
	m_pMeshMngr->SetModelMatrix(m4WallEyePosition, "WallEye");

#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}