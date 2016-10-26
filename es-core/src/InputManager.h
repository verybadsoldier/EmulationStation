#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include <SDL.h>
#include <vector>
#include <map>
#include <string>

class InputConfig;
class Window;

//you should only ever instantiate one of these, by the way
class InputManager
{
private:
	class AbstractJoystick
	{
	public:
		enum class JoyType
		{
			Undefined,
			Joystick,
			GameController,
		};

		void open(int id)
		{
			if (SDL_IsGameController(id))
			{
				// SDL2 gamecontroller support
				m_gctrl = SDL_GameControllerOpen(id);
				assert(m_gctrl);
				m_joy = SDL_GameControllerGetJoystick(gctrl);

				m_type = JoyType.GameController;
			}
			else
			{
				// joystick (legacy)
				m_joy = SDL_JoystickOpen(id);
				m_type = JoyType.Joystick;
			}
			assert(m_joy);
		}

		void close()
		{
			switch(m_type)
			{
			case Joystick:
				assert(m_joy);
				return SDL_JoystickClose(m_joy);
			case GameController:
				assert(m_gctrl);
				return SDL_GameControllerClose(m_gctrl);
			}
		}

		SDL_Joystick* getJoystick()
		{
			return m_joy;
		}

		SDL_GameController* getGameController()
		{
			return m_gctrl;
		}

		JoyType getType() const
		{
			return m_type;
		}

		const char* getName() const
		{
			switch(m_type)
			{
			case Joystick:
				assert(m_joy);
				return SDL_GameControllerName(m_joy);
			case GameController:
				assert(m_gctrl);
				return SDL_GameControllerName(m_gctrl);
			}
		}
	protected:
		JoyType				m_type = JoyType.Undefined;

		SDL_Joystick* 		m_joy = nullptr;
		SDL_GameController* m_gctrl = nullptr;
	};

	InputManager();

	static InputManager* mInstance;

	static const int DEADZONE = 23000;

	void loadDefaultKBConfig();

	std::map<SDL_JoystickID, AbstractJoystick> mJoysticks;
	std::map<SDL_JoystickID, InputConfig*> mInputConfigs;
	InputConfig* mKeyboardInputConfig;

	std::map<SDL_JoystickID, int*> mPrevAxisValues;

	bool initialized() const;

	void addJoystickByDeviceIndex(int id);
	void removeJoystickByJoystickID(SDL_JoystickID id);
	bool loadInputConfig(InputConfig* config); // returns true if successfully loaded, false if not (or didn't exist)

public:
	virtual ~InputManager();

	static InputManager* getInstance();

	void writeDeviceConfig(InputConfig* config);
	void doOnFinish();
	static std::string getConfigPath();
	static std::string getTemporaryConfigPath();

	void init();
	void deinit();

	int getNumJoysticks();
	int getButtonCountByDevice(int deviceId);
	int getNumConfiguredDevices();

	std::string getDeviceGUIDString(int deviceId);

	InputConfig* getInputConfigByDevice(int deviceId);

	bool parseEvent(const SDL_Event& ev, Window* window);
};

#endif
