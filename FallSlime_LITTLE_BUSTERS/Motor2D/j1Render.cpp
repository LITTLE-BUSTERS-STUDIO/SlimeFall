#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Player.h"

#include "j1Input.h"

#define VSYNC true

j1Render::j1Render() : j1Module()
{
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	SetViewPort(camera);
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_RenderClear(renderer);

	//Tests var 
	int speed = 1;
	int level_width = 2000;
	int level_high = 1000;

	////Camera_x hit screen---------------------------------------
	//if (free_camera_x)
	//{
	//	if (camera.x <= 0)
	//	{
	//		camera.x = 0;

	//		free_camera_x = false;
	//		LOG(" HIT LEFT");
	//	}
	//	else if (camera.x + camera.w > level_width)
	//	{
	//		camera.x = level_width - camera.w;
	//		free_camera_x = false;
	//		LOG(" HIT RIGHT");
	//	}
	//	
	//}

	//else if (App->player->position.x > camera.w / 2 && App->player->position.x < level_width - camera.w / 2)
	//{
	//	free_camera_x = true;
	//}

	//Camera_x Follow Player
	if (free_camera_x)
	{
		camera.x = App->player->position.x - camera.w / 2;
		LOG("_________%d", camera.x);
	}


	//Camera_y hit screen---------------------------------------
	if (free_camera_y)
	{
		if (camera.y <= 0)
		{
			camera.y = 0;
			free_camera_y = false;
			LOG(" HIT UP");
		}
		else if (camera.y + camera.h > level_high)
		{
			camera.y = level_high - camera.h ;
			free_camera_y = false;
			LOG(" HIT Down");
		}
	}

	else if (App->player->position.y > camera.h / 2 && App->player->position.y < level_high - camera.h/2)
	{
		free_camera_y = true;
	}

	

	//Camera_y Follow Player
	if (free_camera_y)
	{
		camera.y = App->player->position.y - camera.h / 2;
	}

	LOG("_________%d", free_camera_y);




	//ZOOM
	if (App->input->keyboard[SDL_SCANCODE_F10] == KEY_DOWN)
	{
		if (zoomedOutSize < max_zoom)
		{
			zoomedOutSize++;
			SDL_RenderSetLogicalSize(renderer, camera.w * zoomedOutSize, camera.h * zoomedOutSize);
		}
	}
	else if (App->input->keyboard[SDL_SCANCODE_F11] == KEY_DOWN)
	{
		if (zoomedOutSize > 1)
		{
			zoomedOutSize--;
			SDL_RenderSetLogicalSize(renderer, camera.w  * zoomedOutSize, camera.h * zoomedOutSize);
		}
	}

	return true;
}

bool j1Render::Update(float dt)
{

	return true;
}

bool j1Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}


void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen

bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, bool flip_x, float speed, double angle, int pivot_x, int pivot_y ) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(-camera.x * speed) + x * scale;
	rect.y = (int)(-camera.y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	// SDL Render Function Flip
	SDL_RendererFlip flip = SDL_FLIP_NONE; 
	if (flip_x)
		flip = SDL_FLIP_HORIZONTAL;

	//if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	//{
	//	pivot.x = pivot_x;
	//	pivot.y = pivot_y;
	//	p = &pivot;
	//}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(-camera.x + rect.x * scale);
		rec.y = (int)(-camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, -camera.x + x1 * scale, -camera.y + y1 * scale, -camera.x + x2 * scale, -camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}
