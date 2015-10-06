#include "Game.h"
#include "GameState.h"
#include "DefaultGameStateManager.h"
#include "PlayState.h"

Game::Game(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
	mMainWndCaption = L"Adventures of Gawky";
	
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
	gameStateManager = std::make_shared<DefaultGameStateManager>();
	//set the intial gamestate game will use after it has started
	gameStateManager->Push(std::make_shared<PlayState>(gameStateManager));
}

void Game::OnResize()
{
	D3DApp::OnResize();
}

void Game::UpdateScene(float dt)
{
	gameStateManager->Update(dt);
}

void Game::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gameStateManager->Draw();

	// restore default states, as the SkyFX changes them in the effect file.
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
	HR(mSwapChain->Present(0, 0));
}

void Game::addDeltaTime(float dt)
{
	DeltaTimeF = dt;
}

float Game::getDeltaTime()
{
	return DeltaTimeF;
}
