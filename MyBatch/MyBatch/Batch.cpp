#include <stdio.h>
#include "Batch.h"

Batch::Batch()
	:m_Window(NULL)
	,m_iWidth(960)
	,m_iHeight(480)
	,m_cTitle("New Window")
{}

//===//
Batch::Batch(GLFWwindow* window,GLint width,GLint height,char* title)
	:m_Window(window)
	,m_iWidth(width)
	,m_iHeight(height)
	,m_cTitle(title)
{
	//renderScene();
}

Batch::~Batch()
{}

GLvoid Batch::renderScene()
{
	initiEnvironment();

	do 
	{
		//Set context color
		glClearColor(1.0f,1.0f,1.0f,1.0f);
		//Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		draw();

		//Swap the front and back buffer
		glfwSwapBuffers(m_Window);

		//Poll events by query
		glfwPollEvents();

	} while (glfwGetKey(m_Window,GLFW_KEY_ESCAPE)!=GLFW_PRESS 
		&& glfwWindowShouldClose(m_Window)==0);

}

GLvoid Batch::initiEnvironment()
{
	//Initialized the GLFW
	if (!glfwInit())
	{
		fprintf(stderr,"Failed to the initialized GLFW.\n");
		return ;
	}

	//Set some hints
	glfwWindowHint(GLFW_SAMPLES,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	m_Window=glfwCreateWindow(m_iWidth,m_iHeight,m_cTitle,NULL,NULL);

	if (m_Window==NULL)//Failed to the create window
	{
		fprintf(stderr,"Failed to the create window.\n");
		return ;
	}

	//Make the context of current window
	glfwMakeContextCurrent(m_Window);

	//Initialized the GLEW
	glewExperimental=GL_TRUE;
	if (glewInit()!=GLEW_OK)//Failed to initialized the GLEW
	{
		fprintf(stderr,"Failed to the initialized the GLEW.\n");
		return ;
	}

	//Set the input mode is key
	glfwSetInputMode(m_Window,GLFW_STICKY_KEYS,GL_TRUE);

}

GLvoid Batch::draw()
{}

GLint Batch::getWindowWidth()
{
	return m_iWidth;
}

GLint Batch::getWindowHeight()
{
	return m_iHeight;
}

GLFWwindow* Batch::getWindowHandle()
{
	return m_Window;
}