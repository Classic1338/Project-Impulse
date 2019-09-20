#include "menu.hpp"
#include "features.hpp"
//	note from alpha:
//	i strongly suggest people to use namespaces insteas of classes and singletons
//	not only is it a bad habit, i overused singletons in this sdk, and they're not thread safe.


void Menu::Render()
{
	if (!menuOpened)
		return;

	zgui::poll_input("Valve001");

	if (!Features.OldMenu) {
		if (zgui::begin_window("Project Impulse", { 500, 600 }, render::main_font, zgui::zgui_window_flags_none))
		{
			if (zgui::tab_button("legitbot", { 69, 30 }, Tabs.legitbot))
			{
				Tabs.legitbot = true;
				Tabs.ragebot = false;
				Tabs.visual = false;
				Tabs.misc = false;
			}

			zgui::same_line();
			zgui::next_column(0, 0);
			if (zgui::tab_button("ragebot", { 69, 30 }, Tabs.ragebot))
			{
				Tabs.legitbot = false;
				Tabs.ragebot = true;
				Tabs.visual = false;
				Tabs.misc = false;
			}
			zgui::same_line();
			zgui::next_column(0, 0);

			if (zgui::tab_button("visuals", { 69, 30 }, Tabs.visual))
			{
				Tabs.legitbot = false;
				Tabs.ragebot = false;
				Tabs.visual = true;
				Tabs.misc = false;
			}

			zgui::same_line();
			zgui::next_column(0, 0);


			if (zgui::tab_button("misc", { 69, 30 }, Tabs.misc))
			{
				Tabs.legitbot = false;
				Tabs.ragebot = false;
				Tabs.visual = false;
				Tabs.misc = true;
			}

			zgui::same_line();
			zgui::next_column(0, 0);


			if (Tabs.legitbot)
				legittab();

			if (Tabs.ragebot)
				ragetab();

			if (Tabs.visual)
				vistab();

			if (Tabs.misc)
				misctab();


			zgui::end_window();
		}
	}

	if (Features.OldMenu) {
		if (zgui::begin_window("Project Impulse", { 500, 350 }, render::main_font))
		{
			zgui::begin_groupbox("esp", { 100, 150 });
			zgui::checkbox("rainbow", Features.Rainbow);
			zgui::checkbox("name esp", Features.Name);
			zgui::checkbox("box esp", Features.Box);
			zgui::checkbox("health esp", Features.Health);
		}
		zgui::end_groupbox();
		zgui::begin_groupbox("misc", { 100, 150 });
		{
			zgui::checkbox("bhop", Features.Bhop);
			zgui::checkbox("legit aa", Features.Desync);
			zgui::checkbox("watermark", Features.Watermark);
			zgui::checkbox("clantag", Features.ClanTag);
			zgui::checkbox("backtrack", Features.Backtrack);
			zgui::checkbox("new menu", Features.OldMenu);
		}
		zgui::end_window();
	}
	zgui::end_groupbox();
}

void  Menu::legittab() {
	zgui::begin_groupbox("legit", { 100, 150 });
	zgui::checkbox("backtrack", Features.Backtrack);
}

void  Menu::ragetab() {
	zgui::begin_groupbox("rage", { 100, 150 });
	zgui::checkbox("ragebot", Features.Ragebot);
}

void Menu::vistab() {
	zgui::begin_groupbox("visuals", { 100, 150 });
	zgui::checkbox("rainbow", Features.Rainbow);
	zgui::checkbox("name esp", Features.Name);
	zgui::checkbox("box esp", Features.Box);
	zgui::checkbox("health esp", Features.Health);
}

void Menu::misctab() {
	zgui::begin_groupbox("misc", { 100, 150 });
	zgui::checkbox("bhop", Features.Bhop);
	zgui::checkbox("watermark", Features.Watermark);
	zgui::checkbox("new menu", Features.OldMenu);
}