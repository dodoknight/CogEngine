#pragma once

#include "ofxAreMain.h"
#include "ofxAreAttr.h"
#include "ofxAreBehavior.h"
#include "ofxAreMsg.h"
#include "ofxAraTransform.h"
#include "ofxAreUtils.h"
#include "ofxAreFacade.h"
#include "ofxAraShape.h"


/*! Node running mode */
enum RunningMode{
	RUNNING,			/*!< active for update and draw */
	PAUSED_ALL,			/*!< active for draw */
	PAUSED_CHILDREN,	/*!< active itself but children are inactive */
	PAUSED_ITSELF,		/*!< inactive but children are active */
	INVISIBLE,			/*!< active for update but inactive for draw */
	DISABLED			/*!< inactive for both, update and draw */
};

/**
* Node with attributes, behaviors, states and transformation matrix
*
*/
class ofxAreNode{
protected:
	// identifier incremental counter
	static int idCounter;
	// attributes, mapped by their keys
	map<int, ofxAreAttr*> attributes;
	// list of childrens
	list<ofxAreNode*> children;
	// list of behaviors
	list<ofxAreBehavior*> behaviors;
	// link to the parent
	ofxAreNode* parent;
	// list of children that will be removed at the end of update method
	list<std::pair<ofxAreNode*,bool>> childrenToRemove;
	// list of behaviors that will be removed at the end of update method
	list<std::pair<ofxAreBehavior*,bool>> behaviorToRemove;
	// list of children that will be added at the end of update method
	list<ofxAreNode*> childrenToAdd;
	// list of behaviors that will be added at the end of update method
	list<ofxAreBehavior*> behaviorsToAdd;

	// id of this node
	const int id;
	// tag or name
	string* tag;
	// object type {ROOT, SCENE, OBJECT, HUD, INFO}
	ObjType type;
	// subtype (or category)
	int subType;
	// groups this objects belongs to
	ofxAraFlags* groups ;
	// states of this object
	ofxAraFlags* states;
	// transformation matrix
	ofxAraTransform  transform;
	// mash object
	spt<ofxAraShape> shape;
	// running mode
	RunningMode runMode;

	// initialization method
	void Construct(){
		parent = nullptr;
		tag = nullptr;
		groups = nullptr;
		states = nullptr;
		tag = nullptr;
		runMode = RunningMode::RUNNING;
		shape = spt<ofxAraShape>(new ofxAraShape(RenderType::NONE));
	}

	
public:

	/**
	* Creates a new node
	* @param tag tag/name
	*/
	ofxAreNode(string tag);

	/**
	* Creates a new node
	* @param type object type {ROOT, SCENE, OBJECT, HUD, INFO}
	* @param subType subtype/category number
	* @param tag tag/name 
	*/
	ofxAreNode(ObjType type, int subType, string tag);

	ofxAreNode(const ofxAreNode& copy);

	~ofxAreNode();

	/**
	* Updates absolute transformations according to the parent
	* @param depp if true, children will be updated as well
	*/
	void UpdateTransform(bool deep);

	/**
	* Updates all behaviors in the subtree
	* @param delta delta time from the last loop
	* @param absolute absolute time since the application started
	*/
	void Update(const uint64 delta, const uint64 absolute);

	/**
	* Draws all objects, calling VIEW behaviors
	* @param delta delta time from the last loop
	* @param absolute absolute time since the app begun
	*/
	void Draw(const uint64 delta, const uint64 absolute);

	/**
	* Adds a new behavior
	* @param beh behavior to add
	* @return true, if behavior has been successfully added
	*/
	bool AddBehavior(ofxAreBehavior* beh);

	/**
	* Removes existing behavior (by its id)
	* @param beh behavior to remove
	* @param erase if true, memory will be released
	* @return true, if behavior has been removed
	*/
	bool RemoveBehavior(ofxAreBehavior* beh, bool erase);

	/**
	* Removes existing attribute (by its key)
	* @param key key of attribute that will be removed
	* @param erase if true, memory will be released
	* @return true, if attribute has been removed
	*/
	bool RemoveAttr(int key, bool erase);

	/**
	* Returns true, if this node has an attribute with given key
	*/
	bool HasAttr(int key) const;

	/**
	* Updates collections of behaviors and children nodes
	* Adds elements that are supposed to be added and removes
	* elements that are supposed to be removed
	* @param applyToChildren if true, children will be updated as well
	*/
	void SubmitChanges(bool applyToChildren);

	/**
	* Gets copy of list of all behaviors
	*/
	list<ofxAreBehavior*> GetBehaviorsCopy() const{
		return behaviors;
	}

	/**
	* Gets direct reference to list of all behaviors
	*/
	const list<ofxAreBehavior*>& GetBehaviors() const{
		return behaviors;
	}

	/**
	* Gets copy of list of all children
	*/
	list<ofxAreNode*> GetChildrenCopy() const{
		return children;
	}

	/**
	* Gets direct reference to list of all children
	*/
	const list<ofxAreNode*>& GetChildren() const{
		return children;
	}

	/**
	* Adds a new child
	* @param child child to add
	* @return true, if child has been successfully added
	*/
	bool AddChild(ofxAreNode* child);

	/**
	* Removes an existing child
	* @param child child to remove
	* @param erase if true, memory will be released
	* @return true, if child has been removed
	*/
	bool RemoveChild(ofxAreNode* child, bool erase);

	/**
	* Removes this node from its parent (if it exists)
	* @param erase if true, memory will be released
	* @return true, if this object has been removed
	*/
	bool RemoveFromParent(bool erase);

	/**
	* Gets pointer to the parent of this node
	*/
	ofxAreNode* GetParent(){
		return parent;
	}

	/**
	* Sets parent of this node
	*/
	void SetParent(ofxAreNode* val){
		parent = val;
	}

	/**
	* Finds the first predecessor of given object type
	* @param type predecessor type {ROOT, SCENE, OBJECT, HUD, INFO}
	*/
	ofxAreNode* FindPredecessor(ObjType type);

	/**
	* Gets the nearest parent that is a scene root (if exists)
	*/
	ofxAreNode* GetSceneRoot(){
		if (type == ObjType::SCENE) return this;
		else return FindPredecessor(ObjType::SCENE);
	}

	/**
	* Gets the root of the whole scene
	*/
	ofxAreNode* GetRoot(){
		if (type == ObjType::ROOT) return this;
		else return FindPredecessor(ObjType::ROOT);
	}

	/**
	* Gets unique identifier of this node
	*/
	int GetId() const{
		return id;
	}
	
	/**
	* Gets tag/name
	*/
	string GetTag() const{
		if (tag == nullptr) return "";
		else return string(*tag);
	}

	/**
	* Sets tag/name
	*/
	void SetTag(string tag){
		delete this->tag;
		this->tag = new string(tag);
	}

	/**
	* Gets type/category of this node {ROOT, SCENE, OBJECT, HUD, INFO}
	*/
	ObjType GetType() const{
		return this->type;
	}
	
	/**
	* Gets subtype/category
	*/
	int GetSubType() const{
		return subType;
	}

	/**
	* Sets subtype/category
	*/
	void SetSubType(int val){
		this->subType = val;
	}

	/**
	* Gets transformation entity
	*/
	ofxAraTransform& GetTransform(){
		return transform;
	}

	/**
	* Sets transformation entity
	*/
	void SetTransform(ofxAraTransform val){
		this->transform = val;
	}

	/**
	* Gets indicator, if this entity is renderable
	*/
	bool IsRenderable(){
		return shape->GetRenderType() != RenderType::NONE;
	}

	/**
	* Gets shaping object
	*/
	spt<ofxAraShape> GetShape(){
		return shape;
	}

	/**
	* Get shape of selected template; must inherit from ofxAraShape entity
	*/
	template<class T> T GetShape(){
		auto specialShape = static_cast<T>(shape);
		return specialShape;
	}

	/**
	*  Sets shaping object
	*/
	void SetShape(spt<ofxAraShape> shape){
		this->shape = shape;
	}

	/**
	* Returns true, if the shape is of the selected type
	*/
	bool HasRenderType(RenderType type){
		return shape->GetRenderType() == type;
	}

	/**
	* Returns true, if groups has been initialized
	* Check this indicator first before asking for groups, because
	* they are lazy initialized
	*/
	const bool HasGroups() const{
		return groups != nullptr;
	}

	/**
	* Gets flag array, representing list of all groups this object belongs to
	*/
	ofxAraFlags& GetGroups(){
		if (groups == nullptr) groups = new ofxAraFlags();
		return *groups;
	}

	/**
	* Sets groups this objects belongs to
	*/
	void SetGroups(ofxAraFlags val){
		delete groups;
		this->groups = new ofxAraFlags(val);
	}

	/**
	* Returns true, if this object is in selected group
	*/
	bool IsInGroup(int groupId){
		return groups->HasState(groupId);
	}

	/**
	* Sets selected group
	*/
	void SetGroup(int groupId){
		if (groups == nullptr) groups = new ofxAraFlags();
		groups->SetState(groupId);
	}

	/**
	* Resets selected group
	*/
	void UnsetGroup(int groupId){
		if (groups == nullptr) groups = new ofxAraFlags();
		groups->ResetState(groupId);
	}

	/**
	* Returns true, if states has been initialized
	* Check this indicator first before asking for states, because
	* they are lazy initialized
	*/
	const bool HasStates() const{
		return states != nullptr;
	}

	/**
	* Gets flag array, representing all states this object has set
	*/
	ofxAraFlags& GetStates(){
		if (states == nullptr) states = new ofxAraFlags();
		return *states;
	}

	/**
	* Sets states of this node
	*/
	void SetStates(ofxAraFlags val){
		delete states;
		this->states = new ofxAraFlags(val);
	}

	/**
	* Returns true, if this object has selected state
	*/
	bool HasState(int state){
		if (!HasStates()) return false;
		return states->HasState(state);
	}

	/**
	* Sets new state
	*/
	void SetState(int state){
		GetStates().SetState(state);
	}

	/**
	* Resets selected state
	*/
	void ResetState(int state){
		GetStates().ResetState(state);
	}

	/**
	* Switches values of two states
	*/
	void SwitchState(int state1, int state2){
		GetStates().SwitchState(state1, state2);
	}

	/**
	* Gets running mode
	*/
	RunningMode GetRunningMode(){
		return this->runMode;
	}

	/**
	* Sets running mode
	*/
	void SetRunningMode(RunningMode runMode){
		this->runMode = runMode;
	}

	/**
	* Adds a new attribute; or replace already existing attribute
	* @param key key of the attribute
	* @param value reference
	*/
	template<class T> void AddAttr(int key, T value){
		if (HasAttr(key)){
			RemoveAttr(key,true);
		}

		attributes[key] = new ofxAreAttrR<T>(key, value, this);
	}
	
	/**
	* Gets an attribute by key; call this method only if you are sure that the attribute exists
	* @param key attribute key
	*/
	template<class T> T& GetAttr(int key){
		auto it = attributes.find(key);

		MASSERT(it != attributes.end(), "GNODE", "%s: Attribute %d doesn't exists", tag->c_str(), key);
		MASSERT(typeid(*it->second) == typeid(ofxAreAttrR<T>), "GNODE", "%s: Attribute %d is of the wrong type!", tag->c_str(), key);

		ofxAreAttrR<T>* attr = static_cast<ofxAreAttrR<T>*>(it->second);
		return attr->GetValue();
	}

	/**
	* Changes value of selected attribute or adds a new attribute if it doesn't exist
	* @param key attribute key
	* @param value new value
	*/
	template<class T> void ChangeAttr(int key, T value){
		auto it = attributes.find(key);
		if (it != attributes.end()){
			ofxAreAttrR<T>* attr = static_cast<ofxAreAttrR<T>*>(it->second);
			attr->SetValue(value);
		}
		else{
			AddAttr(key, value);
		}
	}

	bool operator==(int id){
		return this->id == id;
	}

	bool operator==(const ofxAreNode& other){
		return id == other.id;
	}

	bool operator!=(int id){
		return this->id != id;
	}

	bool operator!=(const ofxAreNode& other){
		return id != other.id;
	}

	/**
	* Writes information to the console and returns it as a string
	* @param includeChildren if true, children will be included in info report
	* @param includeAttribute if true, attributes will be included in info report
	*/
	string GetInfo(bool includeChildren, bool includeAttributes);

	/**
	* Writes information to the stringstream 
	* @param includeChildren if true, children will be included in info report
	* @param includeAttribute if true, attributes will be included in info report
	* @param ss stringstream to write into
	* @param level actual space padding
	*/
	void GetInfo(bool includeChildren, bool includeAttributes, std::ostringstream& ss, int level);

	// app storage can access private members
	friend class ofxArcRepository;

	protected:
		/*
		* Inserts children and behaviors that should be inserted
		* at the end of update loop
		*/
		void InsertElementsForAdding();

		/*
		* Deletes children and behaviors that should be removed
		* at the end of update loop
		*/
		void DeleteElementsForRemoving();
};