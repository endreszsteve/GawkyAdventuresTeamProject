

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "Camera.h"
#include "Sky.h"
#include "xnacollision.h"
#include "d3dUtil.h"
#include "TextureMgr.h"
#include "BasicModel.h"
#include "RenderStates.h"
#include "Enemies.h"
#include "TheObjects.h"
#include "LevelBuilder.h"
#include "Player.h"
#include "ModelEnum.cpp"





class Game : public D3DApp
{
public:
	Game(HINSTANCE hInstance);
	~Game();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);







	//DeltaTime getter
	float Game::getDeltaTime();
////////////////////

private:
	
	
	




	Sky* mSky;


	////////////////////////////////////////Player



	XMFLOAT3 mPlayerPosition;


	XMVECTOR PlayerForward;
	XMVECTOR PlayerRight;
	XMVECTOR PlayerUp;
	

















	///////lighting
	DirectionalLight mDirLights[3];

	UINT mLightCount;



	Camera mCam;
	


	//mouse
	POINT mLastMousePos;



	/// DeltaTime

	// The DeltaTime's velocity vector

	void addDeltaTime(float dt);
	

	XMFLOAT3 DeltaTime;
	
	float DeltaTimeF;	
	
	/////// OBJ Model files
	TextureMgr mTexMgr;


	std::vector<BasicModelInstance> mModelInstances;
	std::vector <XNA::AxisAlignedBox> LevelCollisions;
	
	Enemies* theEnemies;
	TheObjects* Objects;

	LevelBuilder* Level1;

	Player* PlayerOne;


	int totEnemy;
	int totCollect;



};






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	

	Game theApp(hInstance);

	

	if (!theApp.Init(hInstance))
		return 0;

	return theApp.Run();
}


Game::Game(HINSTANCE hInstance)
	: D3DApp(hInstance), mSky(0), mLightCount(3), 
	 mPlayerPosition(0.0f, 2.0f, 0.0f), DeltaTimeF(0.0f), totEnemy(0), totCollect(0)
	
{
	mMainWndCaption = L"Adventures of Gawky";


	PlayerForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	PlayerRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	PlayerUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	////send player information to the camera
	mCam.getPlayerPos(mPlayerPosition);
	mCam.playerInfo(PlayerForward, PlayerRight, PlayerUp);

	mCam.SetPosition(0.0f, 2.0f, -20.0f);



	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(-0.5f, -1.9f, -1.57735f);


}

Game::~Game()
{
	SafeDelete(mSky);
	SafeDelete(Objects);
	SafeDelete(Level1);
	SafeDelete(theEnemies);


	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

bool Game::Init(HINSTANCE hInstance)
{
	if (!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);

	mTexMgr.Init(md3dDevice);
	
	mSky = new Sky(md3dDevice, L"Textures//sunsetcube1024.dds", 5000.0f);


	/// create the player
	PlayerOne = new Player(md3dDevice, mTexMgr, "Models\\gawky.obj", L"Textures\\", 0.0f, 10.0f, 0.0f);
	
	//// load  the level models
	theEnemies = new Enemies(md3dDevice, mTexMgr);
	Objects = new TheObjects(md3dDevice, mTexMgr);
	Level1 = new LevelBuilder(md3dDevice, mTexMgr);

	
	
	
	/*
	Objects->createObject(branch, 60.0f, 0.25f, 55.0f, ctStumble, 1);
	Objects->createObject(branch, 0.0f, 0.25f, 20.0f, ctStumble, 1);
	
	Objects->createObject(orange, 80.0f, 30.0f, 20.0f, ctCollect, 1);
	Objects->createObject(orange, 20.0f, 4.0f, 20.0f, ctCollect, 1);
	Objects->createObject(orange, -80.0f, 10.0f, -60.0f, ctCollect, 1);

	Objects->createObject(gatetwo, -95.0f, 8.5f, 0.0f, ctStumble, 7);
	


	theEnemies->createEnemy(simpleEnemy, -85.0f, 9.0f, 78.0f, -65.f, 9.0f, 78.0f);
	theEnemies->createEnemy(simpleEnemy, 55.0f, 3.0f, 80.0f, 55.0f, 3.0f, 60.0f);
	theEnemies->createEnemy(simpleEnemy, 0.0f, 3.0f, 45.0f, 0.0f, 3.0f, 25.0f);

	

	Level1->createLevelParts(Ground,  0, -1.8, 0, 0, 7);

	///left side 3 platforms
	Level1->createLevelParts(Platform, -76, 2.1, 26.6, 0, 7);
	Level1->createLevelParts(Platform, -76, 9.1, 50.82, 0, 7);
	Level1->createLevelParts(Platform, -76, 2.1, 74.2, 0, 7);
	


	///rightside 3 platforms
	Level1->createLevelParts(Platform, 73, 2.1, 75.6, 0, 7);
	Level1->createLevelParts(Platform, 73, 9.1, 50.82, 0, 7);
	Level1->createLevelParts(Platform, 73, 14.0, 15.82, 0, 7);
	
	
	///the tree's
	Level1->createLevelParts(SmallTree, -56, 15.4, 86.8, 0, 7);
	Level1->createLevelParts(SmallTree, -56, 15.4, 72.8, 0, 7);
	Level1->createLevelParts(SmallTree, -56, 15.4, 58.8, 0, 7);

	/// large tree
	Level1->createLevelParts(TreeTrunk, 0, 14, 57.6, 0, 7);
	Level1->createLevelParts(TreeTop, 0, 61.6, 57.6, 0, 7);

	// the Fence
	Level1->createLevelParts(Fence1, 87, 5, 0, 0, 7);
	Level1->createLevelParts(FencePart2, -95, 6, 48, 0, 7);
	Level1->createLevelParts(FencePart2, -95, 6, -48, 0, 7);


	Level1->createLevelParts(Fence2, -5, 6, 91, 0, 7);
	Level1->createLevelParts(Fence2, -5, 6, -91, 0, 7);
	//cattails
	Level1->createLevelParts(Cattail, 84, 5.6, -47.6, 0, 7);
	Level1->createLevelParts(Cattail, 77, 5.6, -47.6, 0, 7);
	Level1->createLevelParts(Cattail, 68.25, 5.6, -47.6, 0, 7);
	Level1->createLevelParts(Cattail, 59.5, 5.6, -47.6, 0, 7);
	Level1->createLevelParts(Cattail,  50.75, 5.6, -47.6, 0, 7);
	/// the House
	Level1->createLevelParts(HouseSide, 43.4, 14, -70.0, 0, 7);
	Level1->createLevelParts(HouseSide, 7, 14, -70.0, 0, 7);
	Level1->createLevelParts(HouseBack, 24.5, 14, -82.6, 0, 7);
	Level1->createLevelParts(HouseRoof, 24.5, 34.0, -70.5, 0, 6);

	/// build the sandbox
	Level1->createLevelParts(SandBox, -60.9, 1.4, -68.0, 0, 7);
	*/


	////2nd section of level
	////offset everything by -250 and -15
	
	Level1->createLevelParts(lvl2Ground, 0, -5, 0.0, ctLevel, 14, 0);
	
	//the barn
	int x2o = -80;
	int y2o = 15;
	int z2o = 90;
	Level1->createLevelParts(barnback, 1.56 + x2o, 7.8 + y2o, 46.8 + z2o, ctLevel, 23, 0);
	Level1->createLevelParts(barnside, 44.34 + x2o, 8.58 + y2o, 10.92 + z2o, ctLevel, 25, 0);
	Level1->createLevelParts(barnside, -44.34 + x2o, 8.58 + y2o, 10.92 + z2o, ctLevel, 25, 0);
	Level1->createLevelParts(barnfrontside, -36.74 + x2o,8.74 + y2o, -24.96 + z2o, ctLevel, 25, 0);
	Level1->createLevelParts(barnfrontside2, 37.21 + x2o, 7.8 + y2o, -24.96 + z2o, ctLevel, 25, 0);
	Level1->createLevelParts(barnfronttop, 0.47 + x2o, 27.38 + y2o, -24.96 + z2o, ctLevel, 25, 0);
	Level1->createLevelParts(barnroof, 1.56 + x2o, 60.84 + y2o, 12.79 + z2o, ctLevel, 25, 0);
	/// 1.57 = 90 degrees
	//bails
	

	//bottom Row
	Level1->createLevelParts(roundbail, 20, 9,  20, ctLevel, 14, 0);
	Level1->createLevelParts(squarebail, -114, 0, 129, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -114, 11.6, 129, ctLevel, 7, 0);

	Level1->createLevelParts(squarebail, -114, 0, 120.2, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -114, 0, 111.48, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -114, 0, 102.9, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -100, 0, 128.98, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -86.68, 0, 128.98, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -86.68, 5.5, 128.98, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -86.68, 11.6, 128.98, ctLevel, 7, 0);

	Level1->createLevelParts(squarebail, -103, 0, 118, ctLevel, 7, 1.57);
	Level1->createLevelParts(squarebail, -93.9, 0, 118, ctLevel, 7, 1.57);
	Level1->createLevelParts(squarebail, -84.98, 0, 118, ctLevel, 7, 1.57);
	Level1->createLevelParts(squarebail, -103, 0, 104.9, ctLevel, 7, 1.57);
	Level1->createLevelParts(squarebail, -93.9, 0, 104.9, ctLevel, 7, 1.57);
	Level1->createLevelParts(squarebail, -85, 0, 104.9, ctLevel, 7, 1.57);
	Level1->createLevelParts(squarebail, -114, 5.5, 120.2, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -114, 11.6, 120.2, ctLevel, 7, 0);

	Level1->createLevelParts(squarebail, -114, 5.5, 111.48, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -100.6, 5.5, 128.9, ctLevel, 7,0);
	Level1->createLevelParts(squarebail, -100.6,11.6, 128.9, ctLevel, 7, 0);
	
	//
	Level1->createLevelParts(squarebail, -72.7, 0, 128.9, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 5.5, 128.9, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 11.6, 128.9, ctLevel, 7, 0);

	Level1->createLevelParts(squarebail, -72.7, 0,119.9, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 5.5, 119.9, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 11.6, 119.9, ctLevel, 7, 0);

	Level1->createLevelParts(squarebail, -72.7, 0,111.2, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 5.5, 111.2, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 11.6, 111.2, ctLevel, 7, 0);

	Level1->createLevelParts(squarebail, -72.7, 0, 102.4, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 5.5, 102.4, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 11.6, 102.4, ctLevel, 7, 0);
	
	Level1->createLevelParts(squarebail, -72.7, 0, 94, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 5.5, 94, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -72.7, 11.6, 94, ctLevel, 7, 0);
	//

	Level1->createLevelParts(squarebail, -59, 11.6, 128.9, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -59, 11.6, 119.9, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -59, 11.6, 111.2, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -59, 11.6, 102.4, ctLevel, 7, 0);

	Level1->createLevelParts(squarebail, -59, 0, 94, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -59, 5.5, 94, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -59, 11.6, 94, ctLevel, 7, 0);

	//
	Level1->createLevelParts(squarebail, -45.3, 11.6, 128.9, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -45.3, 11.6, 119.9, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -45.3, 11.6, 111.2, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -45.3, 11.6, 102.4, ctLevel, 7, 0);

	Level1->createLevelParts(squarebail, -45.3, 0, 94, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -45.3, 5.5, 94, ctLevel, 7, 0);
	Level1->createLevelParts(squarebail, -45.3, 11.6, 94, ctLevel, 7, 0);



	theEnemies->createEnemy(tractor, 4.0f, 13, 88.0f, 4, 13, 50, 40,13, 50, 40, 13, 88, 1);


	//theEnemies->createEnemy(simpleEnemy, 4.0f, 13, 88.0f, 4, 13, -96, 1, 0, 0, 0, 0, 0, 0);
	



	theEnemies->CreateBoundingBox();
	Objects->CreateBoundingBox();
	Level1->CreateBoundingBox();	
	

	////////

	/////////////////////////////////////////////////////////
	std::vector <XNA::AxisAlignedBox> temp;	
	temp = Level1->getLevelPartsCollisions();
	for (UINT i = 0; i < temp.size(); i++)
	{

		LevelCollisions.push_back(temp[i]);


	}

	temp = Objects->getObjectCollisions();
	for (UINT i = 0; i < temp.size(); i++)
	{

		LevelCollisions.push_back(temp[i]);
		totCollect++;

	}

	
	temp = theEnemies->getEnemyCollisions();
	for (UINT i = 0; i < temp.size(); i++)
	{

		LevelCollisions.push_back(temp[i]);
		totEnemy++;


	}


	//////////////////////////////////////////////////////////





	



	return true;
}

void Game::OnResize()
{
	D3DApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

void Game::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	

	mCam.UpdateViewMatrix();
	

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);


	Effects::BasicFX->SetEyePosW(mCam.GetPosition());
	Effects::BasicFX->SetCubeMap(mSky->CubeMapSRV());



	// Figure out which technique to use.  Skull does not have texture coordinates,
	// so we need a separate technique for it, and not every surface is reflective,
	// so don't pay for cubemap look up.

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexTech;
	ID3DX11EffectTechnique* activeReflectTech = Effects::BasicFX->Light1TexReflectTech;
	ID3DX11EffectTechnique* activeSkullTech = Effects::BasicFX->Light1ReflectTech;
	switch (mLightCount)
	{
	case 1:
		activeTexTech = Effects::BasicFX->Light1TexTech;
		activeReflectTech = Effects::BasicFX->Light1TexReflectTech;
		activeSkullTech = Effects::BasicFX->Light1ReflectTech;
		break;
	case 2:
		activeTexTech = Effects::BasicFX->Light2TexTech;
		activeReflectTech = Effects::BasicFX->Light2TexReflectTech;
		activeSkullTech = Effects::BasicFX->Light2ReflectTech;
		break;
	case 3:
		activeTexTech = Effects::BasicFX->Light3TexTech;
		activeReflectTech = Effects::BasicFX->Light3TexReflectTech;
		activeSkullTech = Effects::BasicFX->Light3ReflectTech;
		break;
	}
	


		

		//draw the enemies
		theEnemies->draw(md3dImmediateContext, mCam, activeTexTech);

		//draw the objects
		Objects->draw(md3dImmediateContext, mCam, activeTexTech);
		//draw Level
		Level1->draw(md3dImmediateContext, mCam, activeTexTech);

		



		//draw player
		PlayerOne->drawPlayer(md3dImmediateContext, mCam, activeTexTech);

	
	



	////////////////////////////////////////
	mSky->Draw(md3dImmediateContext, mCam);


	// restore default states, as the SkyFX changes them in the effect file.
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);

	HR(mSwapChain->Present(0, 0));
}




///the mouse functions are not being used
void Game::OnMouseDown(WPARAM btnState, int x, int y)
{

	//theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\Hat.obj", L"Textures\\", x, 4.0f, y);

	SetCapture(mhMainWnd);
}

void Game::OnMouseUp(WPARAM btnState, int x, int y)
{
		ReleaseCapture();
}

void Game::OnMouseMove(WPARAM btnState, int x, int y)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////updates

void Game::UpdateScene(float dt)
{
	
	addDeltaTime(dt);

	theEnemies->update(dt);

	int levelColsize = LevelCollisions.size();
	int tempOtherObject;

	//////updates the enemy collisions as they move
	std::vector <XNA::AxisAlignedBox> temp;
	//original value = 3
	temp = theEnemies->getEnemyCollisions();

	std::vector <XNA::AxisAlignedBox> tempObject;
	//original value = 5
	tempObject = Objects->getObjectCollisions();

	std::vector <XNA::AxisAlignedBox> tempLevel;
	//original value = 26
	tempLevel = Level1->getLevelPartsCollisions();

	tempOtherObject = tempObject.size() + temp.size() + tempLevel.size();


	int tempSize = temp.size();


	int sizeDifference = 0;

	sizeDifference = LevelCollisions.size() - tempOtherObject;


	if (sizeDifference > 0)
	{

		int something = 0;
		LevelCollisions.pop_back();



		int j = 0;
		for (UINT i = tempLevel.size(); i < (tempLevel.size() + tempObject.size()); i++, j++)
		{

			LevelCollisions[i] = tempObject[j];

		}

		if (temp.size() >= 0)
		{

			j = 0;
			for (UINT i = (tempLevel.size() + tempObject.size()); i < (tempLevel.size() + tempObject.size() + temp.size()); i++, j++)
			{

				LevelCollisions[i] = temp[j];
				LevelCollisions[i].Center = temp[j].Center;
				

			}
		}

	}else 
	{
		int	j = 0;
		for (UINT i = tempObject.size() + tempLevel.size(); i < tempOtherObject; i++, j++)
		{
			LevelCollisions[i] = temp[j];

		}
	
	
	}


	/////////////////////////////

	



	PlayerOne->setLevelCollisions(LevelCollisions);
	

	

	/////////////////////////////


	XMVECTOR desiredCharDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR playerPos = XMLoadFloat3(&mPlayerPosition);


	XMVECTOR camRight = XMLoadFloat3(&mCam.GetRight());
	XMVECTOR camForward = XMLoadFloat3(&mCam.GetLook());
	XMVECTOR camUp = XMLoadFloat3(&mCam.GetUp());

	XMVECTOR multiply = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);

	camUp = XMVectorAdd(camUp, multiply);
	
	
	
	bool jumpChar = false;


	bool moveChar = false;


	if (GetAsyncKeyState('W') & 0x8000)
	{
		desiredCharDir += (camForward);

		moveChar = true;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		desiredCharDir += -(camForward);

		moveChar = true;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		desiredCharDir += (camRight);

		moveChar = true;
	}



	if (GetAsyncKeyState('D') & 0x8000)
	{
		desiredCharDir += -(camRight);

		moveChar = true;
	}


	if (GetAsyncKeyState('Q') & 0x8000)
	{
		float dy = 1.5 * dt;
		mCam.RotateY(-dy);


	}

	if (GetAsyncKeyState('E') & 0x8000)
	{

		float dy = 1.5 * dt;
		mCam.RotateY(dy);


	}


	if (GetAsyncKeyState('R') & 0x8000)
	{
		float dy = 0.25 * dt;
		mCam.Pitch(dy);


	}

	if (GetAsyncKeyState('F') & 0x8000)
	{

		float dy = 0.25 * dt;
		mCam.Pitch(-dy);


	}


	
	
	if (PlayerOne->getOnGround() == true)
	{


	


		if (GetAsyncKeyState('J') & 0x8000)
		{
			desiredCharDir += camUp;
			
			moveChar = true;
		}

	}


	XMVECTOR addGravity = XMVectorSet(0.0f, -30 * DeltaTimeF, 0.0f, 0.0f);

	XMFLOAT3 tGrav;
	XMStoreFloat3(&tGrav, addGravity);

	XMVECTOR tGravity = XMLoadFloat3(&tGrav);

	if (PlayerOne->getOnGround() == true)
	{

	}
	else if (PlayerOne->getOnGround() == false)
	{

		desiredCharDir += addGravity;
	}







	//		
	// Switch the number of lights based on key presses.
	//
	if (GetAsyncKeyState('0') & 0x8000)

	{
		mLightCount = 0;

		
	}
	if (GetAsyncKeyState('1') & 0x8000)
		mLightCount = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		mLightCount = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		mLightCount = 3;



	////send player information to the camera

	mCam.getPlayerPos(PlayerOne->getPlayerPosition());
	mCam.getDeltaTime(dt);

	mCam.moveCam();

	PlayerOne->move(dt, desiredCharDir, theEnemies, Objects );

	PlayerOne->update();
	


}






void Game::addDeltaTime(float dt)
{
	
	
	DeltaTimeF = dt;
	

}

float Game::getDeltaTime()
{


	return DeltaTimeF;
}


