#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include <SDL.h>
#include <vector>
#include <map>
#include <string>
#include <set>

class InputConfig;
class Window;

//you should only ever instantiate one of these, by the way
class InputManager
{
private:
	InputManager();

	static InputManager* mInstance;

	static const int DEADZONE = 23000;

	void loadDefaultKBConfig();

	std::map<SDL_JoystickID, SDL_Joystick*> mJoysticks;
	std::map<SDL_JoystickID, InputConfig*> mInputConfigs;
	std::map<SDL_JoystickID, std::set<int>> mTriggerAxis; // set of axis IDs which are triggers actually
	InputConfig* mKeyboardInputConfig;

	std::map<SDL_JoystickID, int*> mPrevAxisValues;

	bool initialized() const;

	Sint16 mapTriggerRange(const SDL_JoyAxisEvent& jaxis);
	void detectTriggerAxis(int id);
	std::string triggerAxisToString(const std::set<int>& triggerAxis);

	void addJoystickByDeviceIndex(int id);
	void removeJoystickByJoystickID(SDL_JoystickID id);
	bool loadInputConfig(InputConfig* config); // returns true if successfully loaded, false if not (or didn't exist)
	void loadAdditionalGameControllerDb();

public:
	virtual ~InputManager();

	static InputManager* getInstance();

	void writeDeviceConfig(InputConfig* config);
	void doOnFinish();
	static std::string getConfigPath();
	static std::string getTemporaryConfigPath();
	static std::string getGameControllerDbPath();

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
