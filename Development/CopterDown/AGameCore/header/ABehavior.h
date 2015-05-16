
#ifndef ABEHAVIOR_H
#define ABEHAVIOR_H

#include "EnFlags.h"
#include "Enums.h"
#include "Msg.h"
#include <list>

class GNode;


/**
* ABehavior - common abstract class for all behaviors
*
*/
class ABehavior{

protected:
	// identifier incremental counter
	static int idCounter;
	// dummy response message
	static Msg _dummyMsg;
	// element type {ALL, VIEW, MODEL}
	const ElemType _elemType;
	// identifier
	const int _id;
	// behavior running state
	BehState _behState;
	// message acceptation mask
	EnFlags _msgFlags;
	// owner of this behavior
	GNode* _owner = nullptr;

	/**
	* Creates a new behavior
	* @param elemType element type {ALL, VIEW, MODEL}
	* @param msgFlags message acceptation mask
	*/
	ABehavior(ElemType elemType, EnFlags msgFlags);

	/**
	* Sends a message, expecting response
	* @param traverse state machine that indicates who should process this message
	* @param action id of action; see Actions namespace for common action ids
	* @param data payload
	* @param resp response that can be initialized only once
	*/
	void SendMessage(EnFlags traverse, int action, void* data, Msg& resp) const;

	/**
	* Sends a message without response
	* @param traverse state machine that indicates who should process this message
	* @param action id of action; see Actions namespace for common action ids
	* @param data payload
	*/
	void SendMessageNoResp(EnFlags traverse, int action, void* data) const;

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
	* Gets owner of this behavior
	* @return pointer to game object
	*/
	const GNode* GetOwnerNode() const{
		return _owner;
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
	* @param msg received message
	*/
	virtual void OnMessage(Msg& msg) const = 0;

	/**
	* Updates behavior inner state
	* @param delta delta time from the last loop
	* @param absolute absolute time since the game begun
	*/
	virtual void Update(const uint64 delta, const uint64 absolute) const = 0;

	bool operator==(int id){
		return this->_id == id;
	}

	bool operator==(const ABehavior& other){
		return _id == other._id;
	}

	bool operator!=(int id){
		return this->_id != id;
	}

	bool operator!=(const ABehavior& other){
		return _id != other._id;
	}

	// only game objects can access private members (especially the owner property)
	friend class GNode;
};


#endif