
#ifndef INPUT_LISTENER
#define INPUT_LISTENER

#include <cstdint>

using namespace std;

//enumerations for the input actions
enum KeyboardKey : uint32_t {KB_INVALID, KB_ESC, KB_W, KB_A, KB_S, KB_D};
enum MouseButton : uint32_t {M_INVALID, M_LEFT, M_RIGHT, M_MIDDLE, M_SCROLL_UP, M_SCROLL_DOWN};
enum JoystickAxis : uint32_t {JA_INVALID};
enum JoystickButton : uint32_t {JB_INVALID};

class InputListener {

public:

	virtual ~InputListener(){};

	virtual void keyPressed(const KeyboardKey key) = 0;
	virtual void keyReleased(const KeyboardKey key) = 0;

	virtual void mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy) = 0;
	virtual void mousePressed(const uint32_t x, const uint32_t y, const MouseButton button) = 0;
	virtual void mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button) = 0;

	virtual void joystickAxisMoved(const JoystickAxis axis, const uint32_t amount) = 0;
	virtual void joystickButtonPressed(const JoystickButton button) = 0;

};

#endif