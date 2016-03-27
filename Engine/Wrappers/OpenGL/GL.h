#ifndef GLWrapper_H
#define GLWrapper_H

#include <GL/glew.h>
#include <string>

class GL
{
public:
	// Viewport
	static void Viewport(GLint x,  GLint y,  GLsizei width,  GLsizei height); 

	// Frame Buffer
	static void Clear(GLbitfield  mask);
	static void ClearColor(GLclampf red,  GLclampf green,  GLclampf blue,  GLclampf alpha); 

	// Culling
	static void CullFace(GLenum mode);

	// Behaviour
	static void Hint(GLenum target,  GLenum mode);

	// Draw Calls
	static void DrawElements (GLenum mode, GLsizei count, GLenum type, const void* indices);
	static void DrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);

	// Buffers
	static void BufferData (GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
	static void BufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);

	// Attributes
	static void EnableVertexAttribArray (GLuint index);
	static void VertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
	static void VertexAttribDivisor (GLuint index, GLuint divisor);

	// Bind
	static void BindVertexArray (GLuint array);
	static void BindBuffer (GLenum target, GLuint buffer);

	// Depth Test
	static void DepthMask (GLboolean flag);
	static void ClearDepth(GLclampd  depth);
	static void DepthFunc(GLenum func);

	// Blend
	static void BlendFunc (GLenum sfactor, GLenum dfactor);
	static void BlendFunci (GLuint buf, GLenum sfactor, GLenum dfactor);

	// Generators
	static void GenVertexArrays (GLsizei n, GLuint * arrays);
	static void GenBuffers(GLsizei n,  GLuint * buffers);
	static void GenTextures(GLsizei n,  GLuint * textures);

	// Textures
	static void BindTexture(GLenum target, GLuint texture);
	static void TexImage2D(GLenum target,  GLint level,  GLint internalformat,  GLsizei width,  
		GLsizei height,  GLint border,  GLenum format,  GLenum type,  const GLvoid * data); 
	static void TexEnvi(GLenum target,  GLenum pname,  GLint param);
	static void TexEnvf(GLenum target,  GLenum pname,  GLfloat param);
	static void TexParameteri(GLenum target,  GLenum pname,  GLint param);
	static void TexParameterf(GLenum target,  GLenum pname,  GLfloat param);

	// Capabilities
	static void Enable (GLenum cap);
	static void Disable (GLenum cap);
	static void IsEnabled(GLenum cap, bool *val);

	// Getters
	static void GetBooleanv(GLenum pname, GLboolean * params); 
	static void GetFixedv(GLenum pname, GLfixed * params); 
	static void GetFloatv(GLenum pname, GLfloat * params); 
	static void GetIntegerv(GLenum pname, GLint * params);

	// Cleaning
	static void DeleteVertexArrays (GLsizei n, const GLuint *arrays);
	static void DeleteBuffers(GLsizei n, const GLuint * buffers);

	static void Check ();

private:
	// Maybe use this only on development mode?
	static void ErrorCheck (const std::string& methodName);
};

#endif