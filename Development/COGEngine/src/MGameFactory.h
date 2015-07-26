#pragma once
#include "GNode.h"
#include "BeSceneManager.h"

/*! Calculation type */
enum class CalcType{
	ABS,	/*!< absolute */
	LOC,	/*!< local */
	PER 	/*!< percentage <0..1> */
};

/**
* Game factory with methods for calculating positions
* All game object construction methods should be implemented
* in classes derived from this one
*/
class MGameFactory{

public:

	// scene manager, assigned to the root object
	BeSceneManager* sceneManager;

	/**
	* Gets position of screen center
	*/
	ofVec2f GetCenter();

	/**
	* Loads render image and loads it to the game object with calculated transformation
	* @param node node to set
	* @param imgPath path to the image
	* @param pos position, calculated due to posCalc parameter
	* @param posCalc position calculation type (absolute, percentage or local)
	* @param scaleX scale in x axis
	* @param scaleCalc scale calculation type (absolute, percentage or local)
	* @param anchor percentage object anchor
	* @param parent link to the object parent
	*/
	void SetRenderImage(GNode* node, string imgPath, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, GNode* parent);

	/**
	* Sets absolute transform to node
	* @param node node to set
	* @param pos position, calculated due to posCalc parameter
	* @param posCalc position calculation type (absolute, percentage or local)
	* @param scaleX scale in x axis
	* @param scaleCalc scale calculation type (absolute, percentage or local)
	* @param anchor percentage object anchor
	* @param width object width
	* @param height object height
	* @param parent link to the object parent
	*/
	void SetTransform(GNode* node, ofVec2f pos, CalcType posCalc, float scaleX, CalcType scaleCalc, ofVec2f anchor, int width, int height, GNode* parent);

	/**
	* Calculates position
	* @param node node to set
	* @param pos position, calculated due to posCalc parameter
	* @param posCalc position calculation type (absolute, percentage or local)
	* @param anchor percentage object anchor
	* @param parent link to the object parent
	*/
	ofVec2f CalcPosition(GNode* node, ofVec2f pos, CalcType posCalc, ofVec2f anchor, GNode* parent);

	/**
	* Calculates scale
	* @param node node to set
	* @param scaleX scale in x axis
	* @param width object width
	* @param scaleCalc scale calculation type (absolute, percentage or local)
	* @param parent link to the object parent
	*/
	ofVec3f CalcScale(GNode* node, float scaleX, float width, CalcType scaleCalc, GNode* parent);

	/**
	* Gets translating speed according to the actual screen width
	* @return speed in X pixels per second, where X is screen width
	*/
	float TranslateSpeed(float speed);

	/**
	* Initializes game factory
	*/
	virtual void Init(){

	}
	
	/**
	* Creates game root with first scene
	*/
	virtual GNode* CreateRoot();


};

