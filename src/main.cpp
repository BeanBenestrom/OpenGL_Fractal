#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "utility.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"


#ifdef _DEBUG 
#define MODE "DEBUG"
#endif

#ifdef _RELEASE 
#define MODE "RELEASE"
#endif

#define GLFW_CLOSE glfwDestroyWindow(window); glfwTerminate(); return 0



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
GLint _width;
GLint _height;
GLint _variable_position;
GLint _mode;
GLint _variable_on_screen;
GLint _variable_box_size;
GLint _variable_box_on;
GLint _variable_position_in_box_offset;

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


void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: !!"; break;    // High
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: !"; break;     // Medium
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: *"; break;     // Low
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: @"; break;     // Notification
    } std::cout << std::endl;
    std::cout << std::endl;
}


void screen_to_world(float container[2], int x, int y)
{
    container[0] = (x - screenSettings::width  * 0.5f) * (5.0f / screenSettings::width * zoom) + position[0];
	container[1] = (screenSettings::height * 0.5f - y) * (5.0f / screenSettings::width * zoom) + position[1];
}


void world_to_screen(int container[2], float x, float y)
{
    container[0] = (x - position[0]) * (screenSettings::width / (5.0f * zoom)) + screenSettings::width * 0.5f;
	container[1] = (position[1] - y) * (screenSettings::width / (5.0f * zoom)) + screenSettings::height * 0.5f;
}


void processInput(GLFWwindow *window, double deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { position[1] += speed * zoom * deltaTime; }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { position[1] -= speed * zoom * deltaTime; }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { position[0] -= speed * zoom * deltaTime; }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { position[0] += speed * zoom * deltaTime; }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { zoom = zoom * pow(0.6, (float)deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { zoom = zoom * pow(1.4, (float)deltaTime); }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) 
    {  
        position[0] = 0;
        position[1] = 0;
        zoom = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) 
    { 
        variableHold = false; 
        variableBoxHold = false;
        mode = 1; 
        glUniform1i(_mode, mode);
        variable_position_in_box_offset[0] = 0; variable_position_in_box_offset[1] = 0;
        glUniform2f(_variable_position_in_box_offset, variable_position_in_box_offset[0], variable_position_in_box_offset[2]);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) 
    { 
        mode = 2; 
        glUniform1i(_mode, mode);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double cursor_x, cursor_y;
    float container[2];
    glfwGetCursorPos(window, &cursor_x, &cursor_y);
    screen_to_world(container, (int)cursor_x, (int)cursor_y);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        int variable_position_in_world[2];
        int mouse_near_box_variable_x = cursor_x - screenSettings::width + variable_box_size / 2 + 30;
        int mouse_near_box_variable_y = cursor_y - variable_box_size / 2 - 30; 
        world_to_screen(variable_position_in_world, variablePosition[0], variablePosition[1]);

        if (!variableBoxHold && mode == 2 && 
            (-3 < mouse_near_box_variable_x && mouse_near_box_variable_x < 3) &&  
            (-3 < mouse_near_box_variable_y && mouse_near_box_variable_y < 3)
        )
        {
            variableBoxHold = true;
            variableHold = false;
            temp_variablePosition[0] = variablePosition[0]; temp_variablePosition[1] = variablePosition[1];
        }
        else if (
            !variableHold && mode == 2 &&
            (-5 < cursor_x - variable_position_in_world[0] && cursor_x - variable_position_in_world[0] < 5) &&  
            (-5 < cursor_y - variable_position_in_world[1] && cursor_y - variable_position_in_world[1] < 5)  
            )
        {
            variableHold = true;
            variableBoxHold = false;
            variable_position_in_box_offset[0] = 0; variable_position_in_box_offset[1] = 0;
            glUniform2f(_variable_position_in_box_offset, variable_position_in_box_offset[0], variable_position_in_box_offset[2]);
        } else {
            variableHold = false;
            variableBoxHold = false;
            variable_position_in_box_offset[0] = 0; variable_position_in_box_offset[1] = 0;
            glUniform2f(_variable_position_in_box_offset, variable_position_in_box_offset[0], variable_position_in_box_offset[2]);
        }
        std::cout << variableHold << "\n";
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) 
    {      
        position[0] = container[0]; position[1] = container[1];
    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) 
    {
        std::cout << "\nMouse position: " << cursor_x << " " << cursor_y << "\n";
        std::cout << "Position at mouse position: " << container[0] << " " << container[1] << "\n";
        std::cout << "Zoom: " << zoom << "\n\n";
    }   
}


void mouse_movement(GLFWwindow* window, double xPos, double yPos)
{
    if (variableBoxHold)
    {
        variable_position_in_box_offset[0] = (float)xPos - screenSettings::width + variable_box_size / 2 + 30;
        variable_position_in_box_offset[1] = (float)yPos - variable_box_size / 2 - 30;
        // std::cout << variable_position_in_box_offset[0] << " " << variable_position_in_box_offset[1] << "\n";

        variablePosition[0] = temp_variablePosition[0] + variable_position_in_box_offset[0] * (5.0f / screenSettings::width * zoom) * 0.1;
        variablePosition[1] = temp_variablePosition[1] + variable_position_in_box_offset[1] * (5.0f / screenSettings::width * zoom) * 0.1;

        glUniform2f(_variable_position_in_box_offset, variable_position_in_box_offset[0], variable_position_in_box_offset[1]);
        glUniform2f(_variable_position, variablePosition[0], variablePosition[1]);
    }
    else if (variableHold)
    {
        float container[2];
        screen_to_world(container, (int)xPos, (int)yPos);

        variablePosition[0] = container[0]; variablePosition[1] = container[1];
        glUniform2f(_variable_position, variablePosition[0], variablePosition[1]);

        // std::cout << variablePosition[0] << " " << variablePosition[1] << "\n";
    }
}



void window_size_callback(GLFWwindow* window, int width, int height)
{
    screenSettings::width = width;
    screenSettings::height = height;
    variable_box_size = std::min(width, height) * 0.1;
    variable_box_on = (variable_box_size >= 20);

    glViewport(0, 0, width, height);

    glUniform1f(_width, (float)width);
    glUniform1f(_height, (float)height);
    glUniform1i(_variable_box_size, variable_box_size);
    glUniform1i(_variable_box_on, variable_box_on);
}


GLuint compile_shader(const GLchar* shaderSource, GLenum shaderType)
{
    // Create shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // Check compilation failure
    GLint compilation_success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_success);
    if (compilation_success != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shader, 1024, &log_length, message);
        
        std::cout << "[!] OPENGL - " << (shaderType == GL_VERTEX_SHADER ? "VERTEX SHADER" : "FRAGMENT SHADER" );
        std::cout << " compilation error:\n" << message << "\n";
        
        return GL_NONE;
    }

    return shader;
}


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

    // Setup GLFW
    if (glfwInit() == GLFW_FALSE) { 
        std::cout << "[!] GLFW failed to intialize!\n";
        std::cin >> error;
        return 0; 
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    #ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    #endif


    // Make window
    GLFWwindow *window = glfwCreateWindow(screenSettings::width, screenSettings::height, "Learning Open GL COMPUTE", NULL, NULL);
    if (!window) {  
        std::cout << "[!] GLFW window failed to be created!\n";
        glfwTerminate();
        std::cin >> error;
        return 0;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, 800, 100);
    glfwSwapInterval(0);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_movement);

    // Setup Glad
    gladLoadGL();
    glViewport(0, 0, screenSettings::width, screenSettings::height);
    std::cout << glGetString(GL_VERSION) << "\n\n";

    #ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    #endif

    {
        int work_group_count[3];

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_group_count[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_group_count[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_group_count[2]);

        int work_group_size[3];

        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_group_size[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_group_size[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_group_size[2]);

        int work_group_invocations;

        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_group_invocations);

        // std::cout << "MAX work group count: " << work_group_count[0] << " " << work_group_count[1] << " " << work_group_count[2];
        // std::cout << "\nMAX work group size: " << work_group_size[0] << " " << work_group_size[1] << " " << work_group_size[2];
        // std::cout << "\nMAX work group invocations: " << work_group_invocations << "\n\n";

        int s1, s2;
        std::string vertexShaderSource, fragmentShaderSource;
        s1 = utility::load_text_from_file(vertexShaderSource, "shaders/vertexShader.vs");
        s2 = utility::load_text_from_file(fragmentShaderSource, "shaders/fractalFragmentShader.fs");
        if (!s1) { 
            std::cout << "[!] UTILITY - Could not load VERTEX SHADER from source file!\n    PATH: shaders/vertexShader.vs\n";
            std::cin >> error;
            GLFW_CLOSE;
        }
        if (!s2) { 
            std::cout << "[!] UTILITY - Could not load FRAGMENT SHADER from source file!\n    PATH: shaders/fractalFragmentShader.fs\n";
            std::cin >> error;
            GLFW_CLOSE;
        }
        
        const GLchar* vS = vertexShaderSource.c_str();
        const GLchar* fS = fragmentShaderSource.c_str();
        

        // REGULAR SHADER
        GLuint vertexShader = compile_shader(vS, GL_VERTEX_SHADER);
        GLuint fragmentShader = compile_shader(fS, GL_FRAGMENT_SHADER);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        // Check for linking failure
        GLint link_success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &link_success);
        if (link_success != GL_TRUE)
        {
            GLsizei log_length = 0;
            GLchar message[1024];
            glGetProgramInfoLog(shaderProgram, 1024, &log_length, message);            
            std::cout << "[!] OPENGL - Program linking error:\n" << message << "\n";
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


        // Objects
        const GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        };

        const GLuint indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        VAO vertexArray;
        VBO vertexBuffer(vertices, sizeof(vertices));
        EBO elementBuffer(indices, sizeof(indices));
        vertexArray.link_atribute(vertexBuffer, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);
        vertexArray.link_atribute(vertexBuffer, 2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
        vertexArray.link_EBO(elementBuffer);
    
        GLint _position = glGetUniformLocation(shaderProgram, "position");
        GLint _zoom = glGetUniformLocation(shaderProgram, "zoom");
        GLint _iterations = glGetUniformLocation(shaderProgram, "iterations");

        _variable_position = glGetUniformLocation(shaderProgram, "variable_position");
        _variable_on_screen = glGetUniformLocation(shaderProgram, "variable_on_screen");
        _variable_box_size = glGetUniformLocation(shaderProgram, "variable_box_size");
        _variable_box_on = glGetUniformLocation(shaderProgram, "variable_box_on");
        _mode = glGetUniformLocation(shaderProgram, "mode");
        _width = glGetUniformLocation(shaderProgram, "window_width");
        _height = glGetUniformLocation(shaderProgram, "window_height");
        _variable_position_in_box_offset = glGetUniformLocation(shaderProgram, "variable_position_in_box_offset");

        glUseProgram(shaderProgram);
        glUniform1i(_variable_box_size, variable_box_size);
        glUniform1i(_variable_box_on, variable_box_on);
        glUniform1f(_width, (float)screenSettings::width);
        glUniform1f(_height, (float)screenSettings::height);
        glUniform2f(_variable_position_in_box_offset, 0.0f, 0.0f);
        glUniform1i(glGetUniformLocation(shaderProgram, "mandel_or_julia"), chosen_fractal);
        glUniform1i(_mode, mode);
        int container[2];

        while (!glfwWindowShouldClose(window))
        {
            deltaTime = glfwGetTime() - lastFrame;
            lastFrame = glfwGetTime();
            
            { // normal drawing pass
                glUniform2f(_position, position[0], position[1]);
                glUniform1f(_zoom, 5.0f / screenSettings::width * zoom);
                glUniform1i(_iterations, iterations);
                world_to_screen(container, variablePosition[0], variablePosition[1]);
                glUniform2i(_variable_on_screen, container[0], (int)(screenSettings::height - container[1]));

                glClear(GL_COLOR_BUFFER_BIT);
                glUseProgram(shaderProgram);
                vertexArray.bind();
                glDrawElements(GL_TRIANGLE_STRIP, sizeof(vertices) / sizeof(GLfloat), GL_UNSIGNED_INT, (const void*)0);
            }
            
            glfwPollEvents();
            processInput(window, deltaTime);
            
            glfwSwapBuffers(window); 
 
            fCounter++;
            if(fCounter > 1/deltaTime) {
                std::cout << "FPS: " << 1 / deltaTime << std::endl;
                // std::cout << "screen: " << screenSettings::width << ", " << screenSettings::height << "\n";
                fCounter = 0;
            } 
        }	
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}