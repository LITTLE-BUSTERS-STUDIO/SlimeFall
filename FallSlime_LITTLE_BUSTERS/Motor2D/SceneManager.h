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

	bool PostUpdate();

	bool CleanUp();

	j1Scene* GetCurrentScene();

	// Load & Save game ====================================
	bool Load(pugi::xml_node& node);

	bool Save(pugi::xml_node& node) const;

	// Load & Save scene ==================================
	bool LoadScene(p2SString name);

	bool UnloadScene();

	// Load & Unload phases and its map  ====================

	bool LoadPhase(uint phase_number, bool spawn = true);

	bool NextPhase();

private:

	j1Scene*                 current_scene = nullptr;
	uint                     current_phase = 0u;
	p2SString                default_scene_str;
	bool                     default_scene_loaded = false;

	// Document scenes.xml ==================
	pugi::xml_document       scenes_doc;

};

#endif // __SCENE_MANAGER_H__

