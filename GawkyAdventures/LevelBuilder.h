#ifndef LevelBuilder_H
#define LevelBuilder_H


#include "BasicModel.h"
#include "xnacollision.h"
#include "GameTimer.h"
//




class BasicModel;
class Camera;
class LevelParts;
class GameTimer;

class LevelBuilder
{
public:
	LevelBuilder(ID3D11Device* device, TextureMgr& texMgr);

	~LevelBuilder();



	void addLevelParts(BasicModelInstance theLevelParts);

	void draw(ID3D11DeviceContext* dc, Camera& camera, ID3DX11EffectTechnique* activeTexTech);

	void createLevelParts(int model,FLOAT x, FLOAT y, FLOAT z, int collisionstype, int scale);


	void CreateBoundingBox();

	void update(float dt);

	void RemovemLevelPartsInstance(int number);

	std::vector<BasicModelInstance> getLevelParts();

	std::vector <XNA::AxisAlignedBox> getLevelPartsCollisions();



private:

	std::vector<BasicModelInstance> mLevelPartsInstances;
	std::vector<LevelParts*> LevelPartsclass;

	std::vector <XNA::AxisAlignedBox> LevelCollisions;

	BasicModel* anLevelParts;

	


	BasicModel* mGround;
	BasicModel* mPlatform;
	BasicModel* mVinebox;
	BasicModel* mSmallTree;
	BasicModel* mTreeTrunk;
	BasicModel* mTreeTop;
	BasicModel* mFence1;
	BasicModel* mFence2;
	BasicModel* mCattail;
	BasicModel* mHouseSide;
	BasicModel* mHouseBack;
	BasicModel* mHouseRoof;
	BasicModel* mSandBox;
	BasicModel* mFencePart1;
	BasicModel* mFencePart2;
	BasicModel* mlvl2Ground;
	BasicModel* mTestBarn;
	BasicModel* mBarnBack;
	BasicModel* mBarnFrontSide;
	BasicModel* mBarnFrontTop;
	BasicModel* mBarnRoof;
	BasicModel* mBarnSide;
	BasicModel* mBarnFrontSide2;


	BasicModelInstance theLevelParts;
	XNA::AxisAlignedBox LevelPartsBox;


	GameTimer timer;

	FLOAT DeltaTime;

	LevelParts* newLevelParts;






};

#endif