

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




	///////////////////////////////////////////////////////


	//DeltaTime getter
	float Game::getDeltaTime();
////////////////////

private:
	
	void BuildSkullGeometryBuffers();
	




	Sky* mSky;


	////////////////////////////////////////Player



	XMFLOAT3 mPlayerPosition;
	XMFLOAT3 mPlayerScale;	
	XMFLOAT4 mPlayerRotationQuad;
	XMFLOAT4 mPlayerRotation;



	XMFLOAT3 mOPlayerPosition;
	XMFLOAT3 mOPlayerScale;
	XMFLOAT4 mOPlayerRotationQuad;
	XMFLOAT4 mOPlayerRotation;




	XMMATRIX mRotation;
	XMFLOAT4X4 playerRotation;

	int mPlayerVertexOffset;
	UINT mPlayerIndexOffset;
	UINT mPlayerIndexCount;

	//textures
	ID3D11ShaderResourceView* mPlayerMapSRV;
	Material mPlayerMat;


	XMFLOAT4X4 mPlayerTexTransform;
	XMFLOAT4X4 mPlayerWorld;

	XMVECTOR PlayerForward;
	XMVECTOR PlayerRight;
	XMVECTOR PlayerUp;
	

	XMVECTOR currCharDirection;
	XMVECTOR oldCharDirection; 
	XMVECTOR charPosition;
	XMVECTOR moveDirection;


	XMVECTOR tripDirection;
	XMVECTOR tripDistance;



	// Bounding box of the Player
	XNA::AxisAlignedBox mPlayerBox;



	///////////// the Players states
	bool isAlive;
	bool isImmune;
	bool isTripping;

	bool hitFeet;
	bool onGround;
	bool fellOffMap;
	


	//////////////////////////////jumping variables
	bool isJump;
	bool isFalling;
	bool hitHead;


	
	int currentObject;
	FLOAT currGround;
	
	XMVECTOR startJumpPos;
	XMVECTOR Jump;



	//////skull stuff
	ID3D11Buffer* mSkullVB;
	ID3D11Buffer* mSkullIB;

	ID3D11Buffer* mSkySphereVB;
	ID3D11Buffer* mSkySphereIB;	
	
	XMFLOAT4X4 mSkullWorld;
	UINT mSkullIndexCount;

	Material mSkullMat;

	XNA::AxisAlignedBox mSkullBox;


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
	: D3DApp(hInstance), mSky(0),
	 mSkullVB(0), mSkullIB(0),
	
	mSkullIndexCount(0), mLightCount(3), mPlayerMapSRV(0),
	 mPlayerPosition(0.0f, 2.0f, 0.0f), mPlayerRotationQuad(0.0f, 0.0f, 0.0f, 0.0f),
	mPlayerScale(1.0f, 1.0f, 1.0f), mPlayerRotation(0.0f, 0.0f, 0.0f, 1.0f), DeltaTimeF(0.0f), isJump(0), onGround(false)
	, isFalling(false), hitHead(false), hitFeet(false), currGround(2.0f), fellOffMap(false), mOPlayerPosition(0.0f, 2.0f, 0.0f), mOPlayerRotationQuad(0.0f, 0.0f, 0.0f, 0.0f),
	mOPlayerScale(1.0f, 1.0f, 1.0f), mOPlayerRotation(0.0f, 0.0f, 0.0f, 1.0f), isAlive(true), isImmune(false), isTripping(false), totEnemy(0), totCollect(0)
	
{
	mMainWndCaption = L"Adventures of Gawky";


	////////////////player movement
	currCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	oldCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	charPosition = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	PlayerForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	PlayerRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);



	
	////send player information to the camera
	mCam.getPlayerPos(mPlayerPosition);
	mCam.playerInfo(PlayerForward, PlayerRight, PlayerUp);

	
	///initialize player
	XMVECTOR S = XMLoadFloat3(&mPlayerScale);
	XMVECTOR P = XMLoadFloat3(&mPlayerPosition);
	XMVECTOR Q = XMLoadFloat4(&mPlayerRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mPlayerRotation);
	XMStoreFloat4x4(&mPlayerWorld, XMMatrixAffineTransformation(S, rot, Q, P));


	tripDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	



	mLastMousePos.x = 0;
	mLastMousePos.y = 0;


	Jump = XMVectorSet(0.0f, 5.0f, 0.0f, 0.0f);

	mCam.SetPosition(0.0f, 2.0f, -20.0f);

	

	XMMATRIX skullScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);	
	XMMATRIX skullRotation = XMMatrixRotationY(0);	
	XMMATRIX skullOffset = XMMatrixTranslation(0.0f, 20.0f, 40.0f);

	

	XMStoreFloat4x4(&mSkullWorld, XMMatrixMultiply(skullScale * skullRotation, skullOffset));

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



	mPlayerMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mPlayerMat.Diffuse = XMFLOAT4(0.7f, 0.70f, 0.70f, 1.0f);
	mPlayerMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);
	mPlayerMat.Reflect = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);



	mSkullMat.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mSkullMat.Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mSkullMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	mSkullMat.Reflect = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
}

Game::~Game()
{
	SafeDelete(mSky);
	SafeDelete(Objects);
	SafeDelete(Level1);
	SafeDelete(theEnemies);


	
	ReleaseCOM(mPlayerMapSRV);

	ReleaseCOM(mSkullVB);
	ReleaseCOM(mSkullIB);

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
	theEnemies = new Enemies();
	Objects = new TheObjects();
	Level1 = new LevelBuilder();

	
	
	


	Objects->createObject(md3dDevice, mTexMgr, "Models\\branch.obj", L"Textures\\", 60.0f, 0.25f, 55.0f, 2);
	Objects->createObject(md3dDevice, mTexMgr, "Models\\branch.obj", L"Textures\\", 0.0f, 0.25f, 20.0f, 2);
	
	Objects->createObject(md3dDevice, mTexMgr, "Models\\Orange.obj", L"Textures\\", 80.0f, 30.0f, 20.0f, 3);
	Objects->createObject(md3dDevice, mTexMgr, "Models\\Orange.obj", L"Textures\\", 20.0f, 4.0f, 20.0f, 3);
	Objects->createObject(md3dDevice, mTexMgr, "Models\\Orange.obj", L"Textures\\", -80.0f, 10.0f, -60.0f, 3);
	


<<<<<<< HEAD
	theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\simpleenemy.obj", L"Textures\\", -80.0f, 7.0f, 78.0f, -70.f, 7.0f, 78.0f);
	theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\simpleenemy.obj", L"Textures\\", 60.0f, 3.0f, 80.0f, 60.0f, 3.0f,60.0f);
	theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\simpleenemy.obj", L"Textures\\", 0.0f, 3.0f, 35.0f, 0.0f, 3.0f, 15.0f);
=======
	theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\simpleenemy.obj", L"Textures\\", -85.0f, 9.0f, 78.0f, -65.f, 9.0f, 78.0f);
	theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\simpleenemy.obj", L"Textures\\", 55.0f, 3.0f, 80.0f, 55.0f, 3.0f, 60.0f);
	theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\simpleenemy.obj", L"Textures\\", 0.0f, 3.0f, 37.0f, 0.0f, 3.0f, 25.0f);
>>>>>>> master

	

	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Ground.obj", L"Textures\\", 0, -1.8, 0, 0, 7);

	///left side 3 platforms
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Platform2.obj", L"Textures\\", -76, 2.1, 26.6, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Platform2.obj", L"Textures\\", -76, 9.1, 50.82, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Platform2.obj", L"Textures\\", -76, 2.1, 74.2, 0, 7);
	


	///rightside 3 platforms
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Platform2.obj", L"Textures\\", 73, 2.1, 75.6, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Platform2.obj", L"Textures\\", 73, 9.1, 50.82, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\vinebox.obj", L"Textures\\", 73, 14.0, 15.82, 0, 7);
	
	///the tree's
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\sTree.obj", L"Textures\\", -56, 15.4, 86.8, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\sTree.obj", L"Textures\\", -56, 15.4, 72.8, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\sTree.obj", L"Textures\\", -56, 15.4, 58.8, 0, 7);

	/// large tree
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\treetrunk.obj", L"Textures\\", 0, 14, 47.6, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\treetop.obj", L"Textures\\", 0, 61.6, 47.6, 0, 7);

	// the Fence
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Fence.obj", L"Textures\\", 87, 5, 0, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Fence.obj", L"Textures\\", -95, 5, 0, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Fence2.obj", L"Textures\\", -5, 5, 91, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\Fence2.obj", L"Textures\\", -5, 5, -91, 0, 7);
	//cattails
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\cattail.obj", L"Textures\\", 84, 5.6, -47.6, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\cattail.obj", L"Textures\\", 77, 5.6, -47.6, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\cattail.obj", L"Textures\\", 68.25, 5.6, -47.6, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\cattail.obj", L"Textures\\", 59.5, 5.6, -47.6, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\cattail.obj", L"Textures\\", 50.75, 5.6, -47.6, 0, 7);
	/// the House
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\houseside.obj", L"Textures\\", 43.4, 14, -70.0, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\houseside.obj", L"Textures\\", 7, 14, -70.0, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\houseback.obj", L"Textures\\", 24.5, 14, -82.6, 0, 7);
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\houseroof.obj", L"Textures\\", 24.5, 34.0, -70.5, 0, 6);

	/// build the sandbox
	Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\sandbox.obj", L"Textures\\", -60.9, 1.4, -68.0, 0, 7);



	



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


	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice,
		L"Textures/WoodCrate02.dds", 0, 0, &mPlayerMapSRV, 0));


	BuildSkullGeometryBuffers();



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

	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;


	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

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

	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;


	//
	// Draw the grid, cylinders, and box without any cubemap reflection.
	// 
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{

		

		//draw the enemies
		theEnemies->draw(md3dImmediateContext, mCam, activeTexTech);

		//draw the objects
		Objects->draw(md3dImmediateContext, mCam, activeTexTech);
		//draw Level
		Level1->draw(md3dImmediateContext, mCam, activeTexTech);

		




		PlayerOne->drawPlayer(md3dImmediateContext, mCam, activeTexTech);

	
	}

	//draw Duck Model
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the skull.

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mSkullVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mSkullIB, DXGI_FORMAT_R32_UINT, 0);

		world = XMLoadFloat4x4(&mSkullWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetMaterial(mSkullMat);

		activeSkullTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);
	}


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



void Game::BuildSkullGeometryBuffers()
{
	std::ifstream fin("Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	XMFLOAT3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	//XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	//XMVECTOR vMax = XMLoadFloat3(&vMaxf3);
	std::vector<Vertex::Basic32> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;

		XMFLOAT3 P = vertices[i].Pos;

		vMin.x = MathHelper::Min(vMin.x, P.x);
		vMin.y = MathHelper::Min(vMin.y, P.y);
		vMin.z = MathHelper::Min(vMin.z, P.z);

		vMax.x = MathHelper::Max(vMax.x, P.x);
		vMax.y = MathHelper::Max(vMax.y, P.y);
		vMax.z = MathHelper::Max(vMax.z, P.z);
	}

	mSkullBox.Center = XMFLOAT3(0.5f*(vMin.x + vMax.x),
		0.5f*(vMin.y + vMax.y),
		0.5f*(vMin.z + vMax.z));

	mSkullBox.Extents = XMFLOAT3(0.5f * (vMax.x - vMin.x),
		0.5f*(vMax.y -vMin.y),
		0.5f*(vMax.z - vMin.z));


	mSkullBox.Center = XMFLOAT3(0.0f, 1.5f, 40.0f);

	
	



	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	mSkullIndexCount = 3 * tcount;
	std::vector<UINT> indices(mSkullIndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mSkullVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mSkullIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mSkullIB));
}








//////////////////////////////////////////////////////updates

void Game::UpdateScene(float dt)
{
	
	addDeltaTime(dt);
	moveDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	PlayerOne->setMoveDirection(moveDirection);
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


