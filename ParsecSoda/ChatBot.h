#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include "parsec-dso.h"
#include "ParsecSession.h"
#include "Stringer.h"
#include "GamepadClient.h"
#include "DX11.h"
#include "TierList.h"

#include "Commands/ACommand.h"
#include "Commands/Command2v2.h"
#include "Commands/Command8ball.h"
#include "Commands/CommandAFK.h"
#include "Commands/CommandBan.h"
#include "Commands/CommandBB.h"
#include "Commands/CommandBonk.h"
#include "Commands/CommandBotMessage.h"
#include "Commands/CommandDecrease.h"
#include "Commands/CommandDefaultMessage.h"
#include "Commands/CommandDiscord.h"
#include "Commands/CommandDC.h"
#include "Commands/CommandExtend.h"
#include "Commands/CommandFF.h"
#include "Commands/CommandFortune.h"
#include "Commands/CommandGameId.h"
#include "Commands/CommandGuests.h"
#include "Commands/CommandHelp.h"
#include "Commands/CommandHotseat.h"
#include "Commands/CommandIpFilter.h"
#include "Commands/CommandJoin.h"
#include "Commands/CommandKick.h"
#include "Commands/CommandMic.h"
#include "Commands/CommandMirror.h"
#include "Commands/CommandMod.h"
#include "Commands/CommandMute.h"
#include "Commands/CommandName.h"
#include "Commands/CommandLimit.h"
#include "Commands/CommandOne.h"
#include "Commands/CommandPads.h"
#include "Commands/CommandPing.h"
#include "Commands/CommandPrivate.h"
#include "Commands/CommandPublic.h"
#include "Commands/CommandQuit.h"
#include "Commands/CommandRestart.h"
#include "Commands/CommandRequest.h"
#include "Commands/CommandSetConfig.h"
#include "Commands/CommandSFX.h"
#include "Commands/CommandSpeakers.h"
#include "Commands/CommandSpectate.h"
#include "Commands/CommandStrip.h"
#include "Commands/CommandSwap.h"
#include "Commands/CommandTimer.h"
#include "Commands/CommandUnban.h"
#include "Commands/CommandUnmod.h"
#include "Commands/CommandVideoFix.h"
#include "CompilerDirectives.h"
#include "MetadataCache.h"

#define BOT_GUESTID 0

class ChatBot
{
public:

	ChatBot(
		AudioIn& audioIn, AudioOut& audioOut, BanList& ban, DX11& dx11, ModList& mod,
		GamepadClient& gamepadClient, GuestList& guests, GuestDataList& guestHistory, ParsecDSO* parsec, ParsecHostConfig& hostConfig,
		ParsecSession& parsecSession, SFXList& sfxList, TierList& _tierList, bool& hostingLoopController, Guest& host
	)
		: _audioIn(audioIn), _audioOut(audioOut), _ban(ban), _dx11(dx11), _mod(mod), _gamepadClient(gamepadClient),
		_guests(guests), _guestHistory(guestHistory), _parsec(parsec), _hostConfig(hostConfig), _parsecSession(parsecSession),
		_sfxList(sfxList), _tierList(_tierList), _hostingLoopController(hostingLoopController), _host(host)
	{
		_basicVersion = MetadataCache::preferences.basicVersion;
	}

	ACommand * identifyUserDataMessage(const char* msg, Guest& sender, bool isHost = false);

	const uint32_t getLastUserId() const;
	void setLastUserId(const uint32_t lastId = BOT_GUESTID);

	const std::string formatGuestConnection(Guest guest, ParsecGuestState state, ParsecStatus status);
	const std::string formatBannedGuestMessage(Guest guest);
	const std::string formatModGuestMessage(Guest guest);
	CommandBotMessage sendBotMessage(const char * msg);
	void updateSettings();

private:
	static bool msgStartsWith(const char* msg, const char* pattern);
	static bool msgStartsWith(const char* msg, vector<const char*> patterns);
	static bool msgIsEqual(const char* msg, const char* pattern);
	static bool msgIsEqual(const char* msg, vector<const char*> patterns);

	uint32_t _lastUserId = 0;

	// Dependency Injection
	ParsecDSO* _parsec;
	AudioIn& _audioIn;
	AudioOut& _audioOut;
	BanList &_ban;
	ModList& _mod;
	DX11 &_dx11;
	GamepadClient& _gamepadClient;
	GuestList& _guests;
	GuestDataList& _guestHistory;
	ParsecHostConfig &_hostConfig;
	ParsecSession &_parsecSession;
	SFXList& _sfxList;
	TierList& _tierList;
	bool &_hostingLoopController;
	Guest& _host;
	bool _basicVersion = false;
};