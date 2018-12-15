#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "j1Module.h"
#include "p2SString.h"
#include "j1Render.h"

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

	bool ResetScene(); // All scene ========================

	bool ChangeScene(const p2SString name, int phase);

	void Exit();

	// Load & Save game ====================================
	bool Load(pugi::xml_node& node);

	bool Save(pugi::xml_node& node) const;

	// Load & Unload phases and its map  ====================

	bool LoadPhase(uint phase_number);

	bool NextPhase();
	 
private:
	// Load & Save scene ==================================
	bool LoadScene(const p2SString name, int phase);

	bool UnloadScene();

private:

	j1Scene*                 current_scene = nullptr;
	uint                     current_phase = 0u;
	p2SString                default_scene_str;
	bool                     default_scene_loaded = false;
	bool                     exit = false;
	// Scene to load ========================
	p2SString                scene_to_load;
	uint                     phase_to_load;

	// Document scenes.xml ==================

	p2SString                scene_doc_path;
private:
	friend j1Render;
};

#endif // __SCENE_MANAGER_H__

