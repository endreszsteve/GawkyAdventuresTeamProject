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

	mTexMgr.Init(md3dDevice);
	mCam = new Camera();
	gameStateManager = std::make_shared<DefaultGameStateManager>();
	GameState* gs;
	gs = new PlayState(gameStateManager, md3dDevice, md3dImmediateContext, mRenderTargetView, mDepthStencilView, mSwapChain, activeTexTech, mCam);
	std::shared_ptr<PlayState> playState(new PlayState(gameStateManager, md3dDevice, md3dImmediateContext, mRenderTargetView, mDepthStencilView, mSwapChain, activeTexTech, mCam));
	//set the intial gamestate game will use after it has started
	gameStateManager->Push(playState);
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
	gameStateManager->Draw();
}

void Game::addDeltaTime(float dt)
{
	DeltaTimeF = dt;
}

float Game::getDeltaTime()
{
	return DeltaTimeF;
}
