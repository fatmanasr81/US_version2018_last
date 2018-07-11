#include "kernel.h"
 #include <stdio.h>
 #include <stdlib.h>
 #ifdef _WIN32
 #define WINDOWS_LEAN_AND_MEAN
 #define NOMINMAX
#include <windows.h>
 #endif
 #ifdef __APPLE__
 #include <GLUT/glut.h>
 #else
 #include <GL/glew.h>
 #include <GL/freeglut.h>
 #endif
 #include <cuda_runtime.h>
 #include <cuda_gl_interop.h>
#include "interactions.h"
#include "image_reconstract_GPU.h"


// texture and pixel objects
 GLuint pbo = 0;     // OpenGL pixel buffer object
 GLuint tex = 0;     // OpenGL texture object
 struct cudaGraphicsResource *cuda_pbo_resource;

void render() {
	  uchar4 *d_out = 0;
	  //BYTE * FI = 0;
	  short int R = 61;
	  float  pitch = 0.2;
	  float depth = 180;
	  float lens = 0.7;
	  int nLines = 256;
	  int nSamples = 512;

	  image_reconstract_GPU g1;
	  g1.read_bin_file_GPU();
	  g1.filtering_GPU(0);
	  g1.Detection_GPU();
	  g1.Under_sampling_GPU(0);
	  g1.dynamic_GPU(60);
	  g1.scan_converter_GPU(R, pitch, depth, lens, nSamples, nLines);

	   cudaGraphicsMapResources(1, &cuda_pbo_resource, 0);
	   cudaGraphicsResourceGetMappedPointer((void **)&g1.FI, NULL,
		                                       cuda_pbo_resource);
	   
	   g1.interpolation_GPU();

	 //  GPU_construct.interpolation_GPU();

 // kernelLauncher(d_out, W, H, loc);
	  cudaGraphicsUnmapResources(1, &cuda_pbo_resource, 0);

}

void drawTexture() {
	 //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA,
	            //    GL_UNSIGNED_BYTE, NULL);

	   glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, W, H, 0, GL_LUMINANCE,
		   GL_UNSIGNED_BYTE, NULL);
    glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
     glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
      glTexCoord2f(0.0f, 1.0f); glVertex2f(0, H);
	  glTexCoord2f(1.0f, 1.0f); glVertex2f(W, H);
     glTexCoord2f(1.0f, 0.0f); glVertex2f(W, 0);
	  glEnd();
	   glDisable(GL_TEXTURE_2D);
	
}

 void display() {
   render();
	  drawTexture();
   glutSwapBuffers();
	
}

void initGLUT(int *argc, char **argv) {
	  //glutInit(argc, argv);
	  char *myargv[1];
	  int myargc = 1;
	  myargv[0] = strdup("Myappname");
	  glutInit(&myargc, myargv);

	   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize(W, H);
  glutCreateWindow(TITLE_STRING);
 #ifndef __APPLE__
		   glewInit();
	 #endif
		
}

void initPixelBuffer() {
	  glGenBuffers(1, &pbo);
	   glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	   glBufferData(GL_PIXEL_UNPACK_BUFFER, W*H * sizeof(GLubyte), 0,
		   GL_STREAM_DRAW);

	 glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	 cudaGraphicsGLRegisterBuffer(&cuda_pbo_resource, pbo,
	                                cudaGraphicsMapFlagsWriteDiscard);
	
}

 void exitfunc() {
	  if (pbo) {
	    cudaGraphicsUnregisterResource(cuda_pbo_resource);
		    glDeleteBuffers(1, &pbo);
		    glDeleteTextures(1, &tex);
		
	}
	
}

 int main(int argc, char** argv) {
	  
	 initGLUT(&argc, argv);
      gluOrtho2D(0, W, H, 0);
	  glutDisplayFunc(display);
	  
   initPixelBuffer();
	 glutMainLoop();
	   atexit(exitfunc);
	   return 0;
	
}