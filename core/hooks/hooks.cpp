#pragma once
#include "..//menu/menu.hpp"
#include "../../dependencies/common_includes.hpp"
#include <algorithm>
#include "../hacks/esp.h"
#include "../menu/features.hpp"
#include "../hacks/misc.h"
#include "../hacks/rageaim.h"

std::unique_ptr<vmt_hook> hooks::client_hook;
std::unique_ptr<vmt_hook> hooks::clientmode_hook;
std::unique_ptr<vmt_hook> hooks::panel_hook;typedef int ImageFormat;
std::unique_ptr<vmt_hook> hooks::renderview_hook;
std::unique_ptr<vmt_hook> hooks::surface_hook;
std::unique_ptr<vmt_hook> hooks::modelrender_hook;
WNDPROC hooks::wndproc_original = NULL;
uint8_t* present_address;
hooks::present_fn original_present;
uint8_t* reset_address;
hooks::reset_fn original_reset;
HWND hooks::window;

Esp esp;
Menu menu;
c_misc misc;

void hooks::initialize( ) {
	client_hook = std::make_unique<vmt_hook>( );
	clientmode_hook = std::make_unique<vmt_hook>( );
	panel_hook = std::make_unique<vmt_hook>( );
	renderview_hook = std::make_unique<vmt_hook>( );
	modelrender_hook = std::make_unique<vmt_hook>();
	surface_hook = std::make_unique<vmt_hook>();

	client_hook->setup( interfaces::client );
//	client_hook->hook_index( 37, reinterpret_cast< void* >( frame_stage_notify ) );

	surface_hook->setup(interfaces::surface);
	surface_hook->hook_index(67, reinterpret_cast<void*>( lock_cursor ) );

	modelrender_hook->setup(interfaces::model_render);
	modelrender_hook->hook_index(21, reinterpret_cast<void*>(draw_model_execute));

	clientmode_hook->setup( interfaces::clientmode );
	clientmode_hook->hook_index(24, reinterpret_cast<void*>(create_move));

	panel_hook->setup( interfaces::panel );
	panel_hook->hook_index( 41, reinterpret_cast< void* >( paint_traverse ) );

	renderview_hook->setup( interfaces::render_view );
	renderview_hook->hook_index( 9, reinterpret_cast< void* >( scene_end ) );

//	present_address = utilities::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB") + 0x2;
//	reset_address = utilities::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 0x2;

//	original_present = **reinterpret_cast<present_fn**>(present_address);
//	original_reset = **reinterpret_cast<reset_fn**>(reset_address);

//	**reinterpret_cast<void***>(present_address) = reinterpret_cast<void*>(&present);
//	**reinterpret_cast<void***>(reset_address) = reinterpret_cast<void*>(&reset);

	window = FindWindowW(L"Valve001", NULL);
	wndproc_original = reinterpret_cast<WNDPROC>(SetWindowLongW(window, GWL_WNDPROC, reinterpret_cast<LONG>(wndproc)));

	interfaces::console->get_convar( "mat_queue_mode" )->set_value( 0 );
	interfaces::console->get_convar( "viewmodel_fov" )->callbacks.SetSize( 0 );
	interfaces::console->get_convar( "mat_postprocess_enable" )->set_value( 0 );
	interfaces::console->get_convar( "crosshair" )->set_value( 1 );

	render::setup_fonts( );

	zgui::functions.draw_line = render::line;
	zgui::functions.draw_rect = render::rect;
	zgui::functions.draw_filled_rect = render::filled_rect;
	zgui::functions.draw_text = render::text;
	zgui::functions.get_text_size = render::get_text_size;
	zgui::functions.get_frametime = render::get_frametime;
}

void hooks::shutdown( ) {
	clientmode_hook->release( );
	client_hook->release( );
	panel_hook->release( );
	renderview_hook->release( );
	surface_hook->release();

	SetWindowLongW(FindWindowW(L"Valve001", NULL), GWL_WNDPROC, reinterpret_cast<LONG>(wndproc_original));
}

void __stdcall hooks::draw_model_execute(IMatRenderContext * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix_t * bone_to_world) noexcept {
	static auto original_fn = reinterpret_cast<draw_model_execute_fn>(modelrender_hook->get_original(21));

	esp.chams();

	original_fn(interfaces::model_render, ctx, state, info, bone_to_world);
}


bool __stdcall hooks::create_move(float frame_time, c_usercmd* user_cmd) {
	static auto original_fn = reinterpret_cast<create_move_fn>(clientmode_hook->get_original(24))(interfaces::clientmode, frame_time, user_cmd);
	if (!user_cmd || !user_cmd->command_number)
		return original_fn;

	if (!interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()))
		return original_fn;

	bool& send_packet = *reinterpret_cast<bool*>(*(static_cast<uintptr_t*>(_AddressOfReturnAddress()) - 1) - 0x1C);

	auto pLocal = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	auto desync = pLocal->MaxDelta(pLocal);

	if (Features.Desync) 
	{
		//send_packet = !send_packet;
		if (send_packet)
		{
			user_cmd->view_angles.y -= 45.f;
		}

	}

	if (Features.Bhop)
	{
		if (user_cmd->buttons & in_jump/*check if on buttons you are jumping*/ && !(pLocal->flags() & fl_onground)/*check if you are in air*/)
			user_cmd->buttons &= ~in_jump;/*if you are in air and on buttons still are in jump, remove the jump so when you land u can jump again*/
	}

	
	rage.run(user_cmd);
	misc.clantag_spammer();

	user_cmd->buttons |= in_bullrush;

	//clamping movement
	user_cmd->forward_move = std::clamp(user_cmd->forward_move, -450.0f, 450.0f);
	user_cmd->side_move = std::clamp(user_cmd->side_move, -450.0f, 450.0f);
	user_cmd->up_move = std::clamp(user_cmd->up_move, -450.0f, 450.0f);

	// clamping angles
	user_cmd->view_angles.x = std::clamp(user_cmd->view_angles.x, -89.0f, 89.0f);
	user_cmd->view_angles.y = std::clamp(user_cmd->view_angles.y, -180.0f, 180.0f);
	user_cmd->view_angles.z = 0.0f;
	return false;
}

//void __stdcall hooks::frame_stage_notify( int frame_stage ) {
//	reinterpret_cast< frame_stage_notify_fn >( client_hook->get_original( 37 ) )( interfaces::client, frame_stage );
//}

void __stdcall hooks::paint_traverse( unsigned int panel, bool force_repaint, bool allow_force ) {
	std::string panel_name = interfaces::panel->get_panel_name( panel );

	reinterpret_cast< paint_traverse_fn >( panel_hook->get_original( 41 ) )( interfaces::panel, panel, force_repaint, allow_force );

	static unsigned int _panel = 0;
	static bool once = false;

	if ( !once ) {
		PCHAR panel_char = ( PCHAR ) interfaces::panel->get_panel_name( panel );
		if ( strstr( panel_char, "MatSystemTopPanel" ) ) {
			_panel = panel;
			once = true;
		}
	}
	else if ( _panel == panel ) {
		menu.Render();
		esp.loop();
		misc.watermark();
	}
}

void __stdcall hooks::scene_end( ) {


	reinterpret_cast< scene_end_fn >( renderview_hook->get_original( 9 ) )( interfaces::render_view );
}

#include "../menu/zgui/zgui.hpp"

LRESULT __stdcall hooks::wndproc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) noexcept {

	static bool pressed = false;

	if (!pressed && GetKeyState(VK_INSERT) & 1) {
		pressed = true;
	}
	else if (pressed && !GetKeyState(VK_INSERT) & 1) {
		pressed = false;
		
		menu.menuOpened = !menu.menuOpened;
	}

	if (menu.menuOpened) {
		interfaces::inputsystem->enable_input(false);

	}
	else if (!menu.menuOpened) {
		interfaces::inputsystem->enable_input(true);
	}

	if (menu.menuOpened && GetKeyState(VK_INSERT) & 1)
		return true;

	return CallWindowProcW(wndproc_original, hwnd, message, wparam, lparam);
}

void __stdcall hooks::lock_cursor() noexcept {

	if (GetKeyState(VK_INSERT) & 1)
		menu.menuOpened = true;

	static auto original_fn = reinterpret_cast<lock_cursor_fn>(surface_hook->get_original(67));

	if (menu.menuOpened) {
		interfaces::surface->unlock_cursor();
		return;
	}

	original_fn(interfaces::surface);

}