#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "j1Player.h"





j1Player::j1Player() 
{
	name.create("player");
	position.x = position.y = 200;
}

j1Player::~j1Player()
{

}

void j1Player::Init()
{
	
}

// Called before render is available
bool  j1Player::Awake(pugi::xml_node& node )
{
	bool ret = true;
	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	bool ret = true;
	collider = App->collision->AddCollider({ 0,0,20,20 }, COLLIDER_PLAYER, this);
	return ret;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		position.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		position.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		position.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.x += 1;
	
	if (collider != NULL)
		collider->data->SetPos(position.x - width / 2, position.y - high / 2);

	return ret;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	bool ret = true;

	
	return ret;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool j1Player::CleanUp()
{
	bool ret = true;
	return ret;
}

//Save and Load
bool j1Player::Load(pugi::xml_node& node)
{
	bool ret = true;
	return ret;
}

bool j1Player::Save(pugi::xml_node& node) const
{
	bool ret = true;
	return ret;
}

// Called by collision module
bool j1Player::OnCollision(Collider* c1, Collider* c2)
{
	bool ret = true;
	LOG("Collision!!!!");

	if (c2->rect.y + c2->rect.h > c1->rect.y)
	{
		position.y += 1;
	}


	
	return ret;
}
