#ifndef __j1Player_H__
#define __j1Player_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Animation.h"

enum class State
{
	idle
};

enum class Direction: uint
{
	right,
	left,
	up,
	down,
	max
};

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

private:

	//Transform
	fPoint position;
	SDL_Rect collider_rect;
	State current_state;
	

	//Physics
	fPoint velocity;
	fPoint acceleration;
	float gravity;
	float speed_air;
	float speed_ground;
	float speed_jump;

	//Collision
	bool on_ground;
	
	//Assets
	p2SString idle_path;
	Animation idle;
	Collider  *collider = nullptr;
	
};


#endif // !__j1Player_H__

