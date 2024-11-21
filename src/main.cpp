namespace Config
{
	namespace General
	{
		static REX::INI::F32 fWorldMapFOV{ "General", "fWorldMapFOV", 80.0f };
	}

	static void Load()
	{
		const auto ini = REX::INI::SettingStore::GetSingleton();
		ini->Init(
			"Data/SKSE/plugins/BakaWorldMapFOV.ini",
			"Data/SKSE/plugins/BakaWorldMapFOVCustom.ini");
		ini->Load();
	}
}

class MenuOpenCloseHandler :
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
public:
	static void Install()
	{
		Config::Load();

		if (auto UI = RE::UI::GetSingleton())
		{
			UI->AddEventSink<RE::MenuOpenCloseEvent>(MenuOpenCloseHandler::GetSingleton());
			SKSE::log::debug("Added MenuOpenCloseEvent handler");
		}
	}

	static MenuOpenCloseHandler* GetSingleton()
	{
		static MenuOpenCloseHandler singleton;
		return std::addressof(singleton);
	}

	RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
	{
		if (a_event && a_event->menuName == "MapMenu")
		{
			if (auto PlayerCamera = RE::PlayerCamera::GetSingleton())
			{
				if (a_event->opening)
				{
					auto fWorldMapFOV = Config::General::fWorldMapFOV.GetValue();
					defaultWorldFOV = PlayerCamera->worldFOV;
					defaultFirstFOV = PlayerCamera->firstPersonFOV;
					PlayerCamera->worldFOV = fWorldMapFOV;
					PlayerCamera->firstPersonFOV = fWorldMapFOV;
				}
				else
				{
					PlayerCamera->worldFOV = defaultWorldFOV;
					PlayerCamera->firstPersonFOV = defaultFirstFOV;
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
		{
			MenuOpenCloseHandler::Install();
			break;
		}
		default:
			break;
		}
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* a_SKSE)
{
	SKSE::Init(a_SKSE, true);
	SKSE::GetMessagingInterface()->RegisterListener(MessageCallback);

	return true;
}
