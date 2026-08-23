#include "Settings.h"

class MenuOpenCloseHandler :
	public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
	public REX::TSingleton<MenuOpenCloseHandler>
{
public:
	static void Install()
	{
		if (auto ui = RE::UI::GetSingleton())
			ui->AddEventSink<RE::MenuOpenCloseEvent>(GetSingleton());
	}

	RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
	{
		if (a_event && a_event->menuName == "MapMenu"sv)
		{
			if (auto camera = RE::PlayerCamera::GetSingleton())
			{
				if (a_event->opening)
				{
					auto worldspaceFOV = Settings::Runtime.defaultFOV;

					if (auto ui = RE::UI::GetSingleton())
					{
						if (auto mapMenu = ui->GetMenu<RE::MapMenu>();
							mapMenu && mapMenu->worldSpace)
						{
							auto iter = Settings::Runtime.worldSpaces.find(mapMenu->worldSpace->editorID.c_str());
							if (iter != Settings::Runtime.worldSpaces.end())
							{
								worldspaceFOV = iter->second;
							}
						}
					}

					defaultWorldFOV = camera->worldFOV;
					defaultFirstFOV = camera->firstPersonFOV;
					camera->worldFOV = worldspaceFOV;
					camera->firstPersonFOV = worldspaceFOV;
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
			Settings::Load();
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
