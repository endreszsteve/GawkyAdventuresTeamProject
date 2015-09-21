#ifndef UPDATEABLE_H
#define UPDATEABLE_H

namespace Gawky { namespace Game {

	class Updateable
	{
	public:
		virtual ~Updateable(){}
		virtual void Update(float elapsedTime) = 0;
	};
	}}
#endif // UPDATEABLE_H