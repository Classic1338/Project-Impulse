#include "esp.h"
#include "../menu/features.hpp"
#include "../../source-sdk/math/math.hpp"

features Features;
c_math math;

void Esp::loop()
{
	auto local_player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!local_player)
		return;



	//player drawing loop
	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		auto entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			continue;

		if (entity == local_player)
			continue;

		if (entity == nullptr)
			return;

		if (entity->team() == local_player->team())
			continue;

		if (entity->health() <= 0)
			continue;

		draw(entity);
		dropped_weapons(entity);
	}
}

void Esp::draw(player_t * entity)
{
	box bbox;
	if (!get_playerbox(entity, bbox))
		return;

	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);

	//rainbow sync
	static unsigned int last_time;
	last_time = GetTickCount();
	color rainbow;
	rainbow.FromHSV(fmod(last_time * 0.0002f, 1.f), 1.f, 0.5f);

	if (Features.Box)
	{
		auto red = 0 * 255;
		auto green = 0 * 255;
		auto blue = 0 * 255;
		auto weapon = entity->active_weapon();

		if (Features.Rainbow) {
			render::draw_filled_rect(bbox.x, bbox.y, bbox.w + 13, bbox.h + 2, color(rainbow));
		}
		else {
			render::draw_filled_rect(bbox.x, bbox.y, bbox.w + 13, bbox.h + 2, color(33, 35, 47, 155));
		}

		//render::draw_outline(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color(0, 0, 0, 255 + alpha[entity->index()]));
		//render::rect(bbox.x, bbox.y, bbox.w, bbox.h, color(90, 31, 166, 255 + alpha[entity->index()]));
		//render::draw_outline(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color(0, 0, 0, 255 + alpha[entity->index()]));
		
	}

	if (Features.Weapon) {
		auto red = 0 * 255;
		auto green = 0 * 255;
		auto blue = 0 * 255;
		auto weapon = entity->active_weapon();

		if (!weapon)
			return;

		std::string weapons;
		weapons = clean_item_name(weapon->get_weapon_data()->weapon_name);

		if (Features.Rainbow) {
			render::draw_filled_rect(bbox.x - 40 + (bbox.w / 2), bbox.h + bbox.y + 2, 75, 15, color(33, 35, 47, 255));
			render::draw_filled_rect(bbox.x - 40 + (bbox.w / 2), bbox.h + bbox.y + 2, 75, 3, color(rainbow));
			render::draw_text(bbox.x + (bbox.w / 2), bbox.h + bbox.y + 2, render::main_font, weapons, true, color(255, 255, 255, 255 + alpha[entity->index()]));
		}
		else {
			render::draw_filled_rect(bbox.x - 40 + (bbox.w / 2), bbox.h + bbox.y + 2, 75, 15, color(33, 35, 47, 255));
			render::draw_filled_rect(bbox.x - 40 + (bbox.w / 2), bbox.h + bbox.y + 2, 75, 3, color(90, 31, 166, 255));
			render::draw_text(bbox.x + (bbox.w / 2), bbox.h + bbox.y + 2, render::main_font, weapons, true, color(255, 255, 255, 255 + alpha[entity->index()]));
		}
	}	

	if (Features.Name)
	{
		std::string print(info.fakeplayer ? std::string("bot ").append(info.name).c_str() : info.name);
		std::transform(print.begin(), print.end(), print.begin(), ::tolower);

		if (Features.Rainbow) {
			render::draw_filled_rect(bbox.x - 40 + (bbox.w / 2), bbox.y - 13, 75, 15, color(33, 35, 47, 155));
			render::draw_filled_rect(bbox.x - 40 + (bbox.w / 2), bbox.y - 1, 75, 3, color(rainbow));
			render::draw_text(bbox.x + (bbox.w / 2), bbox.y - 14, render::main_font, print, true, color(255, 255, 255, 155 + alpha[entity->index()]));
		}

		else {
			render::draw_filled_rect(bbox.x - 40 + (bbox.w / 2), bbox.y - 13, 75, 15, color(33, 35, 47, 155));
			render::draw_filled_rect(bbox.x - 40 + (bbox.w / 2), bbox.y - 1, 75, 3, color(90, 31, 166, 155));
			render::draw_text(bbox.x + (bbox.w / 2), bbox.y - 14, render::main_font, print, true, color(255, 255, 255, 155 + alpha[entity->index()]));
		}
	}

	if (Features.Health) {
		box temp(bbox.x - 5, bbox.y - 5 + (bbox.h - bbox.h * (utilities::math::clamp_value<int>(entity->health(), 0, 100.f) / 100.f)), 1, bbox.h * (utilities::math::clamp_value<int>(entity->health(), 0, 100) / 100.f) - (entity->health() >= 100 ? 0 : -1));
		box temp_bg(bbox.x - 5, bbox.y, 1, bbox.h);

		auto health_color = color((255 - entity->health() * 2.55), (entity->health() * 2.55), 0, alpha[entity->index()]);

		if (entity->health() > 99)
			health_color = color(0, 255, 0, 200);

		else if (entity->health() > 65)
			health_color = color(232, 145, 23, 200);

		else if (entity->health() > 35)
			health_color = color(232, 63, 16, 200);

		else if (entity->health() > 5)
			health_color = color(255, 0, 0, 200);

		//render::draw_filled_rect(temp_bg.x - 1, temp_bg.y - 1, temp_bg.w + 2, temp_bg.h + 2, color(0, 0, 0, 255 + alpha[entity->index()]));
		//render::draw_filled_rect(temp.x, temp.y, temp.w, temp.h, color(health_color));

		render::draw_filled_rect(bbox.x, bbox.y, bbox.w + 13, bbox.h + 2, color(33, 35, 47, 155));
	//	render::draw_filled_rect(temp.x, temp.y, 3, temp.h, color(health_color));
	}
}

void Esp::dropped_weapons(player_t* entity) noexcept {
	//auto class_id = entity->clientclass()->class_id;
	//auto model_name = interfaces::model_info->get_model_name(entity->model());
	//auto weapon = entity;

	//if (!entity)
	//	return;

	//if (!weapon)
	//	return;

	//vec3_t dropped_weapon_position, dropped_weapon_origin;

	//dropped_weapon_origin = weapon->origin();

	//if (!math.world_to_screen(dropped_weapon_origin, dropped_weapon_position))
	//	return;

	//if (!(entity->origin().x == 0 && entity->origin().y == 0 && entity->origin().z == 0)) { //ghetto fix sorry - designer
	//	if (Features.DroppedGuns) {
	//		if (strstr(entity->clientclass()->network_name, ("CWeapon"))) {
	//			std::string data = strstr(entity->clientclass()->network_name, ("CWeapon"));
	//			std::transform(data.begin(), data.end(), data.begin(), ::tolower); //convert dropped weapons names to lowercase, looks cleaner - designer
	//			render::draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::main_font, clean_item_name(data), true, color(255, 255, 255));
	//		}

	//		if (class_id == class_ids::cak47)
	//			render::draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::main_font, "ak47", true, color(255, 255, 255));

	//		if (class_id == class_ids::cc4)
	//			render::draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::main_font, "bomb", true, color(255, 255, 255));

	//		if (class_id == class_ids::cdeagle)
	//			render::draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::main_font, "deagle", true, color(255, 255, 255));

	//		if (strstr(model_name, "w_defuser"))
	//			render::draw_text(dropped_weapon_position.x, dropped_weapon_position.y, render::main_font, "defuse kit", true, color(255, 255, 255));
	//	}
	//}
}