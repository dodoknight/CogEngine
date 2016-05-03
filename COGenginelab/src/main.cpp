#include "ofxCogMain.h"
#include "BehaviorEnt.h"
#include "NodeBuilder.h"
#include "ofxTextLabel.h"
#include "NetworkManager.h"
#include "Mesh.h"
#include "NetworkCommunicator.h"
#include "NetMessage.h"
#include "Interpolator.h"
#include "AttribAnimator.h"
#include "UpdateMessage.h"
#include "Move.h"
#include "Movement.h"
#include "SteeringBehavior.h"
#include "SpriteSheet.h"

enum class NetworkType { NONE, CLIENT, SERVER };

class NetworkBehavior : public Behavior {
private:
	NetworkCommunicator* communicator;
	Interpolator* deltaUpdate;
	NetworkType netType = NetworkType::NONE;
	int unitsNum = 0;
	Node* viewNode = nullptr;
	int frame = 0;
	int period = 3;
	map<int, float> previous;
	vector<Node*> unitModels;
public:
	NetworkBehavior() {

	}

	void OnInit() {
		SubscribeForMessages(ACT_NET_MESSAGE_RECEIVED, ACT_BUTTON_CLICKED);
		communicator = new NetworkCommunicator();
		REGISTER_COMPONENT(communicator);
		unitsNum = GETCOMPONENT(ResourceCache)->GetProjectSettings().GetSettingValInt("scene_settings", "units");
	}

	void LoadUnits() {
		// load view
		viewNode = owner->GetScene()->FindNodeByTag("unit_view");

		// load models
		for (auto child : owner->GetSceneRoot()->GetChildren()) {
			if (child->GetTag().compare("unit_model") == 0) {
				unitModels.push_back(child);
			}
		}
	}

	void InitNetwork(NetworkType netType) {
		this->netType = netType;

		if (netType == NetworkType::CLIENT) {
			deltaUpdate = GETCOMPONENT(Interpolator);
		}

		if (netType == NetworkType::SERVER) {
			communicator->InitListening(1234, 11987);
		}
		else {
			communicator->InitBroadcast(1234, 11986, 11987);
			communicator->SetAutoConnect(true);
		}

		if (netType == NetworkType::SERVER) {
			//  insert movement behaviors to the model nodes
			for (auto unitModel : unitModels) {
				auto movement = Movement();
				unitModel->AddAttr(ATTR_MOVEMENT, movement);
				unitModel->AddBehavior(new Move(true));
				// steering behavior for random walk
				unitModel->AddBehavior(new WanderBehavior(300 * (ofRandomf() + 1), 50 * (ofRandomf() + 1), 1000000));
			}
		}
	}

	void OnMessage(Msg& msg) {
		if (msg.HasAction(ACT_BUTTON_CLICKED)) {
			if (msg.GetContextNode()->GetTag().compare("server_but") == 0) {
				// click on server button
				InitNetwork(NetworkType::SERVER);
			}
			else {
				// click on client button
				InitNetwork(NetworkType::CLIENT);
			}

			// disable buttons
			this->owner->GetScene()->FindNodeByTag("server_but")->SetState(StrId(STATES_DISABLED));
			this->owner->GetScene()->FindNodeByTag("client_but")->SetState(StrId(STATES_DISABLED));
		}

		if (msg.HasAction(ACT_NET_MESSAGE_RECEIVED) && netType == NetworkType::CLIENT) {
			// push received message to Interpolator
			auto msgEvent = msg.GetData<NetworkMsgEvent>();
			auto netMsg = msgEvent->msg;
			if (netMsg->GetAction() == NET_MSG_UPDATE) {

				spt<UpdateMessage> updateMsg = netMsg->GetData<UpdateMessage>();
				spt<UpdateInfo> deltaInfo = spt<UpdateInfo>(new UpdateInfo(netMsg->GetMsgTime(), updateMsg->GetContinuousValues(), updateMsg->GetDiscreteValues()));
				deltaUpdate->AcceptUpdateMessage(deltaInfo);
			}
		}
	}

	virtual void Update(const uint64 delta, const uint64 absolute) {
		if (unitModels.empty()) LoadUnits();

		if (netType == NetworkType::CLIENT) {
			// copy actual interpolated values from interpolator to model nodes
			auto delta = this->deltaUpdate->GetActualUpdate();
			for (int i = 0; i < unitsNum; i++) {
				auto unit = unitModels[i];

				unit->GetTransform().rotation = delta->GetVal(StrId(i * 3 + 0));
				unit->GetTransform().localPos.x = delta->GetVal(StrId(i * 3 + 1));
				unit->GetTransform().localPos.y = delta->GetVal(StrId(i * 3 + 2));
			}
		}
		else if (netType == NetworkType::SERVER) {
			if (frame++ % period == 0) {
				// send values to the client
				auto updateInfo = spt<UpdateInfo>(new UpdateInfo());
				auto& deltas = updateInfo->GetContinuousValues();

				for (int i = 0; i < unitsNum; i++) {
					auto unit = unitModels[i];

					StrId rotId = StrId(i * 3 + 0);
					StrId posXId = StrId(i * 3 + 1);
					StrId posYId = StrId(i * 3 + 2);

					deltas[rotId] = unit->GetTransform().rotation;

					if (!previous.empty()) {
						// check teleported units (when units reaches the corner of the window, it is teleported to the opposite corner)
						if (previous.find(posXId) != previous.end() && previous.find(posYId) != previous.end()) {
							float previousPosX = previous.find(posXId)->second;
							float previousPosY = previous.find(posYId)->second;
							float actualPosX = unit->GetTransform().localPos.x;
							float actualPosY = unit->GetTransform().localPos.y;

							if (abs(actualPosY - previousPosY) > 100 || abs(actualPosX - previousPosX) >100) {
								// use teleport
								updateInfo->GetDiscreteValues()[posXId] = actualPosX;
								updateInfo->GetDiscreteValues()[posYId] = actualPosY;
								continue;
							}
						}
					}
					deltas[posXId] = unit->GetTransform().localPos.x;
					deltas[posYId] = unit->GetTransform().localPos.y;
				}

				previous = deltas;

				UpdateMessage* msg = new UpdateMessage(updateInfo);
				spt<NetOutputMessage> netMsg = spt<NetOutputMessage>(new NetOutputMessage(1));
				netMsg->SetData(msg);
				netMsg->SetAction(StrId(NET_MSG_UPDATE));

				// if R is pressed, sending is stopped
				auto pressedKeys = CogGetPressedKeys();
				if (pressedKeys.size() == 0 || pressedKeys[0]->key != 'r') {
					netMsg->SetMsgTime(absolute);
					communicator->PushMessageForSending(netMsg);
				}

				if (pressedKeys.size() != 0) {
					if (pressedKeys[0]->key == 'm' && period < 20) period++;
					else if (pressedKeys[0]->key == 'n' && period > 2) period--;

					cout << "period changed to " << period << endl;
				}
			}
		}

		// copy transformations from model to view 
		auto& sprites = viewNode->GetMesh<MultiSpriteMesh>()->GetSprites();

		for (int i = 0; i < unitModels.size(); i++) {
			auto model = unitModels[i];
			auto view = sprites[i];

			view->transform = model->GetTransform();
		}
	}
};

class ExampleApp : public ofxCogApp {

	void RegisterComponents() {
		auto binder = new Interpolator();
		REGISTER_COMPONENT(binder);
		// this behavior is defined in config.xml
		REGISTER_BEHAVIOR(NetworkBehavior);
	}

	void InitEngine() {
		ofxCogEngine::GetInstance().Init("config.xml");
		ofxCogEngine::GetInstance().LoadStageFromXml(spt<ofxXml>(new ofxXml("config.xml")));
	}

	void InitStage(Stage* stage) {
		// get scene that is defined in xml
		Scene* main = stage->GetActualScene();
		TransformMath math = TransformMath();

		NodeBuilder bld = NodeBuilder();
		// get number of units defined in xml
		int units = GETCOMPONENT(ResourceCache)->GetProjectSettings().GetSettingValInt("scene_settings", "units");

		// create sprite sheet
		auto spriteSheet = GETCOMPONENT(ResourceCache)->GetSpriteSheet("pawn");
		auto spriteSet = spriteSheet->GetDefaultSpriteSet();

		// all units will be displayed as sprites -> therefore they must be defined twice
		// unit_view is a multisprite node that will display all units as sprites
		// unit_model tag has each unit that has only transformations and movement behaviors
		// during the update loop, model nodes are updated and transformations are copied into view sprites

		// define view
		Node* unitsView = new Node("unit_view");
		main->GetSceneNode()->AddChild(unitsView);
		spt<MultiSpriteMesh> mesh = spt<MultiSpriteMesh>(new MultiSpriteMesh("pawn"));
		unitsView->SetMesh(mesh);

		// add units into the scene
		for (int i = 0; i < units; i++) {
			// each unit will have separate node (and its own movement behavior)
			Node* unitModel = new Node("unit_model");
			main->GetSceneNode()->AddChild(unitModel);
			// rectangles are not rendered (instead of planes) and can be used for size calculations
			auto rectangle = spt<Cog::Rectangle>(new Cog::Rectangle(spriteSet->GetSpriteWidth(), spriteSet->GetSpriteHeight()));
			unitModel->SetMesh(rectangle);

			// set transformation of the unit (center of the screen)
			TransformEnt node2trans = TransformEnt();
			node2trans.pos = ofVec2f(0.5f, 0.5f);
			node2trans.anchor = ofVec2f(0.5f, 0.5f);
			node2trans.pType = CalcType::PER;
			node2trans.zIndex = 100;
			node2trans.sType = CalcType::GRID;
			node2trans.size = ofVec2f(3);

			// calculate transform with grid 100x50 units
			Trans trans;
			math.CalcTransform(trans, unitModel, main->GetSceneNode(), node2trans, 100, 50);

			// add sprite into mesh
			mesh->AddSprite(spt<SpriteInst>(new SpriteInst(Sprite(spriteSet, 0), trans)));

			// set transformation of the model node
			unitModel->SetTransform(trans);
		}

		main->GetSceneNode()->SubmitChanges(true);
	}
};


#ifndef WIN32
#include <jni.h>
#endif

int main() {
	ofSetupOpenGL(800, 450, OF_WINDOW);
	ofRunApp(new ExampleApp());
	return 0;
}

#ifndef WIN32

//========================================================================
extern "C" {
	void Java_cc_openframeworks_OFAndroid_init(JNIEnv*  env, jobject  thiz) {
		main();
	}
}
#endif



