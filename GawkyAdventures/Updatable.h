#ifndef UPDATABLE_H
#define UPDATABLE_H

namespace Gawky { namespace Game{

class Updatable
{
public:
	virtual ~Updatable(){}
	virtual void Update(float elapsedTime) = 0;
};
	}}
#endif //UPDATABLE
