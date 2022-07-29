#version 430 core

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


void main()
{
    vec4 color = vec4(0, 0, 0, 1);

	int i = 0;
	float x = 0; 
	float y = 0;
	float c_x = (gl_FragCoord.x - window_width  * 0.5) * zoom + position.x;
	float c_y = (gl_FragCoord.y - window_height * 0.5) * zoom + position.y;
	float temp_x;	// Do x first
    int variable_box_dot_x;
    int variable_box_dot_y;

	while (i < iterations)
	{
		// Equation

		// MandelBrot
		// temp_x = x * x - y * y + c_x;
		// y = 2 * x * y + c_y;

        // c_x = temp_x;

		// if (c_x * c_x + c_y * c_y > 4) { break; }

		// Julia Set
		temp_x = c_x * c_x - c_y * c_y + variable_position.x;
		c_y = 2 * c_x * c_y + variable_position.y;

		c_x = temp_x;

		if (c_x * c_x + c_y * c_y > 4) { break; }
		i++;
	}

	// Color
    
    float t = 15 * float(i) / iterations + 3.14 + 3.14 / 8;

    color.x = (0.5 * cos(t) + 0.5);
	color.y = (0.5 * sin(t) + 0.5);
	color.z = (1 - 0.5 * (cos(t) + sin(t) + 2));

    // if (i < iterations / 20)
    // {
    //     color.x = color.x * (-cos(t) * 0.5 + 0.5);
    //     color.y = color.y * (-cos(t) * 0.5 + 0.5);
    //     color.z = color.z * (-cos(t) * 0.5 + 0.5);
    // }

    // color.x = 0;
	// color.y = 0;
	// color.z = 0;
    // int color_cycles = 5;
    // int section = int(float(iterations) / color_cycles / 3);
    // int i_color_section = i % int(section * 3);
    // int color_gradient = i_color_section / (section * 3);

    // if (i_color_section <= section)
    // {
    //     color.z = color_gradient;
	//     color.y = 1 - color_gradient;
    // }
    // else if (section <= i_color_section && i_color_section <= 2 * section)
    // {
    //     color.y = color_gradient;
	//     color.x = 1 - color_gradient;
    // }
    // else if (2 * section <= i_color_section)
    // {
    //     color.x = color_gradient;
	//     color.z = 1 - color_gradient;
    // }


    // color.x = 0;
	// color.y = 0;
	// color.z = 0;
    // int color_cycles = 5;
    // int section = int(float(iterations) / color_cycles / 3);
    // int i_color_section = i % int(section * 3);

    // if (i_color_section <= section)
    // {   
    //     float scale = cos(float(i_color_section) / 3 / section * 3.14) * 0.5 + 0.5;    
    //     color.x = scale;
	//     color.y = 1 - scale;
    // }
    // else if (section <= i_color_section && i_color_section <= 2 * section)
    // {
    //     float scale = cos(float(i_color_section) / 3 / section * 3.14) * 0.5 + 0.5;
    //     color.y = scale;
	//     color.z = 1 - scale;
    // }
    // else if (2 * section <= i_color_section)
    // {
    //     float scale = cos(float(i_color_section) / 3 / section * 3.14) * 0.5 + 0.5;
    //     color.z = scale;
	//     color.x = 1 - scale;
    // }


    // float gray_scale = float(i) / iterations;
	// // float gray_scale = sin(float(i));

    // color.x = gray_scale;
	// color.y = gray_scale;
	// color.z = gray_scale;

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