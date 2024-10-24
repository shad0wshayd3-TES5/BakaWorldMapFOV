SKSEPluginLoad(const SKSE::LoadInterface* a_SKSE)
{
	SKSE::Init(a_SKSE, true);

	const auto plugin = SKSE::PluginVersionData::GetSingleton();
	SKSE::log::info("{} {} loaded", plugin->GetPluginName(), plugin->GetPluginVersion());

	return true;
}
