#pragma once

#include <irrlicht.h>
#include <IGUIEnvironment.h>
#include "types.h"

class Render
{
private:
	irr::IrrlichtDevice *device = nullptr;
	irr::video::IVideoDriver *driver;
	irr::video::ITexture *background;
	irr::gui::IGUIStaticText *anime_names = nullptr;
	static Render* _r;
protected:
	int wsize[2] = {1000, 600};
	v2u32 windowSize();
public:
	Render();
	~Render();
	irr::IrrlichtDevice *get_device();
	irr::video::IVideoDriver *get_driver();
	irr::gui::IGUIEnvironment *get_gui_env();
	irr::scene::ISceneManager *get_scene_manager();

	void setBackground();
	void showText(animes anime);
	bool setWindowIcon(std::string icon_file);

	animes read();
};