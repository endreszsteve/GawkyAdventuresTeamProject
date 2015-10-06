#include <memory>
#include <iostream>

#include "PlayState.h"
#include "LevelBuilder.h"
#include "Game.h"

using namespace std;

PlayState::PlayState(const std::shared_ptr<GameStateManager> &gameStateManager) :
					gameStateManager(gameStateManager),
					sky(0),
					lightCount(3),
					playerPosition(0.0f, 2.0f, 0.0f),
					totalEnemies(0),
					totalCollectibles(0) 
					
{};

void PlayState::Entered()
{
	playerForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	playerRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	playerUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	cam.getPlayerPos(playerPosition);
	cam.playerInfo(playerForward, playerRight, playerUp);
	cam.SetPosition(0.0f, 2.0f, -20.0f);

	dirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	dirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[1].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	dirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	dirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	dirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	dirLights[2].Direction = XMFLOAT3(-0.5f, -1.9f, -1.57735f);
	
	
}

void PlayState::Exiting()
{
	cout << "Playing state is exiting" << endl;
}

void PlayState::Update(float dt)
{
	std::printf("Playing state has been updated");
	
}

void PlayState::Draw()
{
	/*deviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	deviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	deviceContext->IASetInputLayout(InputLayouts::Basic32);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);*/

	cam.UpdateViewMatrix();
	// Set per frame constants.
	//Effects::BasicFX->SetDirLights(dirLights);
	//Effects::BasicFX->SetEyePosW(cam.GetPosition());
	//Effects::BasicFX->SetCubeMap(sky->CubeMapSRV());
	//enemies->draw(deviceContext, cam, activeTexTech);
	//objects->draw(deviceContext, cam, activeTexTech);
	//level->draw(deviceContext, cam, activeTexTech);
	//playerOne->drawPlayer(deviceContext, cam, activeTexTech);
	//sky->Draw(deviceContext, cam);
	
	sky = new Sky(md3dDevice, L"Textures//sunsetcube1024.dds", 5000.0f);

	// restore default states, as the SkyFX changes them in the effect file.
	//deviceContext->RSSetState(0);
	//deviceContext->OMSetDepthStencilState(0, 0);
	//HR(mSwapChain->Present(0, 0));
}
void PlayState::Obscuring()
{

}

void PlayState::Revealed()
{

}
