#pragma once

namespace hooks {
	extern std::unique_ptr<vmt_hook> client_hook;
	extern std::unique_ptr<vmt_hook> clientmode_hook;
	extern std::unique_ptr<vmt_hook> panel_hook;
	extern std::unique_ptr<vmt_hook> surface_hook;
	extern std::unique_ptr<vmt_hook> renderview_hook;
	extern std::unique_ptr<vmt_hook> modelrender_hook;
	extern WNDPROC wndproc_original;
	extern HWND window;

	void initialize( );
	void shutdown( );

	using create_move_fn = bool( __thiscall* )( i_client_mode*, float, c_usercmd* );
	using frame_stage_notify_fn = void( __thiscall* )( i_base_client_dll*, int );
	using paint_traverse_fn = void( __thiscall* )( i_panel*, unsigned int, bool, bool );
	using scene_end_fn = void( __thiscall* )( void* );
	using lock_cursor_fn = void(__thiscall*)(void*);
	using draw_model_execute_fn = void(__thiscall*)(iv_model_render*, IMatRenderContext*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
	using present_fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
	using reset_fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

	bool __stdcall create_move( float frame_time, c_usercmd* user_cmd );
	//void __stdcall frame_stage_notify( int frame_stage );
	void __stdcall paint_traverse( unsigned int panel, bool force_repaint, bool allow_force );
	//long __stdcall present(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) noexcept;
	//long __stdcall reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) noexcept;
	void __stdcall lock_cursor() noexcept;
	void __stdcall scene_end( );
	void __stdcall draw_model_execute(IMatRenderContext * ctx, const draw_model_state_t & state, const model_render_info_t & info, matrix_t * bone_to_world)noexcept;
	LRESULT __stdcall wndproc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) noexcept;
}