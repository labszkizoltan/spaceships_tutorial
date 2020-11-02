#pragma once

#include <stdint.h> // withoud including this, the uint32_t type is not recognized

struct FrameBufferSpecification
{
	uint32_t Width, Height;
	uint32_t Samples = 1;

	bool SwapChainTarget = false; // this means sg like this (unbind all framebuffers and render to the screen): glBindFramebuffer(0);
};

class OpenGLFramebuffer
{
public:
	OpenGLFramebuffer(const FrameBufferSpecification& spec);
	~OpenGLFramebuffer();

	void Invalidate();

	void Bind();
	void Unbind();

//	void Resize(uint32_t width, uint32_t height); // probably not going to deal with this now

	uint32_t GetTextureID() const { return m_ColorAttachment; }

	const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

private:
	uint32_t m_RendererID = 0;
	uint32_t m_ColorAttachment = 0;
	uint32_t m_DepthAttachment = 0;
	FrameBufferSpecification m_Specification;
};





