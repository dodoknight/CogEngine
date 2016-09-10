#pragma once

#include "ofxCogCommon.h"
#include "Sprite.h"

namespace Cog {

	/**
	* Entity for rendering
	*/
	class Shape {
	protected:
		RenderType renderType = RenderType::NONE;
		ofColor color;
	public:

		Shape() {
		}

		Shape(RenderType renderType) : renderType(renderType) {

		}

		RenderType GetRenderType() {
			return renderType;
		}

		ofColor GetColor() {
			return color;
		}

		void SetColor(ofColor color) {
			this->color = color;
		}

		virtual float GetWidth() {
			return 1; // must be 1
		}

		virtual float GetHeight() {
			return 1; // must be 1
		}
	};

	class Rectangle : public Shape {
	private:
		float width = 0;
		float height = 0;

	public:
		Rectangle(float width, float height) : width(width), height(height) {
		}

		float GetWidth() {
			return width;
		}

		void SetWidth(float width) {
			this->width = width;
		}

		float GetHeight() {
			return height;
		}

		float SetHeight(float height) {
			this->height = height;
		}
	};

	class Image : public Shape {
	private:
		spt<ofImage> image;
	public:

		Image(spt<ofImage> img) : Shape(RenderType::IMAGE) {
			this->image = img;
		}

		spt<ofImage> GetImage() {
			if (!image->isUsingTexture()) {
				// for preloaded images, they need to be copied to the texture
				image->setUseTexture(true);
				image->update();
			}
			return image;
		}

		void SetImage(spt<ofImage> img) {
			this->image = img;
		}

		float GetWidth() {
			return image->getWidth();
		}

		float GetHeight() {
			return image->getHeight();
		}
	};

	class Text : public Shape {
	private:
		spt<ofTrueTypeFont> font;
		float size;
		string text;

	public:

		Text(spt<ofTrueTypeFont> font, string text, float size) : Shape(RenderType::TEXT) {
			this->font = font;
			this->size = size;
			this->text = text;
		}

		spt<ofTrueTypeFont> GetFont() {
			return font;
		}

		void SetFont(spt<ofTrueTypeFont> font) {
			this->font = font;
		}

		float GetSize() {
			return size;
		}

		void SetSize(float size) {
			this->size = size;
		}

		float GetWidth() {
			return GetTextWidth();
		}

		float GetHeight() {
			return GetTextHeight();
		
		}

		float GetTextWidth() {
			return font->stringWidth(text);

		}

		float GetTextHeight() {
			return font->stringHeight(text);
		}

		string GetText() {
			return text;
		}

		void SetText(string text) {
			this->text = text;
		}
	};

	class SpriteShape : public Shape {
	private:
		spt<Sprite> sprite;

	public:

		SpriteShape(spt<Sprite> sprite) : Shape(RenderType::SPRITE) {
			this->sprite = sprite;
		}

		spt<Sprite>& GetSprite() {
			return sprite;
		}
	};

	/**
	* Crate for multisprite object.
	* Multisprite differs from sprite that it
	* has transformation coordinates in it
	*/
	class MSpriteCrate {
	public:
		spt<Sprite> sprite;
		Trans transform;
	};

	class SpritesShape : public Shape {
	private:
		vector<spt<MSpriteCrate>> sprites;
		int width = 1;
		int height = 1;
		string name;

	public:
		SpritesShape(string name, vector<spt<MSpriteCrate>> sprites) : Shape(RenderType::MULTISPRITE), name(name), sprites(sprites) {
			Recalc();
		}

		void Recalc() {

			int minX = 0;
			int minY = 0;
			int maxX = 0;
			int maxY = 0;
			int mWidth = 0;
			int mHeight = 0;

			for (auto it = sprites.begin(); it != sprites.end(); ++it) {
				spt<MSpriteCrate> crt = (*it);
				int posX = crt->transform.localPos.x;
				int posY = crt->transform.localPos.y;

				if (posX < minX) minX = posX;
				if (posY < minY) minY = posY;
				
				if (posX > maxX) {
					maxX = posX;
					mWidth = crt->sprite->GetWidth();
				}

				if (posY > maxY) {
					maxY = posY;
					mHeight = crt->sprite->GetHeight();
				}
			}

			this->width = (maxX - minX) + mWidth;
			this->height = (maxY - minY) + mHeight;
		}

		vector<spt<MSpriteCrate>>& GetSprites() {
			return sprites;
		}

		string GetName() {
			return name;
		}

		float GetWidth() {
			return width;
		}

		float GetHeight() {
			return height;

		}
	};

}// namespace