
#ifndef INPUT_LISTENER
#define INPUT_LISTENER

class InputListener {

public:

	virtual ~InputListener(){};

	virtual void keyPressed() = 0;
	virtual void keyReleased() = 0;

	virtual void mouseMoved() = 0;
	virtual void mousePressed() = 0;
	virtual void mouseReleased() = 0;

	virtual void joystickAxisMoved() = 0;
	virtual void joystickButtonPressed() = 0;

};

#endif