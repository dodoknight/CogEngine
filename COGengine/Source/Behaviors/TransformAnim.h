#pragma once

#include "Behavior.h"
#include "Node.h"



/**x
* Behavior for common transformation animation
*/
class TransformAnim : public Behavior{

private:
	// duration in ms
	float duration;
	// starting transform
	Transform to;
	// ending transform
	Transform from;
	// actual duration
	float actual;
	// fade function
	FadeFunction fadeFunction;
	float delayAfter;

public:

	TransformAnim(Transform from, Transform to, float duration, float delayAfter) : to(to), from(from), duration(duration), 
		delayAfter(delayAfter){
		this->actual = 0;
		fadeFunction = nullptr;
	}

	TransformAnim(Transform from, Transform to, float duration, float delayAfter, FadeFunction fadeFunction) : 
		to(to), from(from), duration(duration), delayAfter(delayAfter){
		this->actual = 0;
		this->fadeFunction = fadeFunction;
	}

	void Update(const uint64 delta, const uint64 absolute){
		
		actual += delta;

		float actualCropped = actual;

		if (actual >= duration){
			actualCropped = duration;
		}

		float actualPercent = actualCropped / duration;
		if (fadeFunction != nullptr) actualPercent = fadeFunction(actualPercent);

		Transform actualTrans(0, 0);
		actualTrans.localPos = from.localPos + (to.localPos - from.localPos)*actualPercent;
		actualTrans.rotation = from.rotation + (to.rotation - from.rotation)*actualPercent;
		actualTrans.scale = from.scale + (to.scale - from.scale)*actualPercent;
		actualTrans.CalcAbsTransform(owner->GetParent()->GetTransform());
		owner->SetTransform(actualTrans);

		if (actual >= (duration + delayAfter)) Finish();
	}

};
