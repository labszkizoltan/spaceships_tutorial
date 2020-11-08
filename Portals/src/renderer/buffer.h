#pragma once

#include <string>
#include <vector>


enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4,
	Mat3, Mat4,
	Int, Int2, Int3, Int4,
	Bool
};

// size in bytes
static uint32_t ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::None:		return 0;
	case ShaderDataType::Float:		return 4;
	case ShaderDataType::Float2:	return 8;
	case ShaderDataType::Float3:	return 12;
	case ShaderDataType::Float4:	return 16;
	case ShaderDataType::Mat3:		return 36;
	case ShaderDataType::Mat4:		return 64;
	case ShaderDataType::Int:		return 4;
	case ShaderDataType::Int2:		return 8;
	case ShaderDataType::Int3:		return 12;
	case ShaderDataType::Int4:		return 16;
	case ShaderDataType::Bool:		return 1;
	}

	return 0;
}

struct BufferElement
{
	std::string Name;
	ShaderDataType Type;
	uint32_t Size;
	uint32_t Offset;
	bool Normalized;

	BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
		: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
	{
	}

	uint32_t GetComponentCount() const
	{
		switch (Type)
		{
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 9;
		case ShaderDataType::Mat4:		return 16;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
		}

		return 0;
	}
};

class BufferLayout
{
public:
	BufferLayout() {}

	BufferLayout(const std::initializer_list<BufferElement>& elements)
		:m_Elements(elements)
	{
		CalculateOffsetAndStride();
	}

	inline uint32_t GetStride() const { return m_Stride; }
	inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

private:
	void CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;

//		for (auto& element : m_Elements)
		for (BufferElement& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

//private:
public:
	std::vector<BufferElement> m_Elements;
	uint32_t m_Stride = 0;
};



class OpenGLVertexBuffer
{
public:
	OpenGLVertexBuffer() = default;
	OpenGLVertexBuffer(uint32_t size);
	OpenGLVertexBuffer(float* vertices, uint32_t size);

	OpenGLVertexBuffer(const OpenGLVertexBuffer& other) = delete;// copy constructor
	OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer& other) = delete;// copy assignment
	OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept; // move constructor
	OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& other) noexcept; // move assignment
	~OpenGLVertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetData(const void* data, uint32_t size);

	const BufferLayout& GetLayout() const { return m_Layout; };
	void SetLayout(const BufferLayout& layout);
	void SetLayout();


public:
	uint32_t m_RendererID;
	BufferLayout m_Layout;

	friend class Scene;
};


class OpenGLIndexBuffer
{
public:
	OpenGLIndexBuffer() = default;
	OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

	OpenGLIndexBuffer(const OpenGLIndexBuffer& other) = delete;// copy constructor
	OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer& other) = delete;// copy assignment
	OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept; // move constructor
	OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&& other) noexcept; // move assignment
	~OpenGLIndexBuffer();

	void Bind() const;
	void Unbind() const;

	uint32_t GetCount() const { return m_Count; };
public:
	uint32_t m_RendererID;
	uint32_t m_Count;

	friend class Scene;
};




