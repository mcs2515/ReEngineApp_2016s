#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - YOUR USER NAME GOES HERE"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	m_m4Sun = IDENTITY_M4;
	m_m4Earth = IDENTITY_M4;
	m_m4Moon = IDENTITY_M4;
	sunLocation = vector3(0.0f, 0.0f, 0.0f);

	//Setting the days duration
	m_fDay = 1.0f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	//revloution angleAxis
	//orbits SLERP

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	nEarthOrbits = static_cast<int>(fRunTime/(fEarthHalfOrbTime*2)); //run time divided by a full orbit
	nEarthRevolutions = static_cast<int>(fRunTime / (fEarthHalfRevTime * 2)); // divided by a earth full rev
	nMoonOrbits = static_cast<int>(fRunTime / (fMoonHalfOrbTime * 2)); //divided by afull rev

	float eOrbitPercentage = MapValue(static_cast<float>(fRunTime), 0.0f, static_cast<float>(fEarthHalfOrbTime), 0.0f, 1.0f); //at 100% = half orbit
	float eRevAngle = MapValue(static_cast<float>(fRunTime), 0.0f, static_cast<float>(fEarthHalfRevTime), 0.0f, 180.0f); //degrees
	float mOrbitPercentage = MapValue(static_cast<float>(fRunTime), 0.0f, static_cast<float>(fMoonHalfOrbTime), 0.0f, 1.0f);

	//start, end, percentage between the two
	glm::quat eOrbitQuad = glm::mix(glm::quat(), glm::quat(vector3(0.0f, PI, 0.0f)), eOrbitPercentage); //things are in radians
	glm::quat mOrbitQuad = glm::mix(glm::quat(), glm::quat(vector3(0.0f, PI, 0.0f)), mOrbitPercentage);
	glm::quat eRevQuad = glm::angleAxis(eRevAngle, REAXISY); //in degrees

	//blahhh
	m_m4Sun = glm::translate(IDENTITY_M4, sunLocation);

	//get current orbit location
	m_m4Earth = glm::toMat4(eOrbitQuad);
	//move out to where sun is
	m_m4Earth *= glm::translate(m_m4Sun, vector3(0.0f, 0.0f, 0.0f));

	//kick the earth out from the center of the sun
	m_m4Earth = glm::translate(m_m4Earth, vector3(11.0f, 0.0f, 0.0f));
	//revolve on its axis
	m_m4Earth *= glm::toMat4(eRevQuad);
	
	//Calculate the position of the Moon-----------------------------------------------------------------------------

	//move out to where earth is

	//get earth's current orbit location
	m_m4Moon = glm::toMat4(eOrbitQuad);	
	//move out to where sun is
	m_m4Moon *= glm::translate(m_m4Sun, vector3(0.0f, 0.0f, 0.0f));
	//move out to where the earth is
	m_m4Moon = glm::translate(m_m4Moon, vector3(11.0f, 0.0f, 0.0f)); 
	//move 2 units away from earth
	m_m4Moon = glm::translate(m_m4Moon, vector3(0.524f*2.0f, 0.0f, 0.0f)); //relative to earth's size

	//get current orbit location
	m_m4Moon *= glm::toMat4(mOrbitQuad);

	//Scale models appropriately
	m_m4Moon = glm::scale(m_m4Moon, vector3(0.524f*0.27f, 0.524f*0.27f, 0.524f*0.27f)); //relative to the earth's size
	m_m4Sun = glm::scale(m_m4Sun, vector3(5.936f, 5.936f, 5.936f));
	m_m4Earth = glm::scale(m_m4Earth, vector3(0.524f, 0.524f, 0.524f));

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m_m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m_m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m_m4Moon, "Moon");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);

	m_pMeshMngr->Print(std::to_string(sunLocation.x));
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}