#include "..//..//dependencies/common_includes.hpp"
#include "zgui/zgui.hpp"

class Menu
{
public:
	bool menuOpened;
	void Render();
	struct {
		bool legitbot, ragebot, visual, misc;
		int Config;
	}Tabs;
private:
	struct
	{
		float x = 0.f, y = 0.f;
	}Pos; // lol
	void legittab();
	void ragetab();
	void vistab();
	void misctab();

	int ControlsX;
	int ControlsX2;
	int GroupTabBottom;
	int OffsetY;
	int OldOffsetY;
	int TabOffset;
	int SubTabOffset;
	float SubTabSize; // cpp fuckin sux had to make this a float or the whole thing crashes
	float TabSize;
	int GroupTabPos[4];
	bool ControlsReturn;
	int TabNum = 0;
	int SubTabNum = 0;
	int PreviousControl = -1;
}; extern Menu menu;