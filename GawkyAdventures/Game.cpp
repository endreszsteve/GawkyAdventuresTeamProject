#include "Game.h"

Game::Game(HINSTANCE hInstance)
	: D3DApp(hInstance)
	//, mSky(0), mLightCount(3), mPlayerPosition(0.0f, 2.0f, 0.0f), DeltaTimeF(0.0f), totEnemy(0), totCollect(0)

{
	mMainWndCaption = L"Adventures of Gawky";

	/*
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
	*/
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
}

void Game::OnResize()
{
	D3DApp::OnResize();
	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

void Game::UpdateScene(float dt)
{

}

void Game::DrawScene()
{

}
/*
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
	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexTech;

	switch (mLightCount)
	{
	case 1:
		activeTexTech = Effects::BasicFX->Light1TexTech;	
		break;
	case 2:
		activeTexTech = Effects::BasicFX->Light2TexTech;
		break;
	case 3:
		activeTexTech = Effects::BasicFX->Light3TexTech;
		break;
	}

	// restore default states, as the SkyFX changes them in the effect file.
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
	HR(mSwapChain->Present(0, 0));
*/

void Game::addDeltaTime(float dt)
{
	DeltaTimeF = dt;
}

float Game::getDeltaTime()
{
	return DeltaTimeF;
}
