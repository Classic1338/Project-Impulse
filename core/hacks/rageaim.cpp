#include "rageaim.h"
c_rage rage;

void c_rage::run(c_usercmd* user_cmd)
{
	auto pLocal = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	if (!pLocal)
		return;

	for (int i = 1; i <= interfaces::globals->max_clients; ++i)
	{
		 
	}
}