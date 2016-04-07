#include "SpriteBatch.h"

SpriteBatch::SpriteBatch()
	:m_bDrawing(false)
	,m_iRenderCalls(0)
	,m_iTotalRenderCalls(0)
	,m_iSize(1000)
	,m_textureID(-1)
	,m_matrixID(-1)
	,m_lastTexture(NULL)
	,m_invTexWidth(0)
	,m_invTexHeight(0)
	,m_idx(0)
	,m_maxSpritesInBatch(0)
{
	// 32767/5-(32767/5%2)=6552
	if (m_iSize>6552)
	{
		printf("Can't have more than 6552 sprites per batch:%d",m_iSize);
		exit(1);
	}

	//Set the view por0
	glViewport(0,0,getWindowWidth(),getWindowHeight());
	m_projectionMatrix=glm::ortho(0,getWindowWidth(),0,getWindowHeight(),0,1);
	m_viewMatrix = glm::lookAt(glm::vec3(4,3,3),glm::vec3(0,0,0),glm::vec3(0,1,0));
	m_modelMatrix = glm::mat4(1.0f);

	m_vertices=new Vertex[m_iSize*4];

	//Create a default shader
	ShaderProgram defaultShader;
	m_shader=defaultShader;
}

//===//
SpriteBatch::SpriteBatch(GLFWwindow* window,GLint width,GLint height,GLchar* title)
	:Batch(window,width,height,title)
	,m_bDrawing(false)
	,m_iRenderCalls(0)
	,m_iTotalRenderCalls(0)
	,m_iSize(1000)
	,m_textureID(-1)
	,m_matrixID(-1)
	,m_lastTexture(NULL)
	,m_invTexWidth(0)
	,m_invTexHeight(0)
	,m_idx(0)
	,m_maxSpritesInBatch(0)
{
	// 32767/5-(32767/5%2)=6552
	if (m_iSize>6552)
	{
		printf("Can't have more than 6552 sprites per batch:%d",m_iSize);
		exit(1);
	}

	//Set the view por0
	glViewport(0,0,width,height);
	m_projectionMatrix=glm::ortho(0,width,0,height,0,1);
	m_viewMatrix = glm::lookAt(glm::vec3(4,3,3),glm::vec3(0,0,0),glm::vec3(0,1,0));
	m_modelMatrix = glm::mat4(1.0f);

	m_vertices=new Vertex[m_iSize*4];

	//Create a default shader
	ShaderProgram defaultShader();
	m_shader=defaultShader();
}

SpriteBatch::~SpriteBatch()
{}

/*Set up the SpriteBath for drawing*/
GLvoid SpriteBatch::begin()
{
	if (m_bDrawing)
	{
		printf("Please call batch.begin firstly.\n");
		return ;
	}

	m_shader.begin();

	setupMatrices();

	m_bDrawing=true;

}

/*Finishes off rendering*/
GLvoid SpriteBatch::end()
{
	m_bDrawing=false;

	if (m_idx>0)
	{
		flush();
	}
}

/*Draw a rectangle with the bottom left corner at x,y having the given width and height*/
GLvoid SpriteBatch::draw(Texture texture,GLfloat x,GLfloat y,GLfloat originX,GLfloat originY,GLfloat width,GLfloat height,GLfloat scaleX,
	GLfloat scaleY,GLfloat rotation,GLint srcX,GLint srcY,GLint srcWidth,GLint srcHeight,GLboolean flipX,GLboolean flipY)
{
	if (!m_bDrawing)
	{
		printf("Please call batch.begin firstly.\n");
		return ;
	}

	if (texture!=m_lastTexture)
	{
		switchTexture(texture);
	}
	else if (m_idx == (m_iSize*4))
	{
		flush();
	}

	//Bottom left and top right corner points relative to origin
	GLfloat worldOriginX=x+originX;
	GLfloat worldOriginY=y+originY;
	GLfloat fx=-originX;
	GLfloat fy=-originY;
	GLfloat fx2=width-originX;
	GLfloat fy2=height-originY;

	//Scale
	if(scaleX!=1 || scaleY!=1)
	{
		fx *= scaleX;
		fy *= scaleY;
		fx2*= scaleX;
		fy2*= scaleY;
	}

	//Construct corner points,start from top left and go counter clockwise
	GLfloat p1x=fx;
	GLfloat p1y=fy;
	GLfloat p2x=fx;
	GLfloat p2y=fy2;
	GLfloat p3x=fx2;
	GLfloat p3y=fy2;
	GLfloat p4x=fx2;
	GLfloat p4y=fy;

	GLfloat x1,y1,x2,y2,x3,y3,x4,y4;
	//Rotate
	if (rotation!=0)
	{
		GLfloat fcos=cos(rotation);
		GLfloat fsin=sin(rotation);

		x1= fcos * p1x - fsin * p1y;
		y1= fsin * p1x + fcos * p1y;

		x2= fcos * p2x - fsin * p2y;
		y2= fsin * p2x + fcos * p2y;

		x3= fcos * p3x - fsin * p3y;
		y3= fsin * p3x + fcos * p3y;

		x4= x1 + (x3-x2);
		y4= y3 - (y2-y1);

	}
	else
	{
		x1=p1x;
		y1=p1y;

		x2=p2x;
		y2=p2y;

		x3=p3x;
		y3=p3y;

		x4=p4x;
		y4=p4y;
	}

	x1 += worldOriginX;
	y1 += worldOriginY;

	x2 += worldOriginX;
	y2 += worldOriginY;

	x3 += worldOriginX;
	y3 += worldOriginY;

	x4 += worldOriginX;
	y4 += worldOriginY;

	GLfloat u=srcX*m_invTexWidth;
	GLfloat v=(srcY+srcHeight)*m_invTexHeight;
	GLfloat u2=(srcX+srcWidth)*m_invTexWidth;
	GLfloat v2=srcY*m_invTexHeight;

	if(flipX)
	{
		GLfloat tmp=u;
		u=u2;
		u2=tmp;
	}

	if(flipY)
	{
		GLfloat tmp=v;
		v=v2;
		v2=tmp;
	}

	m_vertices[m_idx].x=x1;
	m_vertices[m_idx].y=y1;
	m_vertices[m_idx].z=0;
	m_vertices[m_idx].u=u;
	m_vertices[m_idx].v=v;

	++m_idx;

	m_vertices[m_idx].x=x2;
	m_vertices[m_idx].y=y2;
	m_vertices[m_idx].z=0;
	m_vertices[m_idx].u=u;
	m_vertices[m_idx].v=v2;

	++m_idx;

	m_vertices[m_idx].x=x3;
	m_vertices[m_idx].y=y3;
	m_vertices[m_idx].z=0;
	m_vertices[m_idx].u=u2;
	m_vertices[m_idx].v=v2;

	++m_idx;

	m_vertices[m_idx].x=x4;
	m_vertices[m_idx].y=y4;
	m_vertices[m_idx].z=0;
	m_vertices[m_idx].u=u2;
	m_vertices[m_idx].v=v;

	++m_idx;
}

/*Set up the MVP by multiply projection , view and model matrix */
GLvoid SpriteBatch::setupMatrices()
{
	m_MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
	m_matrixID=m_shader.fetchUniformLocation("MVP");
	glUniformMatrix4fv(m_matrixID, 1, GL_FALSE, &m_MVP[0][0]);
	m_textureID=m_shader.fetchUniformLocation("myTextureSampler");
	glUniform1i(m_textureID, 0);
}

/*Switch the texture,set up the last texture*/
GLvoid SpriteBatch::switchTexture(Texture texture)
{
	flush();
	m_lastTexture=texture;
	m_invTexWidth=1.0f/(texture.getTexWidth());
	m_invTexHeight=1.0f/(texture.getTexHeight());
}

/*Causes any pending sprites to be rendered*/
GLvoid SpriteBatch::flush()
{
	if (m_idx==0)
	{
		return ;
	}

	m_iRenderCalls++;
	m_iTotalRenderCalls++;
	GLint spritesInBatch=m_idx/4;

	if (spritesInBatch>m_maxSpritesInBatch)
	{
		m_maxSpritesInBatch=spritesInBatch;
	}

	m_lastTexture.bind();

	Mesh mesh(m_vertices);

	mesh.render(m_shader);

	m_idx=0;
}

