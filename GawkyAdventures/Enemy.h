#ifndef Enemy_H
#define Enemy_H


#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"

#include "d3dUtil.h"
#include "TextureMgr.h"
#include "BasicModel.h"
#include "xnacollision.h"


class BasicModel;
class Camera;



class Enemy
{
public:
	Enemy();
	~Enemy();

	void move(FLOAT dt);

	void update(FLOAT dt);

	///setters
	void setModelScale(XMMATRIX& Scale);
	void setModelRot(XMMATRIX& Rot);
	void setModelOffset(XMMATRIX& Offset);
	void setModel(BasicModel* Enemy);
	void setBasicMInstance(BasicModelInstance model);
	void setAABB(XNA::AxisAlignedBox* tempEnemyBox);
	void setWorld(XMFLOAT4X4 tempEnemyWorld);
	void SetPositionOne(FLOAT x, FLOAT y, FLOAT z);
	void SetPositionTwo(FLOAT x, FLOAT y, FLOAT z);
	void SetPositionThree(FLOAT x, FLOAT y, FLOAT z);
	void SetPositionFour(FLOAT x, FLOAT y, FLOAT z);
	void setSpeed(FLOAT theSpeed);

	void setScale(FLOAT scale);
	void setcollisiontype(int collisiontype);

	///getters
	XMMATRIX getModelScale();
	XMMATRIX getModelRot();
	XMMATRIX getModelOffset();
	BasicModel* getModel();
	BasicModelInstance getBasicMInstance();
	XNA::AxisAlignedBox* getEnemyAABB();
	XMFLOAT4X4 GetWorld();
	XMFLOAT3 getEnemyPosition();
	FLOAT getScale();
	int getcollisiontype();

	
private:


	XMMATRIX modelScale;
	XMMATRIX modelRot;
	XMMATRIX modelOffset;

	XMFLOAT4X4 mEnemyWorld;


	XMFLOAT4X4 mEnemyStartingWorld;


	XMFLOAT3 mEnemyPosition;
	XMFLOAT3 mEnemyScale;
	XMFLOAT4 mEnemyRotationQuad;
	XMFLOAT4 mEnemyRotation;

	XMFLOAT3 mEnemyPositionOne;
	XMFLOAT3 mEnemyPositionTwo;
	XMFLOAT3 mEnemyPositionThree;
	XMFLOAT3 mEnemyPositionFour;

	BasicModel* anEnemy;

	BasicModelInstance theEnemy;

	XNA::AxisAlignedBox* EnemyBox;


	XMVECTOR currCharDirection;
	XMVECTOR oldCharDirection;
	XMVECTOR charPosition;
	XMVECTOR moveDirection;

	XMVECTOR EnemyForward;
	XMVECTOR EnemyRight;
	XMVECTOR EnemyUp;
	//// last travel point the enemy hit
	XMFLOAT3 lastPoint;

	FLOAT speed;

	XMVECTOR direction;

	int travelToPoint;

	int timesThrough;
	int mcollisiontype;
	FLOAT theScale;










};


#endif 