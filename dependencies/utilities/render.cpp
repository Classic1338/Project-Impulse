#include "render.hpp"

unsigned long render::main_font;
unsigned long render::second_font;
unsigned long render::smallestpxl;

void render::setup_fonts() {
	main_font = interfaces::surface->font_create();
	second_font = interfaces::surface->font_create();
	smallestpxl = interfaces::surface->font_create();

	interfaces::surface->set_font_glyph(main_font, "Tahoma", 12, 500, 0, 0, font_flags::fontflag_outline);
	interfaces::surface->set_font_glyph(second_font, "Tahoma", 12, 500, 0, 0, font_flags::fontflag_underline);
	interfaces::surface->set_font_glyph(smallestpxl, "small_pixel", 12, 500, 0, 0, font_flags::fontflag_antialias);
}

void render::line(int x, int y, int x2, int y2, color c) noexcept {
	interfaces::surface->set_drawing_color(c.r, c.g, c.b, c.a);
	interfaces::surface->draw_line(x, y, x2, y2);
}

void render::draw_outline(int x, int y, int w, int h, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_outlined_rect(x, y, w, h);
}

void render::rect(int x, int y, int x2, int y2, color c) noexcept {
	interfaces::surface->set_drawing_color(c.r, c.g, c.b, c.a);
	interfaces::surface->draw_outlined_rect(x, y, x2, y2);
}

void render::draw_filled_rect(int x, int y, int w, int h, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_filled_rectangle(x, y, w, h);
}

void render::filled_rect(int x, int y, int x2, int y2, color c) noexcept {
	interfaces::surface->set_drawing_color(c.r, c.g, c.b, c.a);
	interfaces::surface->draw_filled_rectangle(x, y, x2, y2);
}
void render::text(int x, int y, unsigned long font, const wchar_t* string, color colour) {
	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	interfaces::surface->draw_text_pos(x, y);
	interfaces::surface->draw_render_text(string, wcslen(string));
}

void render::draw_text(int x, int y, unsigned long font, const wchar_t* string, color colour) {
	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	interfaces::surface->draw_text_pos(x, y);
	interfaces::surface->draw_render_text(string, wcslen(string));
}
void render::draw_text(int x, int y, unsigned long font, std::string string, bool text_centered, color colour) {
	const auto converted_text = std::wstring(string.begin(), string.end());

	int width, height;
	interfaces::surface->get_text_size(font, converted_text.c_str(), width, height);

	interfaces::surface->set_text_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_text_font(font);
	if (text_centered)
		interfaces::surface->draw_text_pos(x - (width / 2), y);
	else
		interfaces::surface->draw_text_pos(x, y);
	interfaces::surface->draw_render_text(converted_text.c_str(), wcslen(converted_text.c_str()));
}
void render::text(int x, int y, color color, int font, bool center, std::string str) noexcept {
	std::wstring text = std::wstring(str.begin(), str.end());
	const wchar_t* converted_text = text.c_str();

	int width, height;
	interfaces::surface->get_text_size(font, converted_text, width, height);

	interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_text_font(font);
	if (center)
		interfaces::surface->draw_text_pos(x - (width / 2), y);
	else
		interfaces::surface->draw_text_pos(x, y);

	interfaces::surface->draw_render_text(converted_text, wcslen(converted_text));
}

void render::get_text_size(unsigned long font, std::string str, int& width, int& height) noexcept {
	std::wstring text = std::wstring(str.begin(), str.end());
	const wchar_t* out = text.c_str();

	interfaces::surface->get_text_size(font, out, width, height);
}

float render::get_frametime() noexcept {
	return interfaces::globals->frame_time;
}