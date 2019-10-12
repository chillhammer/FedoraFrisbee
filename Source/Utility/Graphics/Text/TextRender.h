#pragma once
#include <ft2build.h>

namespace Fed {
	// Font class. Tightly coupled with Freetype and OpenGL
	class Font
	{
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

			Shader* textShader;
			GLuint textVAO, textVBO;

		public:
			Text();
			~Text();
			void Load(const char* fontPath);
			void RenderText(std::string text, Vector2 pos, GLfloat scale, Vector3 & color, GLsizei screenWidth, GLsizei screenHeight);
		};
	};

}