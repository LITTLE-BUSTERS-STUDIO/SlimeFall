#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"

#define MAX_FRAMES 25

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		loops = 0;
		current_frame = 0.0f;
	}

	bool LoadAnimation(pugi::xml_node anim_node , p2SString name)
	{
		pugi::xml_node node;

		if (anim_node == NULL)
		{
			return false;
		}
		
		for (node = anim_node.child("tile"); node; node = node.next_sibling("tile"))
		{
			if (name == node.child("properties").child("property").attribute("name").as_string(""))
			{
				break;
			}
		}

		if (node == NULL)
		{
			return false;
		}

		uint tile_width = anim_node.attribute("tilewidth").as_uint(1);
		uint tile_height = anim_node.attribute("tileheight").as_uint(1);
		uint num_tiles_width = anim_node.attribute("columns").as_uint(1);
		this->loop = anim_node.child("properties").child("property").attribute("value").as_bool(false);

		for (pugi::xml_node frames = node.child("animation").child("frame"); frames; frames = frames.next_sibling("frame"))
		{
			uint  id = frames.attribute("tileid").as_uint(0);
			SDL_Rect rect;
			rect.w = tile_width;
			rect.h = tile_height;
			rect.x = rect.w * (id % num_tiles_width);
			rect.y = rect.h * (id / num_tiles_width);
			this->PushBack(rect);
		}

		return true;
	}
};

#endif
