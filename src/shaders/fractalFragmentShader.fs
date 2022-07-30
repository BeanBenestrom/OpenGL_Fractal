#version 200 core

out vec4 FragColor;
in vec2 texCoord;

uniform vec2 position;
uniform float zoom;
uniform int iterations;

uniform float window_width;
uniform float window_height;

uniform vec2 variable_position;
uniform ivec2 variable_on_screen;
uniform int variable_box_size;
uniform int variable_box_on;
uniform vec2 variable_position_in_box_offset;
uniform int mode;
uniform int mandel_or_julia;


void main()
{
    vec4 color = vec4(0, 0, 0, 1);

	int i = 0;
	float x = variable_position.x; 
	float y = variable_position.y;
	float c_x = (gl_FragCoord.x - window_width  * 0.5) * zoom + position.x;
	float c_y = (gl_FragCoord.y - window_height * 0.5) * zoom + position.y;
	float temp_x;	// Do x first
    int variable_box_dot_x;
    int variable_box_dot_y;

	while (i < iterations)
	{
		// Equation

		// MandelBrot
        if (mandel_or_julia == 0)
        {
            temp_x = x * x - y * y + c_x;
            y = 2 * x * y + c_y;

            x = temp_x;

            if (x * x + y * y > 4) { break; }
        }

		// Julia Set
        if (mandel_or_julia == 1)
        {
            temp_x = c_x * c_x - c_y * c_y + variable_position.x;
            c_y = 2 * c_x * c_y + variable_position.y;

            c_x = temp_x;

            if (c_x * c_x + c_y * c_y > 4) { break; }
        }
		i++;
	}

	// Color
    
    float t = 15 * float(i) / iterations + 3.14 + 3.14 / 8;

    color.x = (0.5 * cos(t) + 0.5);
	color.y = (0.5 * sin(t) + 0.5);
	color.z = (1 - 0.5 * (cos(t) + sin(t) + 2));


    float gray_scale = float(i) / iterations;
    // float pop_gray = -(gray_scale * gray_scale) + 2 * gray_scale;
    float pop_gray = 1.1 - 0.11 / (gray_scale + 0.1);
	// float gray_scale = sin(float(i));
    //float gray_scale = 1 - float((int(10000 * float(i) / iterations) % 2000)) / 2000;
    // float gray_scale = cos(float(int(10000 * (float(i) / iterations)) % 2000) / 2000 * (3.14 / 2));

    color.x = color.x * pop_gray;
	color.y = color.y * pop_gray;
	color.z = color.z * pop_gray;
    

	if (i == iterations) { color.x = 0; color.y = 0; color.z = 0; }

    if (
        mode == 2 &&
        (gl_FragCoord.x - variable_on_screen.x) * (gl_FragCoord.x - variable_on_screen.x) + 
		(gl_FragCoord.y - variable_on_screen.y) * (gl_FragCoord.y - variable_on_screen.y) < 25)
    {
        color.x = 0;
        color.y = 0.74;
        color.z = 0;
    }

    if (
        mode == 2 && variable_box_on == 1 && 
        (window_width - 30 - variable_box_size < gl_FragCoord.x && gl_FragCoord.x < window_width - 30) &&
        (window_height - 30 - variable_box_size < gl_FragCoord.y && gl_FragCoord.y < window_height - 30)
    )
    {
        color.x = 0.12;
        color.y = 0.12;
        color.z = 0.12;

        variable_box_dot_x = int(window_width - variable_box_size * 0.5 - 30 + variable_position_in_box_offset.x);
        variable_box_dot_y = int(window_height - variable_box_size * 0.5 - 30 - variable_position_in_box_offset.y);

        if (
        (gl_FragCoord.x - variable_box_dot_x) * (gl_FragCoord.x - variable_box_dot_x) + 
		(gl_FragCoord.y - variable_box_dot_y) * (gl_FragCoord.y - variable_box_dot_y) < 9)
        {
            color.x = 0;
            color.y = 0.74;
            color.z = 0;
        }
    }

    FragColor = color;
} 