#ifndef GBEHAVIOR_H
#define GBEHAVIOR_H

#include "EnFlags.h"
#include "Enums.h"
#include "GMsg.h"
#include <list>

class GNode;


/**
* GBehavior - common abstract class for all behaviors
*
*/
class GBehavior{

protected:
	// identifier incremental counter
	static int idCounter;
	// dummy response message
	static GMsg _dummyMsg;
	// element type {ALL, VIEW, MODEL}
	const ElemType _elemType;
	// identifier
	const int _id;
	// behavior running state
	BehState _behState;
	// message acceptation mask
	EnFlags _msgFlags;

	virtual ~GBehavior()
	{

	}

	/**
	* Creates a new behavior
	* @param elemType element type {ALL, VIEW, MODEL}
	* @param msgFlags message acceptation mask
	*/
	GBehavior(ElemType elemType, EnFlags msgFlags);

	/**
	* Sends a message, expecting response
	* @param traverse state machine that indicates who should process this message
	* @param action id of action; see Actions namespace for common action ids
	* @param data payload
	* @param resp response that can be initialized only once
	* @param target target game object
	*/
	void SendMessage(EnFlags traverse, int action, void* data, GMsg& resp, GNode* target) const;

	/**
	* Sends a message without response
	* @param traverse state machine that indicates who should process this message
	* @param action id of action; see Actions namespace for common action ids
	* @param data payload
	* @param target target game object
	*/
	void SendMessageNoResp(EnFlags traverse, int action, void* data, GNode* target) const;

public:

	/**
	* Gets element type
	* @return {ALL, VIEW, MODEL}
	*/
	const ElemType GetElemType() const{
		return _elemType;
	}

	/**
	* Gets element identifier
	* @return incremental value
	*/
	const int GetId() const{
		return _id;
	}

	/**
	* Gets behavior running state
	* @return running state
	*/
	const BehState GetBehState() const{
		return _behState;
	}

	/**
	* Gets message flags
	* @return reference to flag object
	*/
	const EnFlags& GetMessageFlags() const{
		return _msgFlags;
	}

	/**
	* Sets behavior state
	* @param val value
	*/
	void SetBehState(BehState val){
		_behState = val;
	}

	/**
	* Receives a message
	* @param GMsg received message
	*/
	virtual void OnMessage(GMsg& msg)  = 0;

	/**
	* Updates behavior inner state
	* @param delta delta time from the last loop
	* @param absolute absolute time since the game begun
	* @param absMatrix current absolute matrix, traversed from parent
	*/
	virtual void Update(const uint64 delta, const uint64 absolute, const ofMatrix4x4& absMatrix, GNode* owner) = 0;

	bool operator==(int id){
		return this->_id == id;
	}

	bool operator==(const GBehavior& other){
		return _id == other._id;
	}

	bool operator!=(int id){
		return this->_id != id;
	}

	bool operator!=(const GBehavior& other){
		return _id != other._id;
	}

	// only game objects can access private members (especially the owner property)
	friend class GNode;
};


#endif