#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>


#ifdef _DEBUG 
#define MODE "DEBUG"
#endif

#ifdef _RELEASE 
#define MODE "RELEASE"
#endif



namespace screenSettings
{
    int width = 600;
    int height = 600;
}
enum FRACTAL_OPTIONS { MANDELBROT, JULIA, BOTH };   // Do not change


// Fractal Settings

float position[2] = {0, 0};
float zoom = 1;
float speed = 1;
int iterations = 300;

float variablePosition[2] = {0, 0};

// Choose what fractal types are implemented (MANDELBROT, JULIA, BOTH) - if BOTH, the user will be asked which one they want
#define FRACTAL_OPTION BOTH  

////////////////////////////////////////////////////////////////////////////////////////


// timing 
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f; // time of last frame
int fCounter = 0;

// ids

// Holder variables
int mode = 1;           // Free roam mode: 1    Variable mode: 2
bool variableHold = false;
bool variableBoxHold = false;
char error;
int variable_box_size = std::min(screenSettings::width, screenSettings::height) * 0.1;
int variable_box_on = (variable_box_size >= 20);
int variable_position_in_box_offset[2] = {0, 0};
float temp_variablePosition[2] = {0, 0};
int chosen_fractal;


int main()
{
    std::cout << "\n" << MODE << " mode is enabled\n\n\n";

    if (FRACTAL_OPTION == BOTH)
        {
            do
            {
                std::cout << "Choose a fractal\n\n";
                std::cout << "0 - Mandelbrot Set\n";
                std::cout << "1 - Julia Set\n\n";

                std::cin >> chosen_fractal;

                if (chosen_fractal != 0 && chosen_fractal != 1)
                {                
                    std::cout << chosen_fractal << " is not a valid choice\n---------------------\n\n";
                }       
            } while (chosen_fractal != 0 && chosen_fractal != 1);
            std::cout << "---------------------\n\n";
        }
    else {
        chosen_fractal = FRACTAL_OPTION;
    }

    while (true)
    {

    }

    return 0;
}