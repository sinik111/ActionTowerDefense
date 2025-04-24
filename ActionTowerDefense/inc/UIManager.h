#pragma once

#include "Singleton.h"

class UIManager :
	public Singleton<UIManager>
{
	friend class Singleton<UIManager>;

private:
	UIManager() = default;
	~UIManager() = default;


};