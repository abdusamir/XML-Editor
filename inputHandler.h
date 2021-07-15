#ifndef _INPUTHANDLER_
#define _INPUTHANDLER_

struct outputXML	{
	std::string pretty{""};
	std::string mini{""};
};

class InputHandler	{
	public: 
		std::string inputXML{""};
		outputXML mainLogic(std::string);
};

#endif