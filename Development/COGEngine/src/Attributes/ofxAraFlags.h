#pragma once

#include "ofxAreMain.h"


/**
* State machine class that holds collection of states
*
*/
class ofxAraFlags{
private:
	// first flag bit array
	int flags1;
	// second flag bit array
	int flags2;
	// third flag bit array
	int flags3;
	// fourth flag bit array
	int flags4;
	// map for other flags (if used)
	map<int, int>* otherFlags;

	/**
	* Initializes state machine
	*/
	void Init(){
		flags1 = 0;
		flags2 = 0;
		flags3 = 0;
		flags4 = 0;
		otherFlags = nullptr;
	}

public:
	/**
	* Creates a new state machine
	*/
	ofxAraFlags();

	/**
	* Creates a new state machine, initialized with list of state
	*/
	ofxAraFlags(vector<int> states);

	/**
	* Creates a new state machine, initialized with one state
	*/
	ofxAraFlags(int state);

	/**
	* Creates a new state machine, initialized with two states
	*/
	ofxAraFlags(int state1, int state2);

	/**
	* Creates a new state machine, initialized with three states
	*/
	ofxAraFlags(int state1, int state2, int state3);

	/**
	* Creates a new state machine, initialized with four states
	*/
	ofxAraFlags(int state1, int state2, int state3, int state4);

	/**
	* Creates a new state machine, initialized with five states
	*/
	ofxAraFlags(int state1, int state2, int state3, int state4, int state5);

	ofxAraFlags(const ofxAraFlags& obj);

	~ofxAraFlags();

	/**
	* Gets list of all states (flags)
	*/
	vector<int> GetAllStates() const;

	/**
	* Returns true, if a state (flag) is set
	*/
	bool HasState(int state) const;

	/**
	* Sets the state
	* @param state state to set
	*/
	void SetState(int state){
		return DoStateOperation(true, state);
	}

	/**
	* Switches values of two states
	*/
	void SwitchState(int state1, int state2);

	/**
	* Resets the state
	* @param state state to reset
	*/
	void ResetState(int state){
		return DoStateOperation(false, state);
	}

	/**
	* Resets all states and sets one given state
	* @param st1 state to set
	*/
	ofxAraFlags& operator=(const int& st1);

	/**
	* Resets all states and sets all states from given EnFlag object
	* @param st1 EnFlag object to set 
	*/
	ofxAraFlags& operator=(const ofxAraFlags& st1);

	/**
	* Compares state machine with one state 
	*/
	bool operator==(int st1);

	/**
	* Compares state machine with another
	*/
	bool operator==(const ofxAraFlags& st1);

	/**
	* Compares state machine with one state
	*/
	bool operator!=(int st1);

	/**
	* Compares state machine with another
	*/
	bool operator!=(const ofxAraFlags& st1);

	/**
	* Adds a state to the state machine and returns a new object
	* @param st1 state to set
	*/
	ofxAraFlags operator+(int st1);

	/**
	* Removes a state from the state machine and returns a new object
	* @param st1 state to reset
	*/
	ofxAraFlags operator-(int st2);

	/**
	* Adds a state to the state machine
	* @param st1 state to set
	*/
	ofxAraFlags& operator+=(int st1);

	/**
	* Renoves a state from the state machine
	* @param st1 state to reset
	*/
	ofxAraFlags& operator-=(int st1);

	/**
	* Returns true, if all states from the given state machine are set in this state machine
	*/
	bool Contains(ofxAraFlags& other) const;

	/**
	* Returns true, if at least one state from the given state machine is set in this state machine
	*/
	bool ContainsAtLeastOne(ofxAraFlags& other) const;

	private:
		/**
		* Gets index of a state
		* @param state number of state
		*/
		int GetStateIndex(int state) const{
			return state / sizeof(int);
		}

		/**
		* Gets offset of a state
		* @param state number of state
		*/
		int GetStateOffset(int state) const{
			return state%sizeof(int);
		}

		/**
		* Does state operation (sets or resets the state)
		* @param set - if true, state will be set; reset otherwise
		* @param state - state to set/reset
		*/
		void DoStateOperation(bool set, int state);
};
