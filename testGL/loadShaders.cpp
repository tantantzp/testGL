//////////////////////////////////////////////////////////////////////////////
//
//  --- LoadShaders.cxx ---
//
//////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <assert.h>

// #define GLEW_STATIC
#include "LoadShaders.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	//----------------------------------------------------------------------------

	static const GLchar *
		readShader(const char *filename)
	{
#ifdef WIN32
			FILE *infile;
			fopen_s(&infile, filename, "rb");
#else
			FILE *infile = fopen(filename, "rb");
#endif // WIN32

			if (!infile)
			{
				std::cerr << "Unable to open file '" << filename << "'" << std::endl;
				assert(false);
				return NULL;
			}

			fseek(infile, 0, SEEK_END);
			long len = ftell(infile);
			fseek(infile, 0, SEEK_SET);

			GLchar *source = new GLchar[len + 1];

			fread(source, 1, len, infile);
			fclose(infile);

			source[len] = 0;

			return const_cast<const GLchar *>(source);
		}

	//----------------------------------------------------------------------------

	GLuint
		loadShaders(ShaderInfo *shaders)
	{
			if (shaders == NULL)
			{
				return 0;
			}

			GLuint program = glCreateProgram();

			ShaderInfo *entry = shaders;
			while (entry->type != GL_NONE)
			{
				GLuint shader = glCreateShader(entry->type);

				entry->shader = shader;

				const GLchar *source = readShader(entry->filename);
				if (source == NULL)
				{
					for (entry = shaders; entry->type != GL_NONE; ++entry)
					{
						glDeleteShader(entry->shader);
						entry->shader = 0;
					}

					return 0;
				}

				glShaderSource(shader, 1, &source, NULL);
				delete[] source;

				glCompileShader(shader);

				GLint compiled;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
				if (!compiled)
				{
					GLsizei len;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

					GLchar *log = new GLchar[len + 1];
					glGetShaderInfoLog(shader, len, &len, log);
					std::cerr << "Shader compilation failed: " << log << std::endl;
					delete[] log;

					assert(false);

					return 0;
				}

				glAttachShader(program, shader);

				++entry;
			}
			return program;

#ifdef GL_VERSION_4_1
#endif /* GL_VERSION_4_1 */
		}
	void linkProgram(GLuint program)
	{

		glLinkProgram(program);

		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLsizei len;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

			GLchar *log = new GLchar[len + 1];
			glGetProgramInfoLog(program, len, &len, log);
			std::cerr << "Shader linking failed: " << log << std::endl;
			delete[] log;
			/*
			for (entry = shaders; entry->type != GL_NONE; ++entry)
			{
			glDeleteShader(entry->shader);
			entry->shader = 0;
			}
			*/
			assert(false);
		}
	}

	//----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif // __cplusplus
