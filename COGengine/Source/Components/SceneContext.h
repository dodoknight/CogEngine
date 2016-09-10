#pragma once

#include "Node.h"
#include "Behavior.h"
#include "Scene.h"
#include "Component.h"
#include "Tween.h"

namespace Cog {

	/**
	* Node context
	*/
	class SceneContext : public Component{
	
		OBJECT(SceneContext)
	
	private:
		// message listeners
		map<StringHash, vector<MsgListener*>> msgListeners;
		// listeners ids and their registered actions
		map<int, vector<StringHash>> msgListenerActions;
		// list of all nodes
		vector<Node*> allNodes;
		// list of all behaviors
		vector<Behavior*> allBehaviors;
		// actual scene
		Scene* actualScene = nullptr;
		// list of all scenes
		vector<Scene*> scenes = vector<Scene*>();

		// root object, should be created only once
		Node* rootObject = nullptr;

	public:

		void Init();

		/**
		* Gets the root object
		*/
		Node* GetRootObject() {
			return rootObject;
		}

		/**
		* Gets actual scene
		*/
		Scene* GetActualScene() {
			return actualScene;
		}

		/**
		* Adds a new scene
		* @param setAsActual if true, scene will be set as the actual scene
		*/
		void AddScene(Scene* scene, bool setAsActual) {

			rootObject->AddChild(scene->GetSceneNode());

			if (setAsActual) {
				this->actualScene = scene;
				scene->GetSceneNode()->SetRunningMode(RunningMode::RUNNING);
			}
			else {
				scene->GetSceneNode()->SetRunningMode(RunningMode::DISABLED);
			}

			rootObject->SubmitChanges(true);
		}

		/**
		* Registers behavior listener for selected action
		* @param action action to register
		* @param listener listener that will be called when selected action is invoked
		*/
		void RegisterListener(StringHash action, MsgListener* listener) {
			if (msgListeners.find(action) == msgListeners.end()) {
				msgListeners[action] = vector <MsgListener*>();
			}

			vector<MsgListener*>& listeners = msgListeners[action];
			listeners.push_back(listener);

			if (msgListenerActions.find(listener->GetId()) == msgListenerActions.end()) {
				msgListenerActions[listener->GetId()] = vector<StringHash>();
			}

			msgListenerActions[listener->GetId()].push_back(action);
		}

		/**
		* Unregisters message listener for selected action
		* @return true if listener has been found and erased
		*/
		bool UnregisterListener(StringHash action, MsgListener* listener) {
			if (msgListeners.find(action) != msgListeners.end()) {
				vector<MsgListener*>& listeners = msgListeners[action];

				for (auto it = listeners.begin(); it != listeners.end(); ++it) {
					if ((*it)->GetId() == listener->GetId()) {
						listeners.erase(it);
						return true;
					}
				}
			}
			return false;
		}

		/**
		* Unregisters all actions that are bound with selected listener
		* @param beh listener to unregister
		*/
		void UnregisterListener(MsgListener* beh) {
			auto found = msgListenerActions.find(beh->GetId());

			if (found != msgListenerActions.end()) {

				vector<StringHash> actions = found->second;

				// unregister all actions
				for (auto action : actions) {
					UnregisterListener(action, beh);
				}

				// remove from the second collection
				msgListenerActions.erase(beh->GetId());
			}
		}

		/**
		* Sends common message
		* @param msg message  to send
		* @param actualNode actual node in bubbling algorithm
		*/
		void SendMessage(Msg& msg, Node* actualNode);

		/**
		* Sends message to specific listener
		* @param msg message  to send
		* @param targetId id of target listener
		*/
		void SendDirectMessageToListener(Msg& msg, int targetId);

		/**
		* Sends message to specific node
		* @param msg message  to send
		* @param targetId id of target node
		*/
		void SendDirectMessageToNode(Msg& msg, int targetId);

		/**
		* Returns true, if there is at least one behavior listener for selected action
		*/
		bool IsRegisteredListener(StringHash action) const {
			return msgListeners.find(action) != msgListeners.end();
		}

		/**
		* Returns true, if listener is listening for selected action
		*/
		bool IsRegisteredListener(int action, MsgListener* beh) {
			if (msgListenerActions.find(beh->GetId()) == msgListenerActions.end()) return false;

			vector<StringHash>& actions = msgListenerActions[beh->GetId()];

			return (std::find(actions.begin(), actions.end(), action) != actions.end());
		}

		/**
		* Finds node by id
		*/
		Node* FindNodeById(int id) const;

		/**
		* Finds behavior by id
		*/
		Behavior* FindBehaviorById(int id) const;

		/**
		* Finds scene by its name
		*/
		Scene* FindSceneByName(string name) const;

		/**
		* Gets number of nodes with specific tag
		*/
		int GetNodesCountByTag(string tag) const;

		/**
		* Finds node by tag
		*/
		Node* FindNodeByTag(string tag) const;

		/**
		* Finds all nodes by tag
		*/
		vector<Node*> FindNodesByTag(char* tag) const;

		/**
		* Gets number of nodes with specific subtype
		*/
		int GetNodesCountBySubType(int subtype) const;

		/**
		* Finds node by subtype
		*/
		Node* FindNodeBySubType(int subtype) const;

		/**
		* Finds all nodes by subtype
		*/
		vector<Node*> FindNodesBySubType(int subtype) const;

		/**
		* Adds a new node to the collection
		* @return true, if node has been added
		*/
		bool AddNode(Node* node) {
			auto found = find(allNodes.begin(), allNodes.end(), node);
			if (found == allNodes.end()) {
				allNodes.push_back(node);
				return true;
			}
			else return false;
		}

		/**
		* Removes node from collection
		*/
		void RemoveNode(Node* node) {
			auto found = find(allNodes.begin(), allNodes.end(), node);
			if (found != allNodes.end()) allNodes.erase(found);
		}

		/**
		* Adds a new behavior
		* @return true if behavior has been added
		*/
		bool AddBehavior(Behavior* beh) {
			auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
			if (found == allBehaviors.end()) {
				allBehaviors.push_back(beh);

				return true;
			}
			else return false;
		}

		/**
		* Removes behavior from collection
		*/
		void RemoveBehavior(Behavior* beh) {
			auto found = find(allBehaviors.begin(), allBehaviors.end(), beh);
			if (found != allBehaviors.end()) allBehaviors.erase(found);

			UnregisterListener(beh);
		}

		void SwitchToScene(Scene* scene, TweenDirection tweenDir);

		/**
		* Loads scene from xml
		*/
		void LoadScenesFromXml(spt<ofxXml> xml);

	private:

		/**
		* Sends message to behaviors
		* @param msg message to send
		* @param actualNode actual node in bubbling algorithm
		*/
		void SendMessageToBehaviors(Msg& msg, Node* actualNode);

		/**MM
		* Sends message to children of actual node
		* @param msg message to send
		* @param actualNode actual node in bubbling algorithm
		*/
		void SendBubblingMessageToChildren(Msg& msg, Node* actualNode);

		/**
		* Sends message that will be bubbled from actualNode
		* @param msg message to send
		* @param actualNode actual node in bubbling algorithm
		*/
		void SendBubblingMessage(Msg& msg, Node* actualNode);

		/**
		* Sends direct message - takes all behaviors and callbacks that have
		* registered selected action
		* @param msg message  to send
		*/
		void SendDirectMessage(Msg& msg);

	};

}// namespace