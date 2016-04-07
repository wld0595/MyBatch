#include "Mesh.h"

Mesh::Mesh(Vertex*& vertexBufferDate)
	:m_mesh(NULL)
	
{
	makeVertexBuffer(vertexBufferDate);
	addManageMesh();
}

Mesh::~Mesh()
{
	glDeleteBuffers(1,&m_uiVertexBuffer);
	glDeleteVertexArrays(1,&m_uiVertexArrayID);
}

void Mesh::makeVertexBuffer(Vertex*& vertexBufferData)
{
	//Generate the vertex array
	glGenVertexArrays(1,&m_uiVertexArrayID);
	glBindVertexArray(m_uiVertexArrayID);
	if (glIsVertexArray(m_uiVertexArrayID)==GL_FALSE)
	{
		printf("Failed to generate or bind the vertex array.Please try again.\n");
		exit(1);
	}

	//Bind the array buffer
	glGenBuffers(1,&m_uiVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,m_uiVertexBuffer);
	if (glIsBuffer(m_uiVertexBuffer)==GL_FALSE)
	{
		printf("Failed to generate or bind the vertex buffer.Please try again.\n");
		exit(1);
	}
	//===Dataset to the vertex array buffer
	glBufferData(GL_ARRAY_BUFFER,(sizeof(Vertex)*4),vertexBufferData,GL_STATIC_DRAW);
}

GLvoid Mesh::render(ShaderProgram shader)
{
	bind(shader);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	unbind(shader);
}

GLvoid Mesh::addManageMesh()
{
	m_mesh.push_back(this);
}

GLuint Mesh::getVertexBufferHandle()
{
	return m_uiVertexBuffer;
}

GLuint Mesh::getVertexArrayHandle()
{
	return m_uiVertexArrayID;
}

GLvoid Mesh::bind(ShaderProgram shader)
{
	GLint numAttributes=shader.getAttributeSize();
	if (numAttributes<0)
	{
		printf("No exit attribute.Please check!\n");
		return ;
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexBuffer);

	for (int i=0;i<numAttributes;i++)
	{
		shader.enableVertexAttribute(i);
		if (shader.getAttributeName(i)=="UV")
		{
			shader.setVertexAttribute(i,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*2,(void*)12);
		}
		else
		{
			shader.setVertexAttribute(i,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat)*2,(void*)0);
		}	
	}
}

GLvoid Mesh::unbind(ShaderProgram shader)
{
	GLint numAttributes=shader.getAttributeSize();
	if (numAttributes<0)
	{
		printf("No exit attribute.Please check!\n");
		return ;
	}
	for (int i=0;i<numAttributes;i++)
	{
		shader.disableVertexAttribute(i);
	}
}