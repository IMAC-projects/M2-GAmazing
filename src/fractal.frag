#version 330 core
in vec4 gl_FragCoord;
 
out vec4 fragColor;
out float gl_FragDepth;
 
uniform float center_x;
uniform float center_y;
uniform float zoom;
uniform vec4 color_ranges;
 
#define MAX_ITERATIONS 600
 
int get_iterations()
{
    float real = ((gl_FragCoord.x / 1080.0f - 0.5f) * zoom + center_x) * 4.0f;
    float imag = ((gl_FragCoord.y / 1080.0f - 0.5f) * zoom + center_y) * 4.0f;
 
    int iterations = 0;
    float const_real = real;
    float const_imag = imag;
 
    while (iterations < MAX_ITERATIONS)
    {
        float tmp_real = real;
        real = (real * real - imag * imag) + const_real;
        imag = (2.0 * tmp_real * imag) + const_imag;
         
        float dist = real * real + imag * imag;
 
        if (dist > 4.0)
            break;
 
        ++iterations;
    }
    return iterations;
}
 
 
vec4 return_color()
{
    int iter = get_iterations();
    if (iter == MAX_ITERATIONS)
    {
        gl_FragDepth = 0.0f;
        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
 
    float iterations = float(iter) / MAX_ITERATIONS;
    gl_FragDepth = iterations;
 
    vec4 color_0 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 color_1 = vec4(0.0f, 0.2f, 0.5f, 1.0f);
    vec4 color_2 = vec4(1.0f, 0.8f, 0.0f, 1.0f);
    vec4 color_3 = vec4(1.0f, 0.0f, 0.4f, 1.0f);
 
    float fraction = 0.0f;
    if (iterations < color_ranges[1])
    {
        fraction = (iterations - color_ranges[0]) / (color_ranges[1] - color_ranges[0]);
        return mix(color_0, color_1, fraction);
    }
    else if(iterations < color_ranges[2])
    {
        fraction = (iterations - color_ranges[1]) / (color_ranges[2] - color_ranges[1]);
        return mix(color_1, color_2, fraction);
    }
    else
    {
        fraction = (iterations - color_ranges[2]) / (color_ranges[3] - color_ranges[2]);
        return mix(color_2, color_3, fraction);
    }
}
 
void main()
{
    fragColor = return_color();
}