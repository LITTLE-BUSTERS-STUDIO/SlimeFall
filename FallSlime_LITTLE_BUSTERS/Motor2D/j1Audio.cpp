#include "p2Defs.h"
#include "p2Log.h"
#include "j1Audio.h"
#include "p2List.h"
#include "j1App.h"
#include "j1Input.h"
#include "Brofiler/Brofiler.h"


#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

j1Audio::j1Audio() : j1Module()
{
	music = NULL;
	name.create("audio");
}

// Destructor
j1Audio::~j1Audio()
{}

// Called before render is available
bool j1Audio::Awake(pugi::xml_node& config)
{
	BROFILER_CATEGORY("Audio Awake", Profiler::Color::CornflowerBlue);

	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	fade_time = config.child("fade_time").attribute("default").as_uint(0u);
	volume_music = config.child("volume_music").attribute("default").as_uint(0u);
	volume_sfx = config.child("volume_sfx").attribute("default").as_uint(0u);
	max_volume = config.child("max_volume").attribute("value").as_uint(0u);
	mute = config.child("mute").attribute("value").as_bool(false);

	if (mute)
	{
		Mix_VolumeMusic(0);
	}
	else
	{
		Mix_VolumeMusic(volume_music);
		
	}

	return ret;
}

bool j1Audio::PreUpdate()
{
	BROFILER_CATEGORY("Audio PreUpdate", Profiler::Color::BurlyWood);

	if (App->input->keyboard[SDL_SCANCODE_F7] == KEY_DOWN && mute == false) //Mute
	{
		mute = true;
		Mix_VolumeMusic(0);
		LOG("Mute ON!");
	}
	else if (App->input->keyboard[SDL_SCANCODE_F7] == KEY_DOWN && mute == true)
	{
		mute = false;
		Mix_VolumeMusic(App->audio->volume_music);
		LOG("Mute OFF!");
	}

	if (!mute)
	{
		if (App->input->keyboard[SDL_SCANCODE_KP_MINUS] == KEY_REPEAT) //Set DOWN Volume Music
		{
			if (App->audio->volume_music > 0)
				App->audio->volume_music--;
			Mix_VolumeMusic(App->audio->volume_music);
			setdown_volume_fx = true;
			LOG("Volume = %d", Mix_VolumeMusic(volume_music));
		}
		if (App->input->keyboard[SDL_SCANCODE_KP_PLUS] == KEY_REPEAT) //Set UP Volume Music
		{
			if (App->audio->volume_music < max_volume)
				App->audio->volume_music++;
			setup_volume_fx = true;
			Mix_VolumeMusic(App->audio->volume_music);
			LOG("Volume = %d", Mix_VolumeMusic(volume_music));

		}
	}
	return true;
}

// Called before quitting
bool j1Audio::CleanUp()
{
	BROFILER_CATEGORY("Audio CleanUp", Profiler::Color::Chartreuse);

	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool j1Audio::PlayMusic(const char* path)
{
	bool ret = true;
	BROFILER_CATEGORY("PlayMusic", Profiler::Color::Brown);

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 0/*1000.0f*/));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}
	

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
unsigned int j1Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool j1Audio::PlayFx(unsigned int id, int repeat)
{
	bool ret = false;
	BROFILER_CATEGORY("PlayFx", Profiler::Color::BlueViolet);

	if(!active)
		return false;

	if(id > 0 && id <= fx.count())
	{
		if (mute)
		{
			Mix_VolumeChunk(fx[id - 1], 0);
			Mix_PlayChannel(-1, fx[id - 1], repeat, 0);
		}
			
		else
			Mix_VolumeChunk(fx[id - 1], volume_sfx);
			Mix_PlayChannel(-1, fx[id - 1], repeat, 0);
	}

	if (setdown_volume_fx) //Set DOWN Volume Music
	{
		if (App->audio->volume_sfx > 0)
			App->audio->volume_sfx = volume_music;
		Mix_VolumeChunk(fx[id - 1], volume_sfx);
		Mix_PlayChannel(-1, fx[id - 1], repeat, 0);
	setdown_volume_fx = false;
	}

	if (setup_volume_fx) //Set UP Volume Music
	{
		if (App->audio->volume_sfx < max_volume)
			App->audio->volume_sfx = volume_music;
		Mix_VolumeChunk(fx[id - 1], volume_sfx);
		Mix_PlayChannel(-1, fx[id - 1], repeat, 0);
		setup_volume_fx = false;
	}

	return ret;
}

bool j1Audio::Load(pugi::xml_node& node)
{
	mute = node.child("volumes").attribute("mute").as_bool(false);
	volume_music = node.child("volumes").attribute("music").as_uint(0u);
	volume_sfx = node.child("volumes").attribute("sfx").as_uint(0u);
	return true;
}
bool j1Audio::Save(pugi::xml_node& node ) const
{
	pugi::xml_node volumes = node.append_child("volumes");

	volumes.append_attribute("music") = volume_music;
	volumes.append_attribute("sfx") = volume_sfx;
	volumes.append_attribute("mute") = mute;

	return true;
}






