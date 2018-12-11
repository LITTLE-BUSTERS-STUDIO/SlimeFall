#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "j1Module.h"
#include "p2SString.h"

class j1Scene;
class Level_1;

struct SDL_Texture;

class SceneManager : public j1Module
{
public:

	SceneManager();

	virtual ~SceneManager();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

	j1Scene* GetCurrentScene();

	// Load & Save scene ==================================

	bool Load(pugi::xml_node& node);

	bool Save(pugi::xml_node& node) const;

	bool LoadScene(p2SString name);

	bool UnloadScene();

	// Load/ Unload phases and its map  ====================

	bool LoadPhase(uint phase_number, bool spawn = true);

	bool NextPhase();


private:

	j1Scene*     current_scene = nullptr;
	uint         current_phase = 0;
	bool         default_phase_loaded = false;
	Level_1* level;
};

#endif // __SCENE_MANAGER_H__

