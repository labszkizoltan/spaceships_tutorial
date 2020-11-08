
#include "buffer.h"

#include "renderer.h"



static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:		return GL_FLOAT;
	case ShaderDataType::Float2:	return GL_FLOAT;
	case ShaderDataType::Float3:	return GL_FLOAT;
	case ShaderDataType::Float4:	return GL_FLOAT;
	case ShaderDataType::Mat3:		return GL_FLOAT;
	case ShaderDataType::Mat4:		return GL_FLOAT;
	case ShaderDataType::Int:		return GL_INT;
	case ShaderDataType::Int2:		return GL_INT;
	case ShaderDataType::Int3:		return GL_INT;
	case ShaderDataType::Int4:		return GL_INT;
	case ShaderDataType::Bool:		return GL_BOOL;
	}

	return 0;
}


//--------------------------------------------//
//-------------- Vertex Buffer ---------------//
//--------------------------------------------//


OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float * vertices, uint32_t size)
{
	GLCall(glCreateBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
}

// // copy constructor
// OpenGLVertexBuffer::OpenGLVertexBuffer(const OpenGLVertexBuffer & other)
// 	: m_RendererID(other.m_RendererID), m_Layout(other.m_Layout)
// {
// 	std::cout << "OpenGLVertexBuffer::OpenGLVertexBuffer(const OpenGLVertexBuffer & other) is being used!" << std::endl;
// }
// 
// // copy assignment
// OpenGLVertexBuffer & OpenGLVertexBuffer::operator=(const OpenGLVertexBuffer & other)
// {
// 	// shouldn't the resources of the original object need to be freed? like this:
// 	glDeleteBuffers(1, &m_RendererID);
// 	return *this = OpenGLVertexBuffer(other);
// }

// move constructor
OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer && other) noexcept
{
	m_RendererID = other.m_RendererID;
	m_Layout = other.m_Layout;

	other.m_RendererID = 0;
}

// move assignment
OpenGLVertexBuffer & OpenGLVertexBuffer::operator=(OpenGLVertexBuffer && other) noexcept
{
	if (this != &other)
	{
		glDeleteBuffers(1, &m_RendererID);

		m_RendererID = other.m_RendererID;
		m_Layout = other.m_Layout;

		other.m_RendererID = 0;
	}

	return *this;
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void * data, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void OpenGLVertexBuffer::SetLayout(const BufferLayout & layout)
{
	m_Layout = layout;
	SetLayout();
}

void OpenGLVertexBuffer::SetLayout()
{
	Bind();

	uint32_t index = 0;

	for (int i = 0; i < m_Layout.m_Elements.size(); i++)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,
			m_Layout.m_Elements[i].GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(m_Layout.m_Elements[i].Type),
			m_Layout.m_Elements[i].Normalized ? GL_TRUE : GL_FALSE,
			m_Layout.GetStride(),
			(const void*)m_Layout.m_Elements[i].Offset
		);
		index++;
	}

	// when I move assigned TexturedShadedMesh in Scene::Scene(const std::string & filename), the Draw() call failed, because the "element" in the below loop became invalid,
	// maybe the iterators that are needed for the below auto etc. syntax were invalidated on the move assignment, because when I made the m_Elements public in the layout class,
	// and changed the for loop to the above one, the error disappeared
//	for (const auto& element : m_Layout)
//	{
//		glEnableVertexAttribArray(index);
//		glVertexAttribPointer(
//			index,
//			element.GetComponentCount(),
//			ShaderDataTypeToOpenGLBaseType(element.Type),
//			element.Normalized ? GL_TRUE : GL_FALSE,
//			m_Layout.GetStride(),
//			(const void*)element.Offset
//		);
//		index++;
//	}
}


//-------------------------------------------//
//-------------- Index Buffer ---------------//
//-------------------------------------------//


OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t * indices, uint32_t count)
	: m_Count(count)
{
	GLCall(glCreateBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW));
}

// // copy constructor
// OpenGLIndexBuffer::OpenGLIndexBuffer(const OpenGLIndexBuffer & other)
// 	: m_RendererID(other.m_RendererID), m_Count(other.m_Count)
// {
// 	std::cout << "OpenGLIndexBuffer::OpenGLIndexBuffer(const OpenGLIndexBuffer & other) is being used!" << std::endl;
// }
// 
// // copy assignment
// OpenGLIndexBuffer & OpenGLIndexBuffer::operator=(const OpenGLIndexBuffer & other)
// {
// 	// shouldn't the resources of the original object need to be freed? like this:
// 	glDeleteBuffers(1, &m_RendererID);
// 	return *this = OpenGLIndexBuffer(other);
// }

// move constructor
OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer && other) noexcept
{
	m_RendererID = other.m_RendererID;
	m_Count = other.m_Count;
	other.m_RendererID = 0;
}

// move assignment
OpenGLIndexBuffer & OpenGLIndexBuffer::operator=(OpenGLIndexBuffer && other) noexcept
{
	if (this != &other)
	{
		glDeleteBuffers(1, &m_RendererID);

		m_RendererID = other.m_RendererID;
		m_Count = other.m_Count;

		other.m_RendererID = 0;
	}

	return *this;
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}






