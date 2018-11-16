#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	bool CameraReset();
	iPoint ScreenToWorld(int x, int y) const;

	// Blit
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, bool flip_x = false, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	// Camera shake
	bool j1Render::CameraTremble();

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Rect		viewport;
	SDL_Color		background;

public:
	bool    reset = false;
	bool	free_camera_x = true;
	bool	free_camera_y = true;
	bool	debug_border = false;
	bool	debug_middle = false;
	bool	draw_pathfinding = true;
	bool    vsync;
	//XML vars
	int		zoom ;
	int		max_zoom;
	int		phase1_width;
	int		phase1_high;
	int		phase2_width;
	int		phase2_high;
	int		margin;

	uint	smooth_speed;
	uint	tremble;

private:
	iPoint		smoth_position = { 0,0 };
	iPoint		camera_flip = { 0,0 };
	uint		index_tremble = 0;
};

#endif // __j1RENDER_H__