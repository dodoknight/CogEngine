#pragma once

#include "ofxAreMain.h"

/**
* Entity for sound attachment
*/
class ofxAraSound{
private:
	ofSoundPlayer soundPlayer;
public:
	string filename;
	bool started;
	float volume;

	ofxAraSound(string filename) : filename(filename){
		started = false;
		// music must be streamed; otherwise it won't work on android
		soundPlayer.loadSound(filename,true);
		SetVolume(1.0f);
	}

	bool Ended(){
		return started && !soundPlayer.getIsPlaying();
	}

	void Play(){
		soundPlayer.play();
		started = true;
	}

	void Stop(){
		soundPlayer.stop();
	}

	void SetLoop(bool loop){
		soundPlayer.setLoop(loop);
	}

	void SetVolume(float volume){
		this->volume = volume;
		soundPlayer.setVolume(volume);
	}

	float GetVolume(){
		return volume;
	}
};


