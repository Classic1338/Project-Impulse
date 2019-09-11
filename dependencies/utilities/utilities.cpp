#include "utilities.hpp"
#include "../common_includes.hpp"
#include <psapi.h>

std::uint8_t* utilities::pattern_scan( void* module, const char* signature ) {
	static auto pattern_to_byte = [ ] ( const char* pattern ) {
		auto bytes = std::vector<int> {};
		auto start = const_cast< char* >( pattern );
		auto end = const_cast< char* >( pattern ) + strlen( pattern );

		for ( auto current = start; current < end; ++current ) {
			if ( *current == '?' ) {
				++current;
				if ( *current == '?' )
					++current;
				bytes.push_back( -1 );
			}
			else {
				bytes.push_back( strtoul( current, &current, 16 ) );
			}
		}
		return bytes;
	};

	auto dos_headers = ( PIMAGE_DOS_HEADER ) module;
	auto nt_headers = ( PIMAGE_NT_HEADERS ) ( ( std::uint8_t* )module + dos_headers->e_lfanew );

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte( signature );
	auto scan_bytes = reinterpret_cast< std::uint8_t* >( module );

	auto s = pattern_bytes.size( );
	auto d = pattern_bytes.data( );

	for ( auto i = 0ul; i < size_of_image - s; ++i ) {
		bool found = true;
		for ( auto j = 0ul; j < s; ++j ) {
			if (scan_bytes[ i + j ] != d [ j ] && d [ j ] != -1 ) {
				found = false;
				break;
			}
		}
		if ( found ) {
			return &scan_bytes[ i ];
		}
	}
	return nullptr;
}

bool utilities::is_behind_smoke(vec3_t start_pos, vec3_t end_pos) {
	typedef bool(__cdecl* line_goes_through_smoke) (vec3_t, vec3_t);
	static line_goes_through_smoke line_goes_through_smoke_fn = 0;

	if (!line_goes_through_smoke_fn)
		line_goes_through_smoke_fn = reinterpret_cast<line_goes_through_smoke>(utilities::pattern_scan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));

	if (line_goes_through_smoke_fn)
		return line_goes_through_smoke_fn(start_pos, end_pos);
	return false;
}

void utilities::apply_clan_tag(const char * name) {
	using Fn = int(__fastcall *)(const char *, const char *);
	static auto apply_clan_tag_fn = reinterpret_cast<Fn>(utilities::pattern_scan(GetModuleHandleA("engine.dll"), "53 56 57 8B DA 8B F9 FF 15"));
	apply_clan_tag_fn(name, name);
};

void* utilities::game::capture_interface( const char* mod, const char* iface ) {
	using fn_capture_iface_t = void*( *)( const char*, int* );
	auto fn_addr = ( fn_capture_iface_t ) GetProcAddress( GetModuleHandleA( mod ), "CreateInterface" );

	auto iface_addr = fn_addr( iface, nullptr );
	printf( "found %s at 0x%p\n", iface, fn_addr( iface, nullptr ) );

	return iface_addr;
}
