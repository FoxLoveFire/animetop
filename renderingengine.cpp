#include <irrlicht.h>
#include <IrrlichtDevice.h>
#include "renderingengine.h"
#include <IGUIEnvironment.h>
#include <fstream>
#include <iostream>
#include "anime.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace irr;

#define OpenGl video::EDT_OPENGL

std::wstring utf8_to_wide(const std::string &input)
{
	const size_t inbuf_size = input.length();
	size_t outbuf_size = input.length() * sizeof(wchar_t);

	char *inbuf = new char[inbuf_size]; 
	memcpy(inbuf, input.c_str(), inbuf_size);
	std::wstring out;
	out.resize(outbuf_size / sizeof(wchar_t));


	char *outbuf = reinterpret_cast<char*>(&out[0]);
	delete[] inbuf;

	out.resize(outbuf_size / sizeof(wchar_t));
	return out;
}


Render* Render::_r = nullptr;

Render::Render()
{
	device = createDevice(OpenGl, core::dimension2d<u32>(wsize[0], wsize[1]), 32,
                                          false, false, false, 0); //создаем окно 
	driver = device->getVideoDriver(); // Получаем драйвер 

	_r = this;

	animes anime = read();
	showText(anime);
}

Render::~Render() 
{
	driver -> drop();
	driver = nullptr;
	device->closeDevice();
	device = nullptr;
	background -> drop();
	background = nullptr;
}

irr::IrrlichtDevice* Render::get_device() 
{
	return _r -> device;
}

irr::video::IVideoDriver* Render::get_driver()
{
	return _r -> driver;
}

irr::gui::IGUIEnvironment* Render::get_gui_env()
{
	return _r->device->getGUIEnvironment();
}

irr::scene::ISceneManager* Render::get_scene_manager()
{
	return _r->device->getSceneManager();
}

v2u32 Render::windowSize()
{
	return _r -> driver-> getScreenSize();
}

animes Render::read()
{
    animes anime;
    std::ifstream file("anime_top.txt", std::ios_base::app);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
        	Anime a(line, "info");
        	anime.push_back(a);
        }
    }
    return anime;
}

void Render::showText(animes anime)
{
   /* auto env = device->getGUIEnvironment();

    int y = 30; 
    gui::CGUITTFont* tt_font = gui::CGUITTFont::createTTFont(device->getGUIEnvironment(), "code2001.ttf", 14);

    for (auto a : anime) {
        if (tt_font)
            env->getSkin()->setFont(tt_font);
         	//font->setKerningWidth(24);

        env->addStaticText(core::stringw(a.getname(a).c_str()).c_str(), core::rect<s32>(10, y, 200, y + 20), false, true);
        y += 20; 
    }
    */
}

void Render::setBackground()
{
    background = driver->getTexture("textures/background.png");

    core::dimension2d<u32> screenSize = driver->getScreenSize();
    float scaleX = (float)screenSize.Width / background->getSize().Width;
    float scaleY = (float)screenSize.Height / background->getSize().Height;

    core::rect<s32> destRect(0, 0, screenSize.Width, screenSize.Height);

    driver->draw2DImage(background, destRect, core::rect<s32>(0, 0, background->getSize().Width, background->getSize().Height), nullptr, 0, true);

    driver->draw2DRectangle(video::SColor(150, 0, 0, 0), destRect, nullptr);
}

bool Render::setWindowIcon(std::string icon_file)
{
	video::IImageLoader *image_loader = NULL;
	u32 cnt = driver->getImageLoaderCount();
	for (u32 i = 0; i < cnt; i++) {
		if (driver->getImageLoader(i)->isALoadableFileExtension(
				    icon_file.c_str())) {
			image_loader = driver->getImageLoader(i);
			break;
		}
	}

	if (!image_loader) {
		std::cout << "Could not find image loader for file '" << icon_file
			      << "'" << std::endl;
		return false;
	}

	io::IReadFile *icon_f =
			device->getFileSystem()->createAndOpenFile(icon_file.c_str());

	if (!icon_f) {
		std::cout << "Could not load icon file '" << icon_file << "'"
			      << std::endl;
		return false;
	}

	video::IImage *img = image_loader->loadImage(icon_f);

	if (!img) {
		std::cout << "Could not load icon file '" << icon_file << "'"
			      << std::endl;
		icon_f->drop();
		return false;
	}

	u32 height = img->getDimension().Height;
	u32 width = img->getDimension().Width;

	size_t icon_buffer_len = 2 + height * width;
	long *icon_buffer = new long[icon_buffer_len];

	icon_buffer[0] = width;
	icon_buffer[1] = height;

	for (u32 x = 0; x < width; x++) {
		for (u32 y = 0; y < height; y++) {
			video::SColor col = img->getPixel(x, y);
			long pixel_val = 0;
			pixel_val |= (u8)col.getAlpha() << 24;
			pixel_val |= (u8)col.getRed() << 16;
			pixel_val |= (u8)col.getGreen() << 8;
			pixel_val |= (u8)col.getBlue();
			icon_buffer[2 + x + y * width] = pixel_val;
		}
	}

	img->drop();
	icon_f->drop();

	const video::SExposedVideoData &video_data = driver->getExposedVideoData();

	Display *x11_dpl = (Display *)video_data.OpenGLLinux.X11Display;

	Window x11_win = (Window)video_data.OpenGLLinux.X11Window;

	Atom net_wm_icon = XInternAtom(x11_dpl, "_NET_WM_ICON", False);
	Atom cardinal = XInternAtom(x11_dpl, "CARDINAL", False);
	XChangeProperty(x11_dpl, x11_win, net_wm_icon, cardinal, 32, PropModeReplace,
			(const unsigned char *)icon_buffer, icon_buffer_len);

	delete[] icon_buffer;
	return true;
}
