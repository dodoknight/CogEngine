#pragma once

#include <string>
using namespace std;

#include "Definitions.h"
#include "MsgLua.h"

namespace Cog {

	class Scene;
	class NodeLua;
	class BehaviorLua;

	/**
	* Scene wrapper for Lua script
	*/
	class SceneLua{
	public:
		Scene* scene;

		SceneLua(Scene* scene): scene(scene) {

		}

		~SceneLua() {
		}

		NodeLua* GetSceneNode();

		string GetName() const;

		void SetName(string name);

		bool Loaded();

		bool Initialized() const;

		void SendMessage(MsgLua& msg);

		NodeLua* FindNodeById(int id) const;

		BehaviorLua* FindBehaviorById(int id) const;

		NodeLua* FindNodeByTag(string tag) const;

		NodeLua* FindNodeBySecondaryId(int secondaryId) const;

	};

} // namespace