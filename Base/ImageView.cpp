/*
 *  ImageView.cpp
 *  LusionEngine
 *
 *  Created by Erik Engheim on 11.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#include "Base/ImageView.h"
#include "Utils/PolygonUtils.h"
#include "Utils/GLUtils.h"

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

using namespace std;

static const int gMaxNoViews = 100;
static int gNextTexture = 0;
static GLuint gTextures[gMaxNoViews];

static GLuint nextTexture()
{
 if (gNextTexture < gMaxNoViews)
   return gTextures[gNextTexture++];
 cerr << "Too many views!" << endl;
 return gTextures[0];
}

// Assume image has been locked before calling this
static void dumpPixelFormat(SDL_PixelFormat* img_format)
{
 cout << "BitsPerPixel " << (int)img_format->BitsPerPixel << endl;
 cout << "BytesPerPixel " << (int)img_format->BytesPerPixel << endl;
 cout << "colorkey " << (int)img_format->colorkey << endl;
 cout << "alpha " << (int)img_format->alpha << endl;

 cout << "Rshift " << (int)img_format->Rshift << endl;        
 cout << "Gshift " << (int)img_format->Gshift << endl;        
 cout << "Bshift " << (int)img_format->Bshift << endl;        
 cout << "Ashift " << (int)img_format->Ashift << endl;  
 cout << "Rloss " << (int)img_format->Rloss << endl;        
 cout << "Gloss " << (int)img_format->Gloss << endl;        
 cout << "Bloss " << (int)img_format->Bloss << endl;        
 cout << "Aloss " << (int)img_format->Aloss << endl;        

 cout << "Rmask " << hex << (int)img_format->Rmask << endl;        
 cout << "Gmask " << hex << (int)img_format->Gmask << endl;                  
 cout << "Bmask " << hex << (int)img_format->Bmask << endl;        
 cout << "Amask " << hex << (int)img_format->Amask << endl << dec;                    
}

void initTextures() 
{
  glGenTextures( gMaxNoViews, gTextures+0 );   // Create the texture        
}

/** Loads an image an puts it into a a 2D texture */
GLuint loadTexture(const char* filename)
{
  SDL_Surface* img;
  img = IMG_Load(filename);
  if (img == 0) {
    cerr << "Unable to load image: " << SDL_GetError() << endl;
    return 0;
  }
  GLuint texture = nextTexture();
  GLenum format = GL_BGR;
  
  SDL_LockSurface(img);
  
  SDL_PixelFormat* img_format = img->format;
  // dumpPixelFormat(img_format);
  if (img_format->BitsPerPixel == 32) {
    format = GL_RGBA;
  }
  
  glBindTexture( GL_TEXTURE_2D, texture );
  glTexImage2D( GL_TEXTURE_2D, 0, format, img->w,
                img->h, 0, format,
                GL_UNSIGNED_BYTE, img->pixels );
  // Linear filtering
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  
  SDL_UnlockSurface(img);  
  SDL_FreeSurface(img);

  return texture;
}

ImageView::ImageView(const StringList& filenames, real w, real h) 
{
  init(filenames, w, h);
}

ImageView::ImageView(const StringList& filenames, const Polygon2& poly, real w, real h) 
{
  init(filenames, w, h);
  setCollisionPolygon(poly.begin(), poly.end());  
}

void ImageView::init(const StringList& filenames, real width, real height)
{
  StringList::const_iterator i;
  for (i = filenames.begin(); i != filenames.end(); ++i) {
    iTextures.push_back(loadTexture(i->c_str()));
  }
  
  real w = width*0.5;
  real h = height*0.5;
  
  iBottomLeft = Point2(-w, -h);
  iBottomRight = Point2( w, -h);
  iTopRight = Point2( w,  h);
  iTopLeft = Point2(-w,  h);  
  
  Point2 points[] = {iBottomLeft, iBottomRight, iTopRight, iTopLeft};    
  Polygon2 poly(points, points+4);
  setCollisionPolygon(poly.begin(), poly.end());    
}

ImageView::~ImageView()
{

}

// Calculations
void ImageView::draw(int image_index) const
{      
  assert(image_index < iTextures.size());
  glBindTexture( GL_TEXTURE_2D, iTextures[image_index]);   // Select our texture  
  glBegin(GL_QUADS);
      glTexCoord2d(0.0, 1.0); 
      gltVertex(iBottomLeft);
      glTexCoord2d(1.0, 1.0);
      gltVertex(iBottomRight);        
      glTexCoord2d(1.0, 0.0); 
      gltVertex(iTopRight);
      glTexCoord2d(0.0, 0.0);
      gltVertex(iTopLeft);                
  glEnd();    
}
