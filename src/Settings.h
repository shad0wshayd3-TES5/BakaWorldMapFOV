#pragma once

#include <glaze/glaze.hpp>

struct SettingsImpl
{
	float                        defaultFOV;
	std::map<std::string, float> worldSpaces;
};

namespace Settings
{
	static SettingsImpl Runtime;

	inline void Load()
	{
		(void)glz::read_file_json(Runtime, "Data/SKSE/plugins/BakaWorldMapFOV.json", std::string{});
	}
}
