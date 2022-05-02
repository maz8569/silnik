#pragma once
#include <glad/glad.h>

namespace GameEngine {

    class VertexBuffer {
	public:
		VertexBuffer(GLenum type);
		~VertexBuffer();

		void BufferData(GLsizeiptr size, void* data, GLenum usage);
		void BufferSubData(GLintptr offset, GLsizeiptr size, void* data);
		void Bind();
		void Unbind();
		void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized,
			GLsizei stride, const GLvoid* pointer);

		void VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);

		GLuint buffer_id;
		GLenum type;
    };

}
