#include "ChatBot.h"


ACommand * ChatBot::identifyUserDataMessage(const char* msg, Guest &sender, bool isHost)
{
	const uint32_t previous = this->_lastUserId;

	setLastUserId(BOT_GUESTID);

	// Pleb commands
	//if (msgIsEqual(msg, CommandAFK::prefixes()))		return new CommandAFK(_guests, _gamepadClient);
	if (msgIsEqual(msg, CommandDiscord::prefixes()))	return new CommandDiscord(sender);
	if (msgIsEqual(msg, CommandFF::prefixes()))			return new CommandFF(sender, _gamepadClient);
	if (msgIsEqual(msg, Command8Ball::prefixes()))		return new Command8Ball(sender);
	if (msgIsEqual(msg, CommandFortune::prefixes()))	return new CommandFortune(sender);
	if (msgIsEqual(msg, CommandHelp::prefixes()))		return new CommandHelp(sender, _tierList);
	//if (CommandIpFilter::containsIp(msg))				return new CommandIpFilter(msg, sender, _parsec, _ban, isHost);
	if (msgIsEqual(msg, CommandJoin::prefixes()))		return new CommandJoin();
	if (msgIsEqual(msg, CommandMirror::prefixes()))		return new CommandMirror(sender, _gamepadClient);
	if (msgIsEqual(msg, CommandOne::prefixes()))		return new CommandOne(sender, _gamepadClient);
	if (msgIsEqual(msg, CommandPads::prefixes()))		return new CommandPads(_gamepadClient);
	if (msgStartsWith(msg, CommandSpectate::prefixes()))	return new CommandSpectate(msg, sender, _guests, _tierList);
	if (msgStartsWith(msg, CommandRequest::prefixes()))	return new CommandRequest(msg);
	if (msgStartsWith(msg, CommandSwap::prefixes()))	return new CommandSwap(msg, sender, _gamepadClient);
	//if (msgStartsWith(msg, CommandTeams::prefixes()))	return new CommandTeams(msg, sender, _guests, _gamepadClient, _tierList);

//#if !BASIC_VERSION
	if (!_basicVersion)
	{
		if (msgStartsWith(msg, CommandSFX::prefixes()))		return new CommandSFX(msg, _sfxList);
		if (msgStartsWith(msg, CommandBonk::prefixes()))	return new CommandBonk(msg, sender, _guests, _host);
	}
//#endif
	

	Tier tier = _tierList.getTier(sender.userID);

	// Admin & Moderator commands
	if (tier >= Tier::ADMIN || isHost)
	{
		if (msgStartsWith(msg, CommandBan::prefixes()))			return new CommandBan(msg, sender, _parsec, _guests, _guestHistory, _ban);
		if (msgStartsWith(msg, CommandBB::prefixes()))			return new CommandBB(_gamepadClient);
		if (msgStartsWith(msg, CommandHotseat::prefixes()))		return new CommandHotseat(sender);
		if (msgStartsWith(msg, CommandDC::prefixes()))			return new CommandDC(msg, _gamepadClient);
		if (msgStartsWith(msg, CommandDecrease::prefixes()))	return new CommandDecrease(msg);
		if (msgStartsWith(msg, CommandExtend::prefixes()))		return new CommandExtend(msg);
		if (msgStartsWith(msg, CommandKick::prefixes()))		return new CommandKick(msg, sender, _parsec, _guests, isHost);
		if (msgStartsWith(msg, CommandLimit::prefixes()))		return new CommandLimit(msg, _guests, _gamepadClient);
		if (msgStartsWith(msg, CommandMute::prefixes()))		return new CommandMute(msg, sender, _guests, _host);
		if (msgStartsWith(msg, CommandPing::prefixes()))		return new CommandPing(msg, sender, _guests, _host);
		if (msgStartsWith(msg, CommandRestart::prefixes()))		return new CommandRestart();
		if (msgStartsWith(msg, CommandStrip::prefixes()))		return new CommandStrip(msg, sender, _gamepadClient);
		if (msgStartsWith(msg, CommandTimer::prefixes()))		return new CommandTimer(msg);
		if (msgStartsWith(msg, CommandUnban::prefixes()))		return new CommandUnban(msg, sender, _ban, _guestHistory);

		// Tournaments
		if (msgStartsWith(msg, Command2v2::prefixes()))		return new Command2v2(msg, _guests, _gamepadClient);
	}

	// God commands
	if (tier >= Tier::GOD || isHost)
	{
		if (msgStartsWith(msg, CommandGameId::prefixes()))		return new CommandGameId(msg, _hostConfig);
		if (msgStartsWith(msg, CommandGuests::prefixes()))		return new CommandGuests(msg, _hostConfig);
		if (msgStartsWith(msg, CommandMic::prefixes()))			return new CommandMic(msg, _audioIn);
		if (msgStartsWith(msg, CommandName::prefixes()))		return new CommandName(msg, _hostConfig);
		if (msgIsEqual(msg, CommandPrivate::prefixes()))		return new CommandPrivate(_hostConfig);
		if (msgIsEqual(msg, CommandPublic::prefixes()))			return new CommandPublic(_hostConfig);
		if (msgIsEqual(msg, CommandSetConfig::prefixes()))		return new CommandSetConfig(_parsec, &_hostConfig, _parsecSession.sessionId.c_str());
		if (msgStartsWith(msg, CommandSpeakers::prefixes()))	return new CommandSpeakers(msg, _audioOut);
		if (msgIsEqual(msg, CommandQuit::prefixes()))			return new CommandQuit(_hostingLoopController);
		if (msgStartsWith(msg, CommandMod::prefixes()))			return new CommandMod(msg, sender, _parsec, _guests, _guestHistory, _mod, _tierList);
		if (msgStartsWith(msg, CommandUnmod::prefixes()))		return new CommandUnmod(msg, sender, _mod, _guestHistory, _tierList);
	}

	this->setLastUserId(previous);
	return new CommandDefaultMessage(msg, sender, _lastUserId, tier, isHost);
}

const uint32_t ChatBot::getLastUserId() const
{
	return this->_lastUserId;
}

const std::string ChatBot::formatGuestConnection(Guest guest, ParsecGuestState state, ParsecStatus status)
{
	setLastUserId(BOT_GUESTID);

	std::ostringstream reply;
	if (state == GUEST_CONNECTED)
	{
		reply << "@join \t\t " << guest.name << " #" << guest.userID << "\0";
	}
	else if (state == GUEST_FAILED)
	{
		reply << "!" << status << " \t\t " << guest.name << " #" << guest.userID << "\0";
	}
	else
	{
		switch (status)
		{
			case 5:
				reply << "!kick \t\t " << guest.name << " #" << guest.userID << "\0";
				break;
			case 11:
				reply << "!full \t\t " << guest.name << " #" << guest.userID << "\0";
				break;
			case -12007:
				reply << "!timeout \t\t " << guest.name << " #" << guest.userID << "\0";
				break;
			case -13014:
				reply << "!quit \t\t " << guest.name << " #" << guest.userID << "\0";
				break;
			default:
				reply << "!" << status << " \t\t " << guest.name << " #" << guest.userID << "\0";
				break;
		}
	}

	const std::string formattedMessage = reply.str();
	reply.clear();
	reply.flush();
	return formattedMessage;
}

const std::string ChatBot::formatBannedGuestMessage(Guest guest)
{
	std::ostringstream reply;
	reply << "!ban \t\t " << guest.name << " #" << guest.userID << "\0";

	return reply.str();
}

const std::string ChatBot::formatModGuestMessage(Guest guest) {

	std::ostringstream reply;
	reply << MetadataCache::preferences.chatbotName + " | Moderator \n\t\t" << guest.name << " \t (#" << guest.userID << ") has joined!\0";

	return reply.str();

}

CommandBotMessage ChatBot::sendBotMessage(const char* msg)
{
	CommandBotMessage message(msg);
	message.run();
	setLastUserId(BOT_GUESTID);
	return message;
}

bool ChatBot::msgStartsWith(const char* msg, const char * pattern)
{
	return Stringer::startsWithPattern(msg, pattern);
}

bool ChatBot::msgStartsWith(const char* msg, vector<const char*> patterns)
{
	vector<const char*>::iterator pi = patterns.begin();
	for (; pi != patterns.end(); ++pi)
	{
		if (Stringer::startsWithPattern(msg, *pi))
		{
			return true;
		}
	}
	return false;
}

bool ChatBot::msgIsEqual(const char * msg, const char * pattern)
{
	return (strcmp(msg, pattern) == 0);
}

bool ChatBot::msgIsEqual(const char* msg, vector<const char*> patterns)
{
	vector<const char*>::iterator pi = patterns.begin();
	for (; pi != patterns.end(); ++pi)
	{
		if ((strcmp(msg, *pi) == 0))
		{
			return true;
		}
	}
	return false;
}

void ChatBot::setLastUserId(uint32_t lastId)
{
	this->_lastUserId = lastId;
}

void ChatBot::updateSettings()
{
	_basicVersion = MetadataCache::preferences.basicVersion;
}