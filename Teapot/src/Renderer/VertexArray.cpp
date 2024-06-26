#include "VertexArray.h"

namespace Teapot
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_RendererID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void VertexArray::AddBuffer(const VertexBuffer& vb, GLuint layout, GLuint numComponents, GLsizeiptr stride, const void* offset) const
    {
        vb.Bind();
        glVertexAttribPointer(layout, numComponents, GL_FLOAT, GL_FALSE, (GLsizei)stride, offset);
        glEnableVertexAttribArray(layout);
        vb.Unbind();
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
}