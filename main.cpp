#include <iostream>
#include <irrlicht.h>
#include "types.h"
#include <sstream>
#include <string>
#include "renderingengine.h"


int main()
{
	Render* render = new Render();
	irr::video::IVideoDriver* driver = render->get_driver();
	irr::IrrlichtDevice* device = render->get_device();

	render->setWindowIcon("textures/icon.png");

	irr::core::stringw str = L"Anime Top 50 [";
	str += driver->getName();
	str +=  L"]";

	device->setWindowCaption(str.c_str());

	while (device->run())
	{
		driver->beginScene(true, true, irr::video::SColor(255, 0, 0, 0));
		render->setBackground();

		driver->endScene();
	}
	
	return 0;
}