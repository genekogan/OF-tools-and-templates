/*
 Alpha/Luminance mask adapted from ofxMask
*/

#pragma once

#include "ofFbo.h"
#include "ofShader.h"
#include "ofGraphics.h"

namespace {
    void makeVertices(float *dst, ofTextureData& texture_data)
    {
        dst[0] =
        dst[1] =
        dst[3] =
        dst[6] = 0;
        dst[2] =
        dst[4] = texture_data.width;
        dst[5] =
        dst[7] = texture_data.height;
    }
    void makeTexCoords(float *dst, ofTextureData& texture_data)
    {
        dst[0] =
        dst[1] =
        dst[3] =
        dst[6] = 0;
#ifndef TARGET_OPENGLES
        if( texture_data.textureTarget==GL_TEXTURE_RECTANGLE_ARB && ofGLSupportsNPOTTextures() ){
            dst[2] =
            dst[4] = texture_data.width;
            dst[5] =
            dst[7] = texture_data.height;
        }else
#endif
        {
            dst[2] =
            dst[4] = texture_data.tex_t;
            dst[5] =
            dst[7] = texture_data.tex_u;
        }
    }
}

class KMask
{
public:
	enum Type
	{
		ALPHA,
		LUMINANCE
	};
	void setup(int width, int height, Type type)
    {
        this->width = width;
        this->height = height;
        
#define _S(a) #a
        switch(type) {
            case ALPHA: {
                masker_.allocate(width, height, GL_RGBA);
                string shader_src = _S(
                                       uniform sampler2DRect masker;
                                       uniform sampler2DRect maskee;
                                       void main()
                                       {
                                           gl_FragColor = texture2DRect(maskee, gl_TexCoord[0].st);
                                           gl_FragColor.a *= texture2DRect(masker, gl_TexCoord[0].st).a;
                                           gl_FragColor.a = sqrt(gl_FragColor.a);
                                       }
                                       );
                shader_.setupShaderFromSource(GL_FRAGMENT_SHADER, shader_src);
                shader_.linkProgram();
            }	break;
            case LUMINANCE: {
                masker_.allocate(width, height, GL_RGB);
                string shader_src = _S(
                                       uniform sampler2DRect masker;
                                       uniform sampler2DRect maskee;
                                       void main()
                                       {
                                           gl_FragColor = texture2DRect(maskee, gl_TexCoord[0].st);
                                           vec4 rgb = texture2DRect(masker, gl_TexCoord[0].st);
                                           gl_FragColor.a *= 0.298912*rgb.r + 0.586611*rgb.g + 0.114478*rgb.b;
                                           gl_FragColor.a = sqrt(gl_FragColor.a);
                                       }
                                       );
                shader_.setupShaderFromSource(GL_FRAGMENT_SHADER, shader_src);
                shader_.linkProgram();
            }	break;
        }
#undef _S
        maskee_.allocate(width, height, GL_RGBA);
        makeTexCoords(tex_coords_, masker_.getTextureReference().getTextureData());
        makeVertices(vertices_, masker_.getTextureReference().getTextureData());
    }
    
    void beginMask(bool clear=true)
    {
        masker_.begin();
        if(clear) {
            ofClear(0);
        }
    }
    
    void endMask()
    {
        masker_.end();
    }
    
    void clearMask()
    {
        masker_.begin();
        ofClear(0);
        masker_.end();
    }
    
    void begin(bool clear=true)
    {
        maskee_.begin();
        if(clear) {
            ofClear(0);
        }
    }
    
    void end()
    {
        maskee_.end();
    }
    
    int getWidth() { return width; }
    int getHeight() { return height; }
    
    void draw()
    {
        shader_.begin();
        shader_.setUniformTexture("masker", masker_, 0);
        shader_.setUniformTexture("maskee", maskee_, 1);
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        glTexCoordPointer(2, GL_FLOAT, 0, tex_coords_ );
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices_ );
        glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        shader_.end();
    }
    
    void drawMasker()
    {
        masker_.draw(0,0,masker_.getWidth(),masker_.getHeight());
    }
    void drawMaskee()
    {
        maskee_.draw(0,0,maskee_.getWidth(),maskee_.getHeight());
    }
    
    
private:
	ofFbo masker_, maskee_;
	ofShader shader_;
	float vertices_[8];
	float tex_coords_[8];
    int width, height;
};