#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "ShaderProgram.h"

//Vertex data struct
typedef struct 
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat u;
	GLfloat v;
}Vertex;

class Mesh{
public:
	Mesh(Vertex*& vertexBufferDate);
	~Mesh();
	GLvoid addManageMesh();
	GLvoid makeVertexBuffer(Vertex*& vertexBufferDate);
	GLvoid render(ShaderProgram shader);
	GLvoid bind(ShaderProgram shader);
	GLvoid unbind(ShaderProgram shader);
	GLuint getVertexBufferHandle();
	GLuint getVertexArrayHandle();
private:
	GLuint m_uiVertexBuffer;
	GLuint m_uiVertexArrayID;
	std::vector<Mesh*> m_mesh;
};