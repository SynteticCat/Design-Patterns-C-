# include <iostream>
# include <memory>
# include <string>

using namespace std;

class MusicPlayerState;

// наши 3 состояния
enum class State
{
	ST_STOPPED, ST_PLAYING, ST_PAUSED
};

class MusicPlayer
{
public:
	explicit MusicPlayer(MusicPlayerState* ptr) : pState(ptr) {}
	virtual ~MusicPlayer() = default;

	void Play();
	void Pause();
	void Stop();

	void SetState(State state);

private:
	unique_ptr<MusicPlayerState> pState;
};

// класс, определяющий состояние
class MusicPlayerState
{
public:
	MusicPlayerState(string nm) : name(nm) {}
	virtual ~MusicPlayerState() = 0;

	virtual void Play(MusicPlayer& player) {}
	virtual void Pause(MusicPlayer& player) {}
	virtual void Stop(MusicPlayer& player) {}

	string GetName() { return name; }

private:
	string   name;
};

MusicPlayerState::~MusicPlayerState() = default;

class PlayingState : public MusicPlayerState {
public:
	PlayingState() : MusicPlayerState(string("Playing")) {}
	virtual ~PlayingState() = default;

	virtual void Pause(MusicPlayer& player) override { player.SetState(State::ST_PAUSED); }
	virtual void Stop(MusicPlayer& player) override { player.SetState(State::ST_STOPPED); }
};

class PausedState : public MusicPlayerState {
public:
	PausedState() : MusicPlayerState(string("Paused")) {}
	virtual ~PausedState() = default;

	virtual void Play(MusicPlayer& player) override { player.SetState(State::ST_PLAYING); }
	virtual void Stop(MusicPlayer& player) override { player.SetState(State::ST_STOPPED); }
};

class StoppedState : public MusicPlayerState {
public:
	StoppedState() : MusicPlayerState(string("Stopped")) {}
	virtual ~StoppedState() = default;

	virtual void Play(MusicPlayer& player) override { player.SetState(State::ST_PLAYING); }
};

#pragma region Methods MusicPlayer
void MusicPlayer::Play() { pState->Play(*this); }
void MusicPlayer::Pause() { pState->Pause(*this); }
void MusicPlayer::Stop() { pState->Stop(*this); }

void MusicPlayer::SetState(State state)
{
	cout << "changing from " << pState->GetName() << " to ";

	if (state == State::ST_STOPPED)
	{
		pState.reset(new StoppedState());
	}
	else if (state == State::ST_PLAYING)
	{
		pState.reset(new PlayingState());
	}
	else
	{
		pState.reset(new PausedState());
	}

	cout << pState->GetName() << " state" << endl;
}

#pragma endregion

int main()
{
	MusicPlayer player(new StoppedState());

	player.Play();
	player.Pause();
	player.Stop();
}
