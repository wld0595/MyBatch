#include "SpriteBatch.h"
int main()
{
	SpriteBatch spriteBatch;
	Texture texture("apple.jpg");

	spriteBatch.begin();
	spriteBatch.draw(texture,0,0,0,0,texture.getTexWidth(),texture.getTexHeight(),1,1,0,0,0,texture.getTexWidth(),texture.getTexHeight(),false,false);
	spriteBatch.end();

	return 0;
}