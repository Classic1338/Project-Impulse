#include "misc.h"
#include "../menu/features.hpp"
#include "../../dependencies/common_includes.hpp"
#include <string>
#include <cstring>
#include <sstream>

color yes(255, 255, 255, 255);

void c_misc::watermark() noexcept {
	if (!Features.Watermark)
		return;


	int width, height;
	interfaces::engine->get_screen_size(width, height);

	//rainbow sync
	static unsigned int last_time;
	last_time = GetTickCount();
	color rainbow;
	rainbow.FromHSV(fmod(last_time * 0.0002f, 1.f), 1.f, 0.5f);

//	std::stringstream print;
	std::string print = "Project Impulse";

	if (Features.Rainbow) {
		render::draw_filled_rect(width - 275, 4, 260, 20, color(33, 35, 47, 200));
		render::draw_filled_rect(width - 275, 4, 260, 3, color(rainbow));
		render::text(width - 270, 7, color(rainbow), render::main_font, false, print.c_str()); //90, 31, 166, 255
	}
	else {
		render::draw_filled_rect(width - 275, 4, 260, 20, color(33, 35, 47, 200));
		render::draw_filled_rect(width - 275, 4, 260, 3, color(90, 31, 166, 200));
		render::text(width - 270, 7, color(90, 31, 166, 200), render::main_font, false, print.c_str());
	}
}

void c_misc::clantag_spammer() noexcept {
	if (!Features.ClanTag)
		return;

	static std::string tag = "Project Impulse ";
	static std::string tag2 = " ";
	static float last_time = 0;

	if (interfaces::globals->cur_time > last_time) {
		std::rotate(std::begin(tag), std::next(std::begin(tag)), std::end(tag));
		utilities::apply_clan_tag(tag.c_str());

		last_time = interfaces::globals->cur_time + 0.9f;
	}

	else {
		std::rotate(std::begin(tag), std::next(std::begin(tag)), std::end(tag));
		utilities::apply_clan_tag(tag2.c_str());

		last_time = interfaces::globals->cur_time + 0.9f;
	}
	 
	if (fabs(last_time - interfaces::globals->cur_time) > 1.f)
		last_time = interfaces::globals->cur_time;
}