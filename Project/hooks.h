#ifndef HOOKS_H
#define HOOKS_H

#include "display.h"

#include <string>

typedef LRESULT(*HOOK)(HWND, UINT, WPARAM, LPARAM);

namespace hooks {
	void add(int msg, std::string name, HOOK func);
	HOOK get(std::string name);
};

//void addMessageHook(int message, HOOK hook, std::string name);
//HOOK getHook(int message, std::string name);
//void removeMessageHook(int message, HOOK hook);

#endif // HOOKS_H