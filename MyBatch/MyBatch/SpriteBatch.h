#include <stdio.h>
#include <stdlib.h>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
#include "Batch.h"
#include "Texture.h"
#include "Mesh.h"

class SpriteBatch:public Batch
{
public:
	SpriteBatch();
	SpriteBatch(GLFWwindow* window,GLint width,GLint height,GLchar* title);
	~SpriteBatch();
	GLvoid begin();
	GLvoid draw(Texture texture,GLfloat x,GLfloat y,GLfloat originX,GLfloat originY,GLfloat width,GLfloat height,GLfloat scaleX,
		GLfloat scaleY,GLfloat rotation,GLint srcX,GLint srcY,GLint srcWidth,GLint srcHeight,GLboolean flipX,GLboolean flipY);
	GLvoid end();
	GLvoid setupMatrices();
	GLvoid switchTexture(Texture texture);
	GLvoid flush();
private:
	GLboolean m_bDrawing;
	
	GLint m_iSize;
	ShaderProgram m_shader;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_modelMatrix;
	glm::mat4 m_MVP;
	GLint m_matrixID;
	GLint m_textureID;
	Texture m_lastTexture;
	GLfloat m_invTexWidth;
	GLfloat m_invTexHeight;
	GLint m_idx;
	Vertex* m_vertices;
	GLint m_iRenderCalls;//Number of render calls since the last begin()
	GLint m_iTotalRenderCalls;//Number of rendering calls 
	GLint m_maxSpritesInBatch;//The maximum number of sprites rendered in one batch so far
	
};