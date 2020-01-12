#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Module.h"
#include "Dummy.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include"ModuleVerlet.h"
#include"ModuleUI.h"
#include "ModuleFonts.h"
#include "ModuleDebug.h"
#include "GravitationalField.h"

class ModuleDebug;
class ModuleVerlet;
class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleVerlet* verlet;
	ModuleUI* UI;
	ModuleFonts* fonts;
	ModuleDebug* debug;
	GravitationalField* gravitationalField;

private:

	p2List<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
};