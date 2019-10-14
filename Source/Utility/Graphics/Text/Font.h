#pragma once
#include <Utility/Graphics/OpenGL/Shader.h>
#include <Macros.h>
#include <map>

namespace Fed {
	// Font class. Tightly coupled with Freetype and OpenGL
	class Font
	{
	private:
		// Holds all state information relevant to a character as loaded using FreeType
		struct Character
		{
			GLuint textureID;		// ID handle of the glyph texture
			Vector2 size;			// Size of glphy
			Vector2 bearing;		// Offset from baseline to left/top glyph
			GLuint advance;			// Horizontal offset to advance to next glyph
		};
		// Holds the list of pre-compiled characters
		std::map<GLchar, Character> Characters;

		ShaderPtr textShader;
		GLuint textVAO, textVBO;

	public:
		Font();
		~Font();
		void Load(const char* fontPath);
		Vector2 GetTextSize(std::string text, float scale);
		void RenderText(std::string text, Vector2 pos, float scale, Vector3 & color);
	};
	typedef std::shared_ptr<Font> FontPtr;
}