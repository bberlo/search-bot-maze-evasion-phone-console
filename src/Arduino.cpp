#include "Arduino.h"

Arduino::Arduino() {

}

/*

	The functions inside the 'Arduino' class have been constructed with the help of the following stackOverflow post:
	http://stackoverflow.com/questions/29861393/openframeworks-reading-serial-data-from-arduino

	The function requires an initialized serial object when being called.
	After being called, the function will get a vector with all open ports.
	Then, a certain port is chosen from the vector and setup with a baud rate of 9600 Bd.

*/

void Arduino::initializePortListener(ofSerial* serial) {
	
	serial->listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial->getDeviceList();
	if (deviceList.size() > 0) {
		serial->setup("COM5", 9600); //open the first device and talk to it at 9600 baud
	}
	else {
		std::cout << "No active com ports have been found";
	}

}

/*

	The 'concateReceivedInformation' function has been created with the help of this source, updated and used in order to turn the read sensor information into a string:
	https://sites.google.com/site/ofauckland/examples/serial-read-string-from-arduino

	The function requires an initialized and setup serial object and a delimiter that determines if the total amount of chars for the complete string with sensor data has been received.
	When the function is called it will use a while loop in order to check if the delimiter has been reached and that characters are still being sent.
	As long as the while loop is running the incoming chars are added to a stringstream.
	If the delimiter has been reached the total content of the stringstream is cast to a string and returned, after that it clears the string connected to the string stream, else it returns an empty string.

*/

string Arduino::concateReceivedInformation(ofSerial &serial, char until) {

	static string str;
	stringstream ss;
	char serialDataSeperateChars;

	while ( (serialDataSeperateChars = serial.readByte()) > 0 && serialDataSeperateChars != until) {
		ss << serialDataSeperateChars;
	}
	str += ss.str();

	if (serialDataSeperateChars == until) {
		string tmp = str;
		str = "";
		return tmp;
	}
	else {
		return "";
	}

}

/*

	The 'StringToData' function has been made with the help of the following source:
	http://stackoverflow.com/questions/1321137/convert-string-containing-several-numbers-into-integers

	The function requires a string with sensorData in it.
	When the function is being called the string is casted to a stringstream
	While the stringstream is being able to cast integer values to an empty int these int values will be pushed inside a vector.
	After that the vector is checked for values that are bigger than 10 or smaller than -10, these values are then floored and ceiled to 10 and -10.
	Last but not least the function returns a vector with the sensor data which can be used inside the rest of the program.

*/

std::vector<int> Arduino::StringToData(string &concatedData) {

	std::vector<int> sensorData;
	stringstream concatedDataAnalyzer(concatedData);
	int n;

	while (concatedDataAnalyzer >> n) {
		sensorData.push_back(n);
	}

	if (sensorData.size() > 0) {
		for (int i = 0; i < sensorData.size(); i++) {
			if (sensorData.at(i) < -10) {
				sensorData.at(i) = -10;
			}
			else if (sensorData.at(i) > 10) {
				sensorData.at(i) = 10;
			}
		}
	}
	
	return sensorData;
}