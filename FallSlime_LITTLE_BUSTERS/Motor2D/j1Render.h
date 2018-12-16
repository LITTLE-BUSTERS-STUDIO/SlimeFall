#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Timer.h"

#define MAX_ALPHA 255
#define MIN_ALPHA 0;

class j1Timer;

enum class FadeStates
{
	On,
	Middle,
	Out,
	Finish,
	None
};

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

	bool PostUpdate();

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

	bool SetCameraLimits(const int x, const int y);

	void CameraFollowPlayer(const bool value);

	// Fade to black methods ============================

	bool FadeToBlack(uint32 time);

public:

	bool            reset = false;
	bool            vsync;
	SDL_Renderer*	renderer = nullptr;
	SDL_Rect		camera = { 0, 0, 0, 0};
	SDL_Rect		viewport = {0, 0, 0, 0};
	SDL_Color		background;

private:
	// Fade to black ====================================
	bool           fade_active = false;
	int            fade_time = 0;
	j1Timer        fade_timer;
	FadeStates     fade_state = FadeStates::None;
	SDL_Rect       fade_rect;
	float          fade_alpha = 0;

	// Camera ===========================================
	int            camera_limit_x = 0;
	int            camera_limit_y = 0;
	bool           camera_follow_player = false;
	bool	       free_camera_x = true;
	bool	       free_camera_y = true;
	bool	       debug_border = false;
	int		       zoom = 0;
	int		       max_zoom = 0;
	int		       margin = 0;
	uint	       smooth_speed = 0;
	uint	       tremble = 0;
			       
	iPoint         smoth_position = { 0,0 };
	uint	       index_tremble = 0;
};

#endif // __j1RENDER_H__