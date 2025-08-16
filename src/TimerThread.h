#ifndef TimerThread_hpp
#define TimerThread_hpp


#include "ofApp.h"
#include "ofThread.h"

class TimerThread : public ofThread {
	
	// the thread function
	void TimerThread::threadedFunction() {

		// start
		unsigned int currentTime =0;
		while (isThreadRunning()) {

			clock_t time = std::clock();
		
			if (time - currentTime >= 100) {
				cout << currentTime << endl;
				currentTime = time;
			}
		}
	}

};

#endif
