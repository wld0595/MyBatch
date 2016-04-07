#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Texture.h"

//===============================================================================//
//Texture class

//===Construction function:initialized the member variable
Texture::Texture(const GLchar* texture_file_path)
	:m_iTexture(0)
	,m_iTexWidth(0)
	,m_iTexHeight(0)
	,m_cFilePath(texture_file_path)
{
	loadTex(m_cFilePath);
	addManageTexture();
}

Texture::Texture(const Texture& copyTexture)
{
	m_iTexHeight=copyTexture.m_iTexHeight;
	m_iTexWidth=copyTexture.m_iTexWidth;
	m_iTexture=copyTexture.m_iTexture;
	m_cFilePath=copyTexture.m_cFilePath;
}

//===Destruct function:release the memory
Texture::~Texture()
{
	glDeleteTextures(1,&m_iTexture);
}

//===Load the texture file
GLvoid Texture::loadTex(const GLchar * texture_file_path)
{
	//===Load the texture file 
	GLint iChannel=0;
	FILE* pFile=fopen(texture_file_path,"rb");
	const unsigned char* pImageData=stbi_load_from_file(pFile,&m_iTexWidth,&m_iTexHeight,&iChannel,0);

	//====//check the texture picture 
	if (pImageData==NULL)
	{
		fprintf(stderr,"The %s texture picture is null.Please check.\n",texture_file_path);
		getchar();
		return ;
	}

	//Generate the texture id
	glGenTextures(1,&m_iTexture);
	glBindTexture(GL_TEXTURE_2D,m_iTexture);
	//====//
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	//===//check the texture id
	if (glIsTexture(m_iTexture)==GL_FALSE)
	{
		fprintf(stderr,"Is not currently the name of texture.Or some error occurs.Please check.\n");
		getchar();
		return ;
	}

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_iTexWidth,m_iTexHeight,0,GL_RGB,GL_UNSIGNED_BYTE,pImageData);
	//Release the texture picture buffer
	free((void *)pImageData);
	pImageData=NULL;
	//Set the texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

//Binds the texture to the given texture unit.
GLvoid Texture::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_iTexture);
}

//===Get the iNum-th texture id
GLuint Texture::getTextureID()
{
	return m_iTexture;
}

/*Return the width of the texture*/
GLint Texture::getTexWidth()
{
	return m_iTexWidth;
}

/*Return the height of the texture*/
GLint Texture::getTexHeight()
{
	return m_iTexHeight;
}

/*Manage the texture*/
GLvoid Texture::addManageTexture()
{
	m_textures.push_back(this);
}

/*Clears all managed textures.This is an internal method.Do not use it!*/
GLvoid Texture::clearAllTextures()
{
	m_textures.clear();
}

GLint Texture::getNumManagedTextures()
{
	return m_textures.size();
}

GLboolean operator != (const Texture& aTexture,const Texture& bTexture)
{
	if ((aTexture.m_cFilePath != bTexture.m_cFilePath) && (aTexture.m_iTexture != bTexture.m_iTexture)
		&& (aTexture.m_iTexWidth != bTexture.m_iTexWidth) && (aTexture.m_iTexHeight != bTexture.m_iTexHeight))
	{
		return GL_TRUE;
	}
	else
	{
		return GL_FALSE;
	}
}
