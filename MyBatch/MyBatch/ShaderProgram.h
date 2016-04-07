#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <array>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

//===============================================================================//
//==Shader class used to load shader  
class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const ShaderProgram& copyShaderProgram);
	ShaderProgram(const GLchar * vertex_file_path,const GLchar * fragment_file_path);
	~ShaderProgram();
	GLvoid begin();
	GLvoid checkManaged();
	GLvoid compileShaders(const GLchar * vertex_file_path,const GLchar * fragment_file_path);
	int loadShader(int type,const GLchar * file_path);
	GLint getLog(int type);
	GLvoid linkProgram();
	GLint fetchUniformLocation(const GLchar * name);
	GLint fetchAttributeLocation(const GLchar * name);
	GLboolean isCompiled();
	GLvoid fetchUniforms();
	GLvoid fetchAttributes();
	GLvoid addManagedShader();
	GLvoid enableVertexAttribute(GLint location);
	GLvoid disableVertexAttribute(GLint location);
	GLvoid invalidateAllShaderPrograms();
	GLint * getUniformLocation();
	GLint getAttributeSize();
	GLint getUniformSize();
	std::string getAttributeName(GLint location);
	std::string getUniformName(GLint location);
	GLvoid setVertexAttribute(GLint location,GLint size,GLint type,GLboolean normalize,GLint stride,const GLvoid * pointer);

private:
	//program handle
	GLuint m_uiProgramID;

	//whether this shader was invalidated
	GLboolean m_invalidated;

	//file path of the vertex and fragment shade 
	const GLchar * m_vertexShaderFile;
	const GLchar * m_fragmentShaderFile;

	//whether this program compiled successfully
	GLboolean m_isCompiled;

	//vertex and fragment handle
	GLint m_vertexShaderHandle;
	GLint m_fragmentShaderHandle;

	//vertex and fragment shader source code
	std::string m_vertexShaderCode;
	std::string m_fragmentShaderCode;

	//all the uniform name and location
	std::string *m_uniformNames;
	GLint *m_iUniformLocation;

	//all the attribute name and location
	std::string *m_attributeNames;
	GLint *m_iAttributeLocation;

	//manage the shader
	std::vector<ShaderProgram*> m_shaders;

	//attribute size
	GLint m_iAttributeSize;

	//uniform size
	GLint m_iUniformSize;
};