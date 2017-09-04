#include "AllegroExitSystem.h"
#include <string>
#include <sstream>
#include <allegro.h>
#include <winalleg.h>

void AllegroExitSystem(const char* fname, const char* ename)
{
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);

	std::ostringstream oss;
	oss << "Source: " << fname << "\n" << "Error: " << ename;
	allegro_message(oss.str().c_str());

	exit(0);
}