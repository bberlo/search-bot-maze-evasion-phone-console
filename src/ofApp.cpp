#include "ofApp.h"

//Global Object/variable declaration
ofSerial serial;
clock_t timer;
float timeInformation;
int interval = 500;

//--------------------------------------------------------------
void ofApp::setup(){
  
	//Different program inits which will be used in the draw function:
	
	timer = std::clock();
	
	ofBackground(200);
    
	arduino.initializePortListener(&serial);
	
    std::srand(std::time(0));
    
    grid.generateMaze();
	
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	//This timer is used in order to carry out a search bot move every 500ms
	if (std::clock() - timer  >= BOT_SPEED) {
		grid.greedyBot();
		grid.aStarBot();
		grid.breadthFirstBot();
		timer = std::clock();
		timeInformation += BOT_SPEED / 1000;
	}
	
	if (serial.available()) {
		string p = arduino.concateReceivedInformation(serial, '&');
		std::vector<int> values = arduino.StringToData(p);

		//Check to see if there are actual values inside the returned vector
		if (values.size() > 0) {
			
			//The cout below can be used in order to check all the values inside the sensorData vector:
			//std::cout << values.at(0) << "," << values.at(1) << "," << values.at(2) << "," << values.at(3) << "," << values.at(4) << "\n";

			//Switch to check from which sensor in the mobile phone the sensorData is coming from, 1 and 10 are predefined by the maker of the sensoDuino App: Hazim Bitar
			//1 stands for the normal accelerometer and 10 stands for the linear accelerometer
			//If sensorData has been detected from either source it will carry out a function which will either move the player or reshuffles the maze:
			switch (values.at(0)) {
				case 1:
					if (values.at(3) > 2) {
						grid.playerD();
					}
					else if (values.at(3) < -2) {
						grid.playerU();
					}
					else if (values.at(2) > 2) {
						grid.playerL();
					}
					else if (values.at(2) < -2) {
						grid.playerR();
					}
					break;
				case 10:
					if (values.at(4) > 9 || values.at(4) < -9) { 
						grid.reset();
						grid.generateMaze();
					}
					break;
			}
		}
	}

	//Draws the grid 
	grid.draw();

	//The following conditionals are used in order to check the end conditions for the game to do a complete reset
	if (grid.a == grid.breadthFirstxPos && grid.b == grid.breadthFirstyPos) {
		cout << "Breadth First Bot as Found you!" << "\n";
		grid.startAgain();
		std::cout << timeInformation << " Seconds survived" << endl;
		timeInformation = 0;
	}

	else if (grid.a == grid.greedyxPos && grid.b == grid.greedyyPos) {
		cout << "Greedy First Bot as Found you!" << "\n";
		grid.startAgain();
		std::cout << timeInformation << " Seconds survived" << endl;
		timeInformation = 0;
	}

	else if (grid.a == grid.aStarxPos && grid.b == grid.aStaryPos) {
		cout << "Astar First Bot as Found you!" << "\n";
		grid.startAgain();
		std::cout << timeInformation << " Seconds survived" << endl;
		timeInformation = 0;
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
	//The following statements are used for debugging purposes to check what happens at certain types of mazes, bots and player moves etc.
	switch(key) {
        case 'm':
            grid.reset();
            grid.generateMaze();
            break;
        case 'r':
            grid.reset();
            grid.generateRooms();
            break;
        case 'o':
            grid.reset();
            grid.generateObstacles();
            break;
        case 'b':
            grid.breadthFirstBot();
            break;
        case 'g':
            grid.greedyBot();
            break;
        case 'a':
            grid.aStarBot();
            break;
		case 'i':
			grid.playerU();
			break;
		case 'j':
			grid.playerL();
			break;
		case 'k':
			grid.playerD();
			break;
		case 'l':
			grid.playerR();
			break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
