#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

//===============================================================================//
//==Texture class used to load texture data
class Texture
{
public:
	Texture(const GLchar * texture_file_path);
	Texture(const Texture& copyTexture);
	~Texture();
	GLvoid loadTex(const GLchar * texture_file_path);
	GLuint getTextureID();
	GLint getTexHeight();
	GLint getTexWidth();
	GLvoid addManageTexture();
	GLvoid clearAllTextures();
	GLint getNumManagedTextures();
	GLvoid bind();
	friend GLboolean operator !=(const Texture& aTexture,const Texture& bTexture);
private:
	GLuint m_iTexture;
	GLint m_iTexWidth;
	GLint m_iTexHeight;
	std::vector<Texture*> m_textures;
	const GLchar * m_cFilePath;
};