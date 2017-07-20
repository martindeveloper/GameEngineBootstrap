#if RENDERER_OPENGL4

#include "OpenGL4FrameBuffer.h"

using namespace Renderer;

OpenGL4FrameBuffer::OpenGL4FrameBuffer(Renderer::OpenGL4Renderer* renderer)
{
	Renderer = renderer;
	ScreenShaderProgramId = 0;
}

OpenGL4FrameBuffer::~OpenGL4FrameBuffer()
{
	glDeleteVertexArrays(1, &ScreenVAO);
	glDeleteBuffers(1, &ScreenVBO);
	glDeleteProgram(ScreenShaderProgramId);
}

void OpenGL4FrameBuffer::Create()
{
	PrepareScreenQuad();
	CreateBuffer();
}

void OpenGL4FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	assert(glGetError() == GL_NO_ERROR);
}

void OpenGL4FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGL4FrameBuffer::Draw()
{
	// Bind default frame buffer
	Unbind();

	glBindVertexArray(ScreenVAO);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(ScreenShaderProgramId);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ColorBufferTexture);

	// Draw screen quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	assert(glGetError() == GL_NO_ERROR);
}

void OpenGL4FrameBuffer::CreateBuffer()
{
	glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	// Color buffer
	glGenTextures(1, &ColorBufferTexture);
	glBindTexture(GL_TEXTURE_2D, ColorBufferTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Renderer->Parameters.Width, Renderer->Parameters.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach color buffer to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorBufferTexture, 0);

	// Depth-stencil buffer
	glGenRenderbuffers(1, &DepthStencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Renderer->Parameters.Width, Renderer->Parameters.Height);

	// Attach depth-stencil buffer to frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthStencilBuffer);

	assert(glGetError() == GL_NO_ERROR);
}

void OpenGL4FrameBuffer::SwitchProstProcessEffect(const char* vertexShaderName, const char* fragmentShaderName)
{
	if (ScreenShaderProgramId > 0)
	{
		glDeleteProgram(ScreenShaderProgramId);
	}

	GLuint vertexShader = Renderer->CompileShader(vertexShaderName, GL_VERTEX_SHADER);
	GLuint fragmentShader = Renderer->CompileShader(fragmentShaderName, GL_FRAGMENT_SHADER);

	ScreenShaderProgramId = Renderer->CreateShaderProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void OpenGL4FrameBuffer::PrepareScreenQuad()
{
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);

	glGenVertexArrays(1, &ScreenVAO);
	glBindVertexArray(ScreenVAO);

	glGenBuffers(1, &ScreenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, ScreenVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(ScreenQuadVerticies), ScreenQuadVerticies, GL_STATIC_DRAW);

	// Shaders
	SwitchProstProcessEffect(PostProcessDefaultVertex, PostProcessDefaultFragment);

	glBindVertexArray(ScreenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ScreenVBO);

	GLint posAttrib = glGetAttribLocation(ScreenShaderProgramId, "vertexPosition");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(ScreenShaderProgramId, "vertexUV");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glUseProgram(ScreenShaderProgramId);
	glUniform1i(glGetUniformLocation(ScreenShaderProgramId, "frameBufferTextureSampler"), 0);
	
	GLint error = glGetError();

	assert(error == GL_NO_ERROR);
}

#endif
