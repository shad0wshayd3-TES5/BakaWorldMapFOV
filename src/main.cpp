namespace Config
{
	namespace General
	{
		static REX::INI::F32 fWorldMapFOV{ "General"sv, "fWorldMapFOV"sv, 80.0f };
	}

	namespace Runtime
	{
		static float                        fWorldMapFOV{ 80.0f };
		static std::map<std::string, float> mWorldspaces;
	}

	static void Load()
	{
		const auto ini = REX::INI::SettingStore::GetSingleton();
		ini->Init(
			"Data/SKSE/plugins/BakaWorldMapFOV.ini",
			"Data/SKSE/plugins/BakaWorldMapFOVCustom.ini");
		ini->Load();

		Runtime::fWorldMapFOV = General::fWorldMapFOV;
		if (auto data = RE::TESDataHandler::GetSingleton())
		{
			for (auto world : data->formArrays[71])
			{
				auto editorID = world->GetFormEditorID();
				auto fileBase = std::format("Data/SKSE/plugins/BakaWorldMapFOV.{}.ini"sv, editorID);

				if (!std::filesystem::exists(fileBase))
					continue;

				ini->Init(fileBase.c_str(), "");
				ini->Load();

				Runtime::mWorldspaces.emplace(editorID, General::fWorldMapFOV);
			}
		}
	}
}

class MenuOpenCloseHandler :
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
	public REX::TSingleton<MenuOpenCloseHandler>
{
public:
	static void Install()
	{
		if (auto ui = RE::UI::GetSingleton())
			ui->AddEventSink<RE::MenuOpenCloseEvent>(MenuOpenCloseHandler::GetSingleton());
	}

	RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
	{
		if (a_event && a_event->menuName == "MapMenu"sv)
		{
			if (auto camera = RE::PlayerCamera::GetSingleton())
			{
				if (a_event->opening)
				{
					auto fWorldMapFOV = Config::Runtime::fWorldMapFOV;
					if (auto ui = RE::UI::GetSingleton())
					{
						if (auto map = ui->GetMenu<RE::MapMenu>())
						{
							if (map->worldSpace)
							{
								auto editorID = map->worldSpace->GetFormEditorID();
								if (Config::Runtime::mWorldspaces.contains(editorID))
								{
									fWorldMapFOV = Config::Runtime::mWorldspaces[editorID];
								}
							}
						}
					}

					defaultWorldFOV = camera->worldFOV;
					defaultFirstFOV = camera->firstPersonFOV;
					camera->worldFOV = fWorldMapFOV;
					camera->firstPersonFOV = fWorldMapFOV;
				}
				else
				{
					camera->worldFOV = defaultWorldFOV;
					camera->firstPersonFOV = defaultFirstFOV;
				}
			}
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	inline static auto defaultWorldFOV{ 80.0f };
	inline static auto defaultFirstFOV{ 80.0f };
};

namespace
{
	void MessageCallback(SKSE::MessagingInterface::Message* a_msg) noexcept
	{
		switch (a_msg->type)
		{
		case SKSE::MessagingInterface::kPostLoad:
			MenuOpenCloseHandler::Install();
			break;
		case SKSE::MessagingInterface::kDataLoaded:
			Config::Load();
			break;
		default:
			break;
		}
	}
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
	SKSE::GetMessagingInterface()->RegisterListener(MessageCallback);
	return true;
}
