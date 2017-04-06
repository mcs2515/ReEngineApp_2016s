#include "AppClass.h"
void AppClass::Update(void)
{
#pragma region DOES NOT NEED CHANGES
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
	static float fTimer = 0.0f;//creates a timer
	static uint uClock = m_pSystem->GenClock();//ask the system for a new clock id
	float fDeltaTime = m_pSystem->LapClock(uClock);//lap the selected clock
	fTimer += fDeltaTime;//add the delta time to the current clock
#pragma endregion

#pragma region YOUR CODE GOES HERE
	modelMatrix = IDENTITY_M4;

	if (fTimer >= 2.0f) {
		fTimer = 0.0f;
		if (!reverse) {
			reverse = true;
		}
		else {
			reverse = false;
		}
	}

	float degrees = MapValue(fTimer, 0.0f, 2.0f, 0.0f, 360.0f); //in 2sec go in a full circle
	float tPercentage = MapValue(fTimer, 0.0f, 2.0f, 0.0f, 1.0f); // in 2 secs

																  //quaternion
																  // parameters (degrees, axis)
	glm::quat rotate = glm::angleAxis(degrees, REAXISZ); //I forgot what axis the green line is, i remember y is the depth

	if (!reverse) {
		modelMatrix = glm::translate(glm::lerp(vector3(0.0f, 0.0f, 0.0f), vector3(0.0f, 3.0f, 0.0f), tPercentage));
	}
	else {
		modelMatrix = glm::translate(glm::lerp(vector3(0.0f, 3.0f, 0.0f), vector3(0.0f, 0.0f, 0.0f), tPercentage));
	}

	modelMatrix *= glm::toMat4(rotate);

#pragma endregion

#pragma region DOES NOT NEED CHANGES
	m_pMeshMngr->SetModelMatrix(modelMatrix, 0);//Set the model matrix to the model

	m_pMeshMngr->AddSkyboxToRenderList();//Adds a skybox to the renderlist
	m_pMeshMngr->AddInstanceToRenderList("ALL"); //Adds all loaded instances to the render list

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("\n" + m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->PrintLine("Timer: ");
	m_pMeshMngr->PrintLine(std::to_string(fTimer), REGREEN);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

