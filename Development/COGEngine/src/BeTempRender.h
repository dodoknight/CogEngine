#ifndef BETEMPRENDER_H
#define BETEMPRENDER_H

#include "GBehavior.h"
#include "GNode.h"
#include "SmartPointer.h"
#include "EnPolygon.h"
#include "BeRender.h"


/**
* Behavior for temporary render
*/
class BeTempRender : public GBehavior{
private:
	spt<ofImage> image;
	int framesNum;
	int actualFrame;

public:
	BeTempRender(spt<ofImage> image, int framesNum) : GBehavior(ElemType::VIEW, EnFlags()){
		this->image = image;
		this->framesNum = framesNum;
		this->actualFrame = 0;
	}

	virtual void OnMessage(GMsg& msg){

	}


	virtual void Update(const uint64 delta, const uint64 absolute, GNode* owner){

		if (actualFrame++ > framesNum){
			owner->RemoveBehavior(this);
			return;
		}

		ofMatrix4x4 absM = owner->GetTransform().GetAbsMatrix();
		ofLoadMatrix(absM);
		ofSetColor(0x000000ff);
		image->draw(-image->getWidth() / 2, -image->getHeight() / 2);

	}

};


#endif
