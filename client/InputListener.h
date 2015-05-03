
#ifndef INPUT_LISTENER
#define INPUT_LISTENER

#include <cstdint>

using namespace std;

//enumerations for the input actions
enum KeyboardKey : uint32_t {KB_INVALID, KB_TAB, KB_ESC, KB_SPACE, KB_LSHIFT, KB_W, KB_A, KB_S, KB_D, 
                              KB_Q, KB_Z, KB_E, KB_C, KB_F, KB_UP, KB_DOWN, KB_LEFT, KB_RIGHT, KB_RETURN };

enum MouseButton : uint32_t {M_INVALID, M_LEFT = 0, M_RIGHT = 1, M_MIDDLE = 2, M_BACK = 3, M_SCROLL = 4 };
enum JoystickButton : uint32_t {JB_INVALID, JB_1, JB_2, JB_3, JB_4, JB_5, JB_6, JB_7, JB_8};

//this is a pure virtual class that other classes can inherit from in order to recieve input from
//the input manager. All methods have no implementation
class InputListener {

public:

	virtual ~InputListener(){};

	virtual void keyPressed(const KeyboardKey key) = 0;
	virtual void keyReleased(const KeyboardKey key) = 0;

	virtual void mouseMoved(const uint32_t x, const uint32_t y, const int32_t dx, const int32_t dy) = 0;
	virtual void mousePressed(const uint32_t x, const uint32_t y, const MouseButton button) = 0;
	virtual void mouseReleased(const uint32_t x, const uint32_t y, const MouseButton button) = 0;

	virtual void joystickAxisMoved(const int *axes, const int numAxes) = 0;
	virtual void joystickButtonPressed(const JoystickButton button) = 0;

};

#endif