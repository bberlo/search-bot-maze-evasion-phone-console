#pragma once

#ifndef Arduino_hpp
#define Arduino_hpp

#include"ofSerial.h"

class Arduino {
	
	public:
		Arduino();
		
		void initializePortListener(ofSerial* serial);

		string concateReceivedInformation(ofSerial &serial, char until);

		std::vector<int> StringToData(string &concatedData);

};

#endif
