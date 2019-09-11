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
		zgui::begin_groupbox("esp", { 100, 150 }); 
		zgui::checkbox("rainbow", Features.Rainbow);
		zgui::checkbox("name esp", Features.Name);
		zgui::checkbox("box esp", Features.Box);
	}
	zgui::end_groupbox();
	zgui::begin_groupbox("misc", { 100, 150 }); 
	{
		zgui::checkbox("bhop", Features.Bhop);
		zgui::checkbox("legit aa", Features.Desync);
		zgui::checkbox("watermark", Features.Watermark);
		zgui::checkbox("clantag", Features.ClanTag);	
	}
	zgui::end_groupbox();
	zgui::end_window();
}
