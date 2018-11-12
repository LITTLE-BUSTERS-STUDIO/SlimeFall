#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Player.h"
#include "Level_1.h"
#include "j1Input.h"

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

	zoom = config.child("debug").attribute("zoom").as_uint(1u);
	max_zoom = config.child("debug").attribute("max_zoom").as_uint(1u);

	phase1_width = config.child("level1_1").attribute("width").as_int(0);
	phase1_high = config.child("level1_1").attribute("high").as_int(0);
	phase2_width = config.child("level1_2").attribute("width").as_int(0);
	phase2_high = config.child("level1_2").attribute("high").as_int(0);

	margin = config.child("debug_border_margin").attribute("value").as_int(0);

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SetViewPort(camera);
	SDL_RenderGetViewport(renderer, &viewport);

	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_RenderClear(renderer);

	//ZOOM
	if (App->input->keyboard[SDL_SCANCODE_F3] == KEY_DOWN)
	{
		debug_border = true;

		if (zoom < max_zoom)
		{
			zoom++;
			SDL_RenderSetLogicalSize(renderer, camera.w * zoom, camera.h * zoom);
		}
	}
	else if (App->input->keyboard[SDL_SCANCODE_F4] == KEY_DOWN)
	{
		if (zoom > 1)
		{
			zoom--;
			SDL_RenderSetLogicalSize(renderer, camera.w  * zoom, camera.h * zoom);
		}
		if (zoom <= 1)
		{
			debug_border = false;
		}
	}

	//Debug Middle pointer
	if (App->input->keyboard[SDL_SCANCODE_F9] == KEY_DOWN) {
		if(!debug_middle)
			debug_middle = true;
		else
			debug_middle = false;
	}

	return true;
}

bool j1Render::Update(float dt)
{	
	fPoint player_position(App->player->position.x, App->player->position.y);

	//Camera_x hit screen---------------------------------------
	if (free_camera_x)
	{
	    if (camera.x <= 0)
		{
			camera.x = 0;

			free_camera_x = false;
		}
		else if (camera.x + camera.w > phase1_width)
		{
			camera.x = phase1_width - camera.w;
			free_camera_x = false;
		}
	}
	else if (App->win->GetScale() * player_position.x > camera.w / 2 && App->win->GetScale() *player_position.x < phase1_width - camera.w / 2)
		free_camera_x = true;

	//Camera_x Follow Player
	if (free_camera_x)
		camera.x = App->win->GetScale() * player_position.x - camera.w / 2;

	//Camera_y hit screen---------------------------------------
	if (free_camera_y)
	{
		if (camera.y < 0) 
		{
			camera.y = 0;
			free_camera_y = false;
		}

		else if (camera.y + camera.h > phase1_high)
		{
			camera.y = phase1_high - camera.h;
			free_camera_y = false;
		}
	}
	else if (App->win->GetScale() * player_position.y > camera.h / 2 && App->win->GetScale() * player_position.y < phase1_high - camera.h / 2)
		free_camera_y = true;

	//Camera_y Follow Player
	if (free_camera_y)
		camera.y = App->win->GetScale() * player_position.y - camera.h / 2;

	if (reset)
	{
		CameraReset();
		reset = false;
	}

	return true;
}

bool j1Render::PostUpdate()
{
	//Quad borders DEBUG
	int borderWidth = margin * zoom;
	int scale = App->win->GetScale();

	if (debug_border)
	{
		// Up border
		App->render->DrawQuad({ camera.x / scale - borderWidth - margin , camera.y / scale - borderWidth - margin, camera.w / scale + borderWidth * 2 + margin * 2, borderWidth }, 255, 255, 255, 255);
		// Down border
		App->render->DrawQuad({ camera.x / scale - borderWidth - margin , (camera.y + camera.h) / scale + margin, camera.w / scale + borderWidth * 2 + margin * 2, borderWidth }, 255, 255, 255, 255);
		// Left border
		App->render->DrawQuad({ camera.x / scale - borderWidth - margin, camera.y / scale - margin , borderWidth, camera.h / scale + margin * 2 }, 255, 255, 255, 255);
		// Right border
		App->render->DrawQuad({ (camera.x + camera.w) / scale + margin, camera.y / scale - margin , borderWidth , camera.h / scale + margin * 2 }, 255, 255, 255, 255);
	}
	
	if (debug_middle)
	{
		// Centered point DEBUG
		App->render->DrawCircle(camera.x + camera.w / 2, camera.y + camera.h / 2, 1, 50, 255, 50, 255);
	}

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
	camera.x = data.child("camera").attribute("x").as_int(0);
	camera.y = data.child("camera").attribute("y").as_int(0);
	zoom = data.child("debug").attribute("zoom").as_int(0);
	free_camera_x = data.child("conditions").attribute("free_camera_x").as_bool(false);
	free_camera_y = data.child("conditions").attribute("free_camera_y").as_bool(false);

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	pugi::xml_node debug = data.append_child("debug");

	debug.append_attribute("zoom") = zoom;

	pugi::xml_node conditions = data.append_child("conditions");

	conditions.append_attribute("free_camera_x") = free_camera_x;
	conditions.append_attribute("free_camera_y") = free_camera_y;

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

iPoint j1Render::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, bool flip_x, float speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)((camera.w * (zoom - 1)) / 2) + (int)(-camera.x * speed) + x * scale;
	rect.y = (int)((camera.h * (zoom - 1)) / 2) + (int)(-camera.y * speed) + y * scale;


	if (section != NULL)
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
		rec.x = (int)((camera.w * (zoom - 1)) / 2) + (-camera.x + rect.x * scale);
		rec.y = (int)((camera.h * (zoom - 1)) / 2) + (-camera.y + rect.y * scale);
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
		result = SDL_RenderDrawLine(renderer, ((camera.w * (zoom - 1)) / 2) -camera.x + x1 * scale, ((camera.h * (zoom - 1)) / 2) -camera.y + y1 * scale, -camera.x + x2 * scale, -camera.y + y2 * scale);
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
		points[i].x = (int)(x -camera.x* use_camera + radius * cos(i * factor));
		points[i].y = (int)(y - camera.y* use_camera + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::CameraReset() {
	camera.x = 0;
	camera.y = 0;
	free_camera_x = false;
	free_camera_y = false;
    return true;
}