#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Module.h"
#include "p2List.h"

class j1Player : public j1Module
{
public:

	j1Player();
	~j1Player();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

private:
	p2List_item<Collider*>  *collider;


};


#endif // !__j1Player_H__

