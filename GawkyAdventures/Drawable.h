#ifndef DRAWABLE_H
#define DRAWBALE_H

namespace Gawky { namespace Game{

	class Drawable
	{
	public:
		virtual ~Drawable(){}
		virtual void Draw(float elapsedFrameTime) = 0;
	};
	}}
#endif //DRAWABLE_H