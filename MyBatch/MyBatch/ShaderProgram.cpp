#include "ShaderProgram.h"

/*Non-parameter constructor:use the default shader*/
ShaderProgram::ShaderProgram()
	:m_uiProgramID(-1)
	,m_invalidated(false)
	,m_vertexShaderFile("VertexShader.glsl")
	,m_fragmentShaderFile("FragmentShader.glsl")
	,m_isCompiled(false)
	,m_vertexShaderHandle(-1)
	,m_fragmentShaderHandle(-1)
	,m_uniformNames(NULL)
	,m_iUniformLocation(NULL)
	,m_attributeNames(NULL)
	,m_iAttributeLocation(NULL)
	,m_iAttributeSize(0)
	,m_iUniformSize(0)
{
	compileShaders(m_vertexShaderFile,m_fragmentShaderFile);
	if (isCompiled())
	{
		fetchAttributes();
		fetchUniforms();
		addManagedShader();
	}
}

ShaderProgram::ShaderProgram(const ShaderProgram& copyShaderProgram)
{
	m_uiProgramID=copyShaderProgram.m_uiProgramID;
	m_invalidated=copyShaderProgram.m_invalidated;

	m_vertexShaderFile=copyShaderProgram.m_vertexShaderFile;
	m_fragmentShaderFile=copyShaderProgram.m_fragmentShaderFile;

	m_isCompiled=copyShaderProgram.m_isCompiled;

	m_vertexShaderHandle=copyShaderProgram.m_vertexShaderHandle;
	m_fragmentShaderHandle=copyShaderProgram.m_fragmentShaderHandle;

	m_vertexShaderCode=copyShaderProgram.m_vertexShaderCode;
	m_fragmentShaderCode=copyShaderProgram.m_fragmentShaderCode;

	m_uniformNames=copyShaderProgram.m_uniformNames;
	m_iUniformLocation=copyShaderProgram.m_iUniformLocation;

	m_attributeNames=copyShaderProgram.m_attributeNames;
	m_iAttributeLocation=copyShaderProgram.m_iAttributeLocation;

	m_iAttributeSize=copyShaderProgram.m_iAttributeSize;
	m_iUniformSize=copyShaderProgram.m_iUniformSize;
}

/* Parameter constructor: specified the shader file path*/
ShaderProgram::ShaderProgram(const GLchar * vertex_file_path,const GLchar * fragment_file_path)
	:m_uiProgramID(-1)
	,m_invalidated(false)
	,m_vertexShaderFile(vertex_file_path)
	,m_fragmentShaderFile(fragment_file_path)
	,m_isCompiled(false)
	,m_vertexShaderHandle(-1)
	,m_fragmentShaderHandle(-1)
	,m_uniformNames(NULL)
	,m_iUniformLocation(NULL)
	,m_attributeNames(NULL)
	,m_iAttributeLocation(NULL)
{
	compileShaders(m_vertexShaderFile,m_fragmentShaderFile);
	if (isCompiled())
	{
		fetchAttributes();
		fetchUniforms();
		addManagedShader();
	}
}

/*Destruct function:release the memory*/
ShaderProgram::~ShaderProgram()
{
	glDeleteShader(m_vertexShaderHandle);
	glDeleteShader(m_fragmentShaderHandle);
	glDeleteProgram(m_uiProgramID);
}

/*Use this vertex and fragment shader pair*/
GLvoid ShaderProgram::begin()
{
	checkManaged();
	glUseProgram(m_uiProgramID);
}

/*Check the current shader is valid*/
GLvoid ShaderProgram::checkManaged()
{
	if (m_invalidated)
	{
		compileShaders(m_vertexShaderFile,m_fragmentShaderFile);
		m_invalidated=false;
	}
}

/*Compile the shader*/
GLvoid ShaderProgram::compileShaders(const GLchar * vertex_file_path,const GLchar * fragment_file_path)
{
	m_vertexShaderHandle=loadShader(GL_VERTEX_SHADER,vertex_file_path);
	m_fragmentShaderHandle=loadShader(GL_FRAGMENT_SHADER,fragment_file_path);

	if (m_vertexShaderHandle ==-1 || m_fragmentShaderHandle==-1)
	{
		m_isCompiled=false;
		return ;
	}

	char const* vertexSourcePointer=m_vertexShaderCode.c_str();
	glShaderSource(m_vertexShaderHandle,1,&vertexSourcePointer,NULL);
	glCompileShader(m_vertexShaderHandle);

	GLint errorVertex=getLog(m_vertexShaderHandle);

	char const* fragmentSourcePointer=m_fragmentShaderCode.c_str();
	glShaderSource(m_fragmentShaderHandle,1,&fragmentSourcePointer,NULL);
	glCompileShader(m_fragmentShaderHandle);
	GLint errorFragment=getLog(m_fragmentShaderHandle);

	if (errorVertex>1 || errorFragment>1)
	{
		m_isCompiled=false;
		return ;
	}

	linkProgram();

	if(m_uiProgramID==-1)
	{
		m_isCompiled=false;
		return ;
	}

	m_isCompiled=true;

}

/*Load the shader*/
GLint ShaderProgram::loadShader(int type,const GLchar * file_path)
{
	GLuint shaderID=glCreateShader(type);
	std::ifstream shaderStream(file_path,std::ios::in);
	std::string shaderCode;
	if (shaderStream.is_open())
	{
		std::string Line="";
		while(getline(shaderStream,Line))
		{
			shaderCode += "\n" + Line;
		}
		shaderStream.close();
		if (type==GL_VERTEX_SHADER)
		{
			m_vertexShaderCode=shaderCode;
		}
		else if(type==GL_FRAGMENT_SHADER)
		{
			m_fragmentShaderCode=shaderCode;
		}
		return shaderID;
	}
	else
	{
		printf("Failed to open %s. Are you in the right directory ? \n", file_path);
		return -1;
	}
}

/*Get the log*/
GLint ShaderProgram::getLog(int type)
{
	GLint iResult=GL_FALSE;
	GLint iInfoLogLength;

	glGetShaderiv(type,GL_COMPILE_STATUS,&iResult);
	glGetShaderiv(type,GL_INFO_LOG_LENGTH,&iInfoLogLength);
	if (iInfoLogLength>0)
	{
		std::vector<char> ShaderErrorMessage(iInfoLogLength+1);
		glGetShaderInfoLog(type, iInfoLogLength, NULL, &ShaderErrorMessage[0]);
		printf("%s\n", &ShaderErrorMessage[0]);
	}
	return iInfoLogLength;
}

/*Create a new program and links the shader*/
GLvoid ShaderProgram::linkProgram()
{
	m_uiProgramID=glCreateProgram();
	glAttachShader(m_uiProgramID,m_vertexShaderHandle);
	glAttachShader(m_uiProgramID,m_fragmentShaderHandle);
	glLinkProgram(m_uiProgramID);

	GLint errorProgram=getLog(m_uiProgramID);
	if (errorProgram>0)
	{
		m_uiProgramID=-1;
		return ;
	}
	glDetachShader(m_uiProgramID, m_vertexShaderHandle);
	glDetachShader(m_uiProgramID, m_fragmentShaderHandle);
}

/*Get the location of attribute by name*/
GLint ShaderProgram::fetchAttributeLocation(const GLchar * name)
{
	GLint location;
	location=glGetAttribLocation(m_uiProgramID,name);
	if (location==-1)
	{
		printf("No exit %s.Please check.",name);
		exit(1);
	}
	return location;
}

/*Get the location of uniform by name*/
GLint ShaderProgram::fetchUniformLocation(const GLchar * name)
{
	GLint location;
	location=glGetUniformLocation(m_uiProgramID,name);
	if (location==-1)
	{
		printf("No exit %s.Please check.",name);
		exit(1);
	}
	return location;
}

/*Return whether this shader program compiled successfully*/
GLboolean ShaderProgram::isCompiled()
{
	return m_isCompiled;
}

/*Get all the location and name of uniform in shader */
GLvoid ShaderProgram::fetchUniforms()
{
	GLint iParams;
	GLsizei iMaxLength;
	GLint iSize,iWritten;
	GLenum eType;
	GLchar * cName;

	glGetProgramiv(m_uiProgramID,GL_ACTIVE_UNIFORM_MAX_LENGTH,&iMaxLength);
	glGetProgramiv(m_uiProgramID,GL_ACTIVE_UNIFORMS,&iParams);
	m_iUniformSize=iParams;
	cName=new GLchar[iMaxLength];
	m_uniformNames=new std::string[iParams];
	m_iUniformLocation=new GLint[iParams];
	for (int i=0;i<iParams;++i)
	{
		glGetActiveUniform(m_uiProgramID,i,iMaxLength,&iWritten,&iSize,&eType,cName);
		m_iUniformLocation[i]=glGetUniformLocation(m_uiProgramID,cName);
		m_uniformNames[i]=cName;
	}
}

/*Get all the location and name of attribute in shader */
GLvoid ShaderProgram::fetchAttributes()
{
	GLint iParams;
	GLsizei iMaxLength;
	GLint iSize,iWritten;
	GLenum eType;
	GLchar * cName;
	glGetProgramiv(m_uiProgramID,GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,&iMaxLength);
	glGetProgramiv(m_uiProgramID,GL_ACTIVE_ATTRIBUTES,&iParams);
	m_iAttributeSize=iParams;
	cName=new GLchar[iMaxLength];
	m_attributeNames=new std::string[iParams];
	m_iAttributeLocation=new GLint[iParams];
	for(int i=0;i<iParams;++i)
	{
		glGetActiveAttrib(m_uiProgramID,i,iMaxLength,&iWritten,&iSize,&eType,cName);
		m_iAttributeLocation[i]=glGetAttribLocation(m_uiProgramID,cName);
		m_attributeNames[i]=cName;
	}
}

/*Get the attribute name in location*/
std::string ShaderProgram::getAttributeName(GLint location)
{
	return m_attributeNames[location];
}

/*Get the uniform name in location*/
std::string ShaderProgram::getUniformName(GLint location)
{
	return m_uniformNames[location];
}

/*Manage the shader*/
GLvoid ShaderProgram::addManagedShader()
{
	m_shaders.push_back(this);
}

/*Enables the vertex attribute by location */
GLvoid ShaderProgram::enableVertexAttribute(int location)
{
	checkManaged();
	glEnableVertexAttribArray(location);
}

/*Disable the attribute by location */
GLvoid ShaderProgram::disableVertexAttribute(int location)
{
	checkManaged();
	glDisableVertexAttribArray(location);
}

/*Invalidates all shader so the next time they are used new handle are generated*/
GLvoid ShaderProgram::invalidateAllShaderPrograms()
{
	for (unsigned int i=0;i < m_shaders.size();i++)
	{
		m_shaders[i]->m_invalidated=true;
		m_shaders[i]->checkManaged();
	}
}

/*Return all the uniform location*/
GLint* ShaderProgram::getUniformLocation()
{
	return m_iUniformLocation;
}

/*Return the number of uniform size*/
GLint ShaderProgram::getUniformSize()
{
	return m_iUniformSize;
}

/*Return the number of attribute*/
GLint ShaderProgram::getAttributeSize()
{
	return m_iAttributeSize;
}

/*Set the vertex attribute with the given name*/
GLvoid ShaderProgram::setVertexAttribute(GLint location,GLint size,GLint type,GLboolean normalize,GLint stride,const GLvoid * pointer)
{
	checkManaged();
	glVertexAttribPointer(location,size,type,normalize,stride,pointer);
}






