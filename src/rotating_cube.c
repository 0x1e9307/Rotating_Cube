#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int WIDTH = 1280;
const int HEIGHT = 720;
const float ROTATION_SPEED = 0.01f;

void renderCube(float angleX, float angleY) {
    glBegin(GL_POINTS);
    
    const int step = 20; // Step for generating points
    for (float x = -1.0f; x <= 1.0f; x += 2.0f / step) {
        for (float y = -1.0f; y <= 1.0f; y += 2.0f / step) {
            for (float z = -1.0f; z <= 1.0f; z += 2.0f / step) {
                float tempX = x * cos(angleY) - z * sin(angleY);
                float tempZ = x * sin(angleY) + z * cos(angleY);
                float rotatedX = tempX;

                float tempY = y * cos(angleX) - tempZ * sin(angleX);
                float rotatedZ = y * sin(angleX) + tempZ * cos(angleX);
                float rotatedY = tempY;

                float color = (x + 1.0f) / 2.0f; // Normalize x for color
                glColor3f(color, (y + 1.0f) / 2.0f, (z + 1.0f) / 2.0f); // Gradient across all axes

                glVertex3f(rotatedX, rotatedY, rotatedZ);
            }
        }
    }

    glEnd();
}

void cleanup(SDL_Window* window, SDL_GLContext context) {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Rotating Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    
    glewInit();
    glEnable(GL_DEPTH_TEST);

    float angleX = 0.0f;
    float angleY = 0.0f;

    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                cleanup(window, context); // Clean up resources
                return 0;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                cleanup(window, context); // Clean up resources on ESC key
                return 0;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
                cleanup(window, context); // Clean up resources on 'q' key
                return 0;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5.0f);
        
        renderCube(angleX, angleY);

        SDL_GL_SwapWindow(window);
        
        angleX += ROTATION_SPEED; // Increase rotation angle
        angleY += ROTATION_SPEED; // Increase rotation angle
    }

    cleanup(window, context); // Clean up resources
    return 0;
}
