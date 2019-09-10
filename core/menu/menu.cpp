#include "menu.hpp"
#include "features.hpp"
//	note from alpha:
//	i strongly suggest people to use namespaces insteas of classes and singletons
//	not only is it a bad habit, i overused singletons in this sdk, and they're not thread safe.

void Menu::Render()
{

	if (!menuOpened)
		return;

		zgui::poll_input("Counter-Strike: Global Offensive");

		//	could add an if statement but whatever
		if (zgui::begin_window("Project Impulse", { 500, 350 }, render::main_font))
		{
			zgui::begin_groupbox("esp", { 100, 150 }); {
				zgui::checkbox("rainbow", Features.Rainbow);
		//		zgui::checkbox("box esp", Features.Box);
		//		zgui::checkbox("health esp", Features.Health);
				zgui::checkbox("name esp", Features.Name);
		//		zgui::checkbox("weapon esp", Features.Weapon);
		//		zgui::checkbox("dropped weapons", Features.DroppedGuns);
			}
			zgui::end_groupbox();
			zgui::begin_groupbox("misc", { 100, 150 }); {
				zgui::checkbox("bhop", Features.Bhop);
				zgui::checkbox("legit aa", Features.Desync);
				zgui::checkbox("watermark", Features.Watermark);
			//	zgui::checkbox("clantag", Features.ClanTag);	
			}
			zgui::end_groupbox();
			zgui::next_column(105, 0);
			zgui::begin_groupbox("legit", { 100, 150 });
			{
				zgui::checkbox("enabled###legit", Features.Legitbot);
				if (Features.Legitbot) {
					zgui::slider_int("fov:", 0, 20, Features.LegitFov);
					zgui::slider_int("smooth:", 0, 100, Features.LegitSmoothing);
					zgui::checkbox("backtrack###legit", Features.Backtrack);
					zgui::slider_float("timer:###legit", 0, 200, Features.BacktrackTime);
				}

			}
			zgui::end_groupbox();
			zgui::begin_groupbox("rage", { 100, 150 });
			{
				zgui::checkbox("enabled###rage", Features.Ragebot);
				if (Features.Ragebot) {
					zgui::checkbox("autoshoot", Features.AutoShoot);
				}
			}
			zgui::end_window();
		}
}