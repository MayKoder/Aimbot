#ifndef __ModuleDebug_H__
#define __ModuleDebug_H__

#include "SDL/include/SDL_pixels.h"
#include "Globals.h"
#include"Application.h"

class Application;
class ModuleDebug :public Module {

public:

	ModuleDebug(Application* app, bool start_enabled = true);
	virtual ~ModuleDebug();
	bool Awake();
	bool Start();
	update_status Update();

	int debugPointNumber = -1;
};



#endif