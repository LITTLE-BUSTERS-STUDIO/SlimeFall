#ifndef _Image_H__
#define _Image_H__

#include "p2Point.h"
#include "Animation.h"
#include "Object.h"

class Image: public Object
{

public:

	Image (iPoint position, Animation animation);

	virtual ~Image();

	bool Draw();
};

#endif // _Image_H__

