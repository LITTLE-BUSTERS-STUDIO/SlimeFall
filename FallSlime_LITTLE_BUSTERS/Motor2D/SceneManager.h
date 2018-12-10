#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "j1Module.h"
#include "j1Scene.h"

struct SDL_Texture;

class SceneManager : public j1Module
{
public:

	SceneManager();

	virtual ~SceneManager();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

private:


};

#endif // __SCENE_MANAGER_H__

