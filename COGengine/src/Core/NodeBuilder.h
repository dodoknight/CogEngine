#pragma once

#include "Definitions.h"
#include "ofColor.h"
#include "Mesh.h"
#include "BehaviorEnt.h"
#include "pugixml.h"

using namespace pugi;

namespace Cog {

	class Behavior;
	class Node;
	class Scene;

	/**
	* Factory class that fills the given node with necessary attributes and therefore
	* it constructs a well-defined object
	*
	* May also load nodes and behaviors from XML
	*/
	class NodeBuilder {

	public:


		/**
		* Fills given node with button behavior
		* @param node node to fill
		* @param defaultImg path to default image
		* @param clickedImg path to image for clicked state
		* @param disabledImg path to image for disabled state
		*/
		void CreateButtonNode(Node* node, string defaultImg, string clickedImg, string disabledImg);

		/**
		* Fills given node with multiselection behavior (checkbox)
		* @param node node to fill
		* @param defaultImg path to default image
		* @param selectImg path to image for selected state
		* @param selectionGroup name of checkbox group
		*/
		void CreateMultiSelectionNode(Node* node, string defaultImg, string selectImg, string selectionGroup);

		/**
		* Fills given node with selection behavior (radio button)
		* @param node node to fill
		* @param defaultImg path to default image
		* @param selectImg path to image for selected state
		*/
		void CreateSelectionNode(Node* node, string defaultImg, string selectImg);

		/**
		* Fills given node with animation
		* @param node node to fill
		* @param animName name of the animation
		*/
		void CreateAnimationNode(Node* node, string animName);

		/**
		* Fills given node with image mesh
		* @param node node to fill
		* @param path path to image
		*/
		void CreateImageNode(Node* node, string path);

		/**
		* Fills given node with text mesh
		* @param node node to fill
		* @param font font name
		* @param size font size
		* @param color font color
		* @param text displayed text
		*/
		void CreateTextNode(Node* node, string font, float size, ofColor color, string text);

		/**
		* Fills given node with label mesh
		* @param node node to fill
		* @param font font name
		* @param width label width
		* @param size font size
		* @param color font color
		* @param text displayed text
		*/
		void CreateLabelNode(Node* node, string font, float width, float size, ofColor color, string text);

		/**
		* Fills given node with sprite mesh
		* @param scene scene whose layers contain the sprite
		* @param node node to fill
		* @param layer name of the layer the sprite should be rendered on
		* @param row sprite row on the sprite sheet
		* @param column sprite column on the sprite sheet
		*/
		void CreateSpriteNode(Scene* scene, Node* node, string layer, int row, int column);

		/**
		* Fills given node with sprite mesh
		* @param scene scene whose layers contain the sprite
		* @param node node to fill
		* @param layer name of the layer the sprite should be rendered on
		* @param spritesheet sprite sheet the sprite consists of
		* @param row sprite row on the sprite sheet
		* @param column sprite column on the sprite sheet
		*/
		void CreateSpriteNode(Scene* scene, Node* node, string layer, string spritesheet, int row, int column);

		/**
		* Fills given node with rectangle mesh
		* @param node node to fill
		* @param size rectangle size in pixels
		* @param color rectangle color
		* @param noFill indicator whether the rectangle should be filled
		*/
		void CreateRectangleNode(Node* node, ofVec2f size, ofColor color, bool noFill);

		/**
		* Fills given node with bounding box (a box whose size is set according to its children)
		* @param scene a scene into which the node will be later added
		* @param node node to fill
		* @param color bounding box color
		* @param margin box margin (percentage size of its children)
		* @param renderable indicator whether the boudning box should be rendered
		*
		*/
		void CreateBoundingBoxNode(Scene* scene, Node* node, ofColor color, float margin, bool renderable);

		/**
		* Creates a sprite mesh from a scene layer
		* @param scene actual scene
		* @param layer name of a layer in which the sprite will be rendered
		* @param row sprite row on the sprite sheet
		* @param column sprite column on the sprite sheet
		*/
		spt<SpriteMesh> CreateSpriteMesh(Scene* scene, string layer, int row, int column);

		/**
		* Creates a sprite mesh from a scene layer and given sprite set
		* @param scene actual scene
		* @param layer name of a layer in which the sprite will be rendered
		* @param spriteSheet name of the sprite sheet the sprite consists of
		* @param row sprite row on the sprite sheet
		* @param column sprite column on the sprite sheet
		*/
		spt<SpriteMesh> CreateSpriteMesh(Scene* scene, string layer, string spriteSheet, int row, int column);

		/**
		* Creates a rectangle mesh 
		* @param size rectangle size
		* @param color rectangle color
		*/
		spt<FRect> CreateRectangleMesh(ofVec2f size, ofColor color);

		/**
		* Creates a behavior from given description entity (usually loaded from xml)
		*/
		Behavior* CreateBehavior(spt<BehaviorEnt> behEntity);

		/**
		* Creates node and assigns it to the given scene
		* @param name node name
		* @param scene scene this node belongs to
		*/
		Node* CreateNode(string name, Scene* scene);

		/**
		* Loads node from XML
		* @param xml xml object that holds scenes
		* @param parent parent of the node
		* @param scene actual scene
		*/
		Node* LoadNodeFromXml(xml_node& xml, Node* parent, Scene* scene);

		/**
		* Loads referenced node from XML, must be located in nodes.xml file
		* @param contextXml xml object that holds context
		* @param nodeName name of the node
		* @param parent parent of the node
		* @param scene actual scene
		*/
		Node* LoadRefNodeFromXml(xml_node& contextXml, string nodeName, Node* parent, Scene* scene);

		/**
		* Loads text node from XML
		* @param xml xml object
		* @param node node to fill
		* @param parent parent of the node
		*/
		void LoadTextFromXml(xml_node& xml, Node* node, Node* parent);

		/**
		* Loads label node from XML
		* @param xml xml object
		* @param node node to fill
		* @param parent parent of the node
		*/
		void LoadLabelFromXml(xml_node& xml, Node* node, Node* parent);

		/**
		* Loads behavior from XML
		* @param xml xml object
		* @param node node to fill
		*/
		void LoadBehaviorFromXml(xml_node& xml, Node* node);

		/**
		* Loads mesh from xml
		* @param xml xml object
		* @param node node to fill
		* @param scene actual scene
		*/
		void LoadMeshFromXml(xml_node& xml, Node* node, Scene* scene);

	private:
		/**
		* Checks if the string serves as a reference to a resource (starts with @ character)
		*/
		string checkResource(string value);
	};

}// namespace