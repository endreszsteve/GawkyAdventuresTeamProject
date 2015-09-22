
#include "Enemy.h"
#include "Effects.h"



#include "Camera.h"





Enemy::Enemy() : mEnemyPosition(0.0f, 0.0f, 0.0f), mEnemyScale(3.0f, 3.0f, 3.0f), mEnemyRotation(0.0f, 0.0f, 0.0f, 1.0f),
<<<<<<< HEAD
				mEnemyRotationQuad(0.0f, 0.0f, 0.0f, 0.0f), mEnemyPositionOne(0.0f, 0.0f, 0.0f), mEnemyPositionTwo(0.0f, 0.0f, 0.0f)
=======
mEnemyRotationQuad(0.0f, 0.0f, 0.0f, 0.0f), mEnemyPositionOne(0.0f, 0.0f, 0.0f), mEnemyPositionTwo(0.0f, 0.0f, 0.0f), travelToPoint(1)
>>>>>>> master
{



	///initialize player
	XMVECTOR S = XMLoadFloat3(&mEnemyScale);
	XMVECTOR P = XMLoadFloat3(&mEnemyPosition);
	XMVECTOR Q = XMLoadFloat4(&mEnemyRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mEnemyRotation);
	XMStoreFloat4x4(&mEnemyWorld, XMMatrixAffineTransformation(S, rot, Q, P));


	currCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	oldCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	charPosition = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	EnemyForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	EnemyRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);


	direction = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);


}


Enemy::~Enemy()
{
}

//setters
void Enemy::setModelScale(XMMATRIX& Scale)

{

	modelScale = Scale;

}

void Enemy::setModelRot(XMMATRIX& Rot)
{
	modelRot = Rot;

}
void Enemy::setModelOffset(XMMATRIX& Offset)
{


	modelOffset = Offset;

}

void Enemy::setModel(BasicModel* Enemy)
{

	anEnemy = Enemy;

}

void Enemy::setBasicMInstance(BasicModelInstance model)
{

	theEnemy = model;
}

void Enemy::setAABB(XNA::AxisAlignedBox* tempEnemyBox)
{

	EnemyBox = tempEnemyBox;
}


void Enemy::setWorld(XMFLOAT4X4 tempEnemyWorld)
{


	mEnemyWorld = tempEnemyWorld;


	XMMATRIX tempWorld = XMLoadFloat4x4(&mEnemyWorld);


	XMVECTOR Scale;
	XMVECTOR Position;
	XMVECTOR Rotation;


	XMMatrixDecompose(&Scale, &Rotation, &Position, tempWorld);

	XMStoreFloat3(&mEnemyPosition, Position);

	XMStoreFloat3(&mEnemyScale, Scale);

	XMStoreFloat4(&mEnemyRotation, Rotation);





	XMVECTOR S = XMLoadFloat3(&mEnemyScale);
	XMVECTOR P = XMLoadFloat3(&mEnemyPosition);
	XMVECTOR Q = XMLoadFloat4(&mEnemyRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mEnemyRotation);





	XMStoreFloat4x4(&mEnemyStartingWorld, XMMatrixAffineTransformation(S, rot, Q, P));











}




//getters
XMMATRIX Enemy::getModelScale()
{

	return modelScale;
}
XMMATRIX Enemy::getModelRot()
{


	return modelRot;
}
XMMATRIX Enemy::getModelOffset()
{


	return modelOffset;
}

BasicModel* Enemy::getModel()
{

	return anEnemy;
}

BasicModelInstance Enemy::getBasicMInstance()
{


	return theEnemy;
}

XNA::AxisAlignedBox* Enemy::getEnemyAABB()
{


	return EnemyBox;
}

XMFLOAT4X4 Enemy::GetWorld()
{


	return mEnemyWorld;
}


XMFLOAT3 Enemy::getEnemyPosition()
{


	return mEnemyPosition;
}

void Enemy::SetPositionOne(FLOAT x, FLOAT y, FLOAT z)
{

<<<<<<< HEAD
	XMVECTOR temp = XMVectorSet (x, y, z, 0.0f);
=======
	XMVECTOR temp = XMVectorSet(x, y, z, 0.0f);
>>>>>>> master
	XMStoreFloat3(&mEnemyPositionOne, temp);

}


void Enemy::SetPositionTwo(FLOAT x, FLOAT y, FLOAT z)
{

	XMVECTOR temp = XMVectorSet(x, y, z, 0.0f);
	XMStoreFloat3(&mEnemyPositionTwo, temp);

<<<<<<< HEAD
=======
	mEnemyPositionTwo;
>>>>>>> master
}








void Enemy::update(FLOAT dt)
{

	move(dt);

	XMVECTOR S = XMLoadFloat3(&mEnemyScale);
	XMVECTOR P = XMLoadFloat3(&mEnemyPosition);
	XMVECTOR Q = XMLoadFloat4(&mEnemyRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mEnemyRotation);
	XNA::AxisAlignedBox tempEnemyBox;

	tempEnemyBox.Center = mEnemyPosition;

	EnemyBox = &tempEnemyBox;


	XMStoreFloat4x4(&mEnemyWorld, XMMatrixAffineTransformation(S, rot, Q, P));

}



void Enemy::move(FLOAT dt)
{

	XMMATRIX startingworldMatrix = XMLoadFloat4x4(&mEnemyStartingWorld);
	XMVECTOR S;
	XMVECTOR P;
	XMVECTOR Q;

	XMMatrixDecompose(&S, &Q, &P, startingworldMatrix);


	XMVECTOR enemyPosition = XMLoadFloat3(&mEnemyPosition);



	FLOAT newPos = XMVectorGetZ(enemyPosition);
	FLOAT oldPos = XMVectorGetZ(P);

	direction = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);



	FLOAT diffX;
	FLOAT diffY;
	FLOAT diffZ;

	diffX = 0;
	diffY = 0;
	diffZ = 0;
	




	if (travelToPoint == 1)
	{
		diffX = mEnemyPosition.x - mEnemyPositionTwo.x;
		diffY = mEnemyPosition.y - mEnemyPositionTwo.y;
		diffZ = mEnemyPosition.z - mEnemyPositionTwo.z;

		int nothing = 0;
		if (diffX < 0.0f)
		{

			diffX *= -1;
		}
		if (diffY < 0.0f)
		{

			diffY *= -1;
		}
		if (diffZ < 0.0f)
		{

			diffZ *= -1;
		}


		if (diffX < 1.0f && diffY < 1.f && diffZ < 1.0f)
		{

			travelToPoint = 2;
		}



		if (mEnemyPosition.x > mEnemyPositionTwo.x)
		{

			direction += XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);


		}
		else if (mEnemyPosition.x < mEnemyPositionTwo.x)
		{

			direction += XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		}



		if (mEnemyPosition.z > mEnemyPositionTwo.z)
		{

			direction += XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);


		}
		else if (mEnemyPosition.z < mEnemyPositionTwo.z)
		{

			direction += XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		}


		if (mEnemyPosition.y > mEnemyPositionTwo.y)
		{

			direction += XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);


		}
		else if (mEnemyPosition.y < mEnemyPositionTwo.y)
		{

			direction += XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		}

		

	}
	else if (travelToPoint == 2)
	{


		diffX = mEnemyPosition.x - mEnemyPositionOne.x;
		diffY = mEnemyPosition.y - mEnemyPositionOne.y;
		diffZ = mEnemyPosition.z - mEnemyPositionOne.z;

		if (diffX < 0.0f)
		{

			diffX *= -1;
		}
		if (diffY < 0.0f)
		{

			diffY *= -1;
		}
		if (diffZ < 0.0f)
		{

			diffZ *= -1;
		}



		if (mEnemyPosition.x > mEnemyPositionOne.x)
		{

			direction += XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);


		}
		if (mEnemyPosition.x < mEnemyPositionOne.x)
		{

			direction += XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		}



		if (mEnemyPosition.z > mEnemyPositionOne.z)
		{

			direction += XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);


		}
		if (mEnemyPosition.z < mEnemyPositionOne.z)
		{

			direction += XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		}


		if (mEnemyPosition.y > mEnemyPositionOne.y)
		{

			direction += XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);


		}
		if (mEnemyPosition.y < mEnemyPositionOne.y)
		{

			direction += XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		}
		if (diffX < 0.2f && diffY < 0.2f && diffZ < 0.2f)
		{

			travelToPoint = 1;
		}
	}

	



	XMMATRIX worldMatrix = XMLoadFloat4x4(&mEnemyWorld);
	XMVECTOR r = XMLoadFloat3(&mEnemyPosition);



	// Normalize our destinated direction vector
	direction = XMVector3Normalize(direction);




	///// the direction the player is going to move
	moveDirection = direction;
	/////character spinning make it more smooth
	if (XMVectorGetX(XMVector3Dot(direction, oldCharDirection)) == -1)
	{
		oldCharDirection += XMVectorSet(0.01f, 0.0f, 0.0f, 0.0f);
	}

	///////get characters position in world space
	charPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	charPosition = XMVector3TransformCoord(charPosition, worldMatrix);



	///// rotate the character
	float destDirLength = 10.0f * dt;



	currCharDirection = (oldCharDirection)+(direction * destDirLength);
	currCharDirection = XMVector3Normalize(currCharDirection);


	// get the angle 
	float charDirAngle = XMVectorGetX(XMVector3AngleBetweenNormals(XMVector3Normalize(currCharDirection), XMVector3Normalize(EnemyForward)));

	charDirAngle = -charDirAngle;

	if (XMVectorGetY(XMVector3Cross(currCharDirection, EnemyRight)) > 0.0f)
	{
		charDirAngle = -charDirAngle;
	}



	float speed = 15.0f * dt;
	

	
	direction = direction * speed;
	charPosition = charPosition + direction;


	


	XMMATRIX rotationMatrix;

	XMMATRIX previousWorld = worldMatrix;



	XMMATRIX Translation = XMMatrixTranslation(XMVectorGetX(charPosition), XMVectorGetY(charPosition), XMVectorGetZ(charPosition));
	rotationMatrix = XMMatrixRotationY(charDirAngle - 3.14159265f);		// Subtract PI from angle so the character doesn't run backwards



	worldMatrix = rotationMatrix * Translation;


	XMMatrixDecompose(&S, &Q, &P, worldMatrix);

	
	XMStoreFloat3(&mEnemyPosition, P);
	XMStoreFloat4(&mEnemyRotationQuad, Q);



	XMStoreFloat4x4(&mEnemyWorld, worldMatrix);







}
