#include <gl/glew.h>
#include <GLFW/glfw3.h>

//===============================================================================//
//==Based class used to create a window and change the object size by window size with orthographic 
class Batch
{
public:
	Batch();
	Batch(GLFWwindow* window,GLint width,GLint height,char* title);
	~Batch();
	GLvoid initiEnvironment();
	GLint getWindowWidth();
	GLint getWindowHeight();
	GLFWwindow* getWindowHandle();
	GLvoid renderScene();
	virtual GLvoid draw();

protected:
	//window handle
	GLFWwindow* m_Window;

	//width and height of window
	GLint m_iWidth;
	GLint m_iHeight;

	//title of window
	char* m_cTitle;
};