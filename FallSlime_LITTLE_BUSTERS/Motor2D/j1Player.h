#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"


class j1Player : public j1Module
{
public:

	j1Player();
	~j1Player();

	void Init();

	// Called before render is available
	 bool Awake(pugi::xml_node&);

	// Called before the first frame
	 bool Start();

	// Called each loop iteration
	 bool PreUpdate();

	// Called each loop iteration
	  bool Update(float dt);

	// Called each loop iteration
	  bool PostUpdate();

	// Called before quitting
	  bool CleanUp();

	 //Save and Load
	  bool Load(pugi::xml_node&);

	  bool Save(pugi::xml_node&) const;

	// Called by collision module
	  bool OnCollision(Collider*, Collider*);

//private:

	//Transform
	iPoint position;
	uint width;
	uint height;

	//Collision
	bool onCollisionLeft;
	bool onCollisionRight;
	bool onCollisionUp;
	bool onCollisionDown;


	//Assets
	Animation idle;
	p2List_item<Collider*>  *collider = nullptr;
	
};


#endif // !__j1Player_H__

