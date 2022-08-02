// This file defines the equations and colors
// I could have made it more comprehensive, but it's really just meant to make my life easier
// If you want to change the equations or colors... then... go ahead, try it
// The code is written in GLSL, but the file format doesn't have to be .vs, as I only made it so because it looks better for me in VSCode
// 
// Equations should be written in the form:
// 
// temp_x = ???;
// y = ???;
// x = temp_x;
// 
// if statement for when to exit loop
//
// 
// DO NOT DELETE THE COMMENTS, as the program uses them to differentiate the different pieces of code
// In the shaders the names <MandelBrot> <Julia Set> <Color> are used to denote where the piece of code are supposed to go
// 
// The variables are:
//
//                                     (x, y)   -   z / new value / next value
//                                 (c_x, c_y)   -   c / current pixel value
// (variable_position.x, variable_position.y)   -   movable variable
//                (color.x, color.y, color.z)   -   RGB color for pixel
//                                 iterations   -   maximun amount of iterations
//                                          i   -   iterations that were done for current pixel
// 
// If a certain equation is not wanted, it can be turned off in the main.cpp file by changing the FRACTAL_OPTION variable,
// or by simply not writing anything between the commends


// <MandelBrot>
temp_x = x * x - y * y + c_x;
y = 2 * x * y + c_y;
x = temp_x;

if (x * x + y * y > 4) { break; }


// <Julia Set>
temp_x = c_x * c_x - c_y * c_y + variable_position.x;
c_y = 2 * c_x * c_y + variable_position.y;
c_x = temp_x;

if (c_x * c_x + c_y * c_y > 4) { break; }


// <Color>
float t = 15 * float(i) / iterations + 3.14 + 3.14 / 8;

color.r = (0.5 * cos(t) + 0.5);
color.g = (0.5 * sin(t) + 0.5);
color.b = (1 - 0.5 * (cos(t) + sin(t) + 2));


float gray_scale = float(i) / iterations;
float pop_gray = 1.1 - 0.11 / (gray_scale + 0.1);

color.r = color.r * pop_gray;
color.g = color.g * pop_gray;
color.b = color.b * pop_gray;


if (i == iterations) { color.r = 0; color.g = 0; color.b = 0; }

// <END>