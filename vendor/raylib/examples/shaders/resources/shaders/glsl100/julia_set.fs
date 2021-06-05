#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform vec2 screenDims;        // Dimensions of the screen
uniform vec2 c;                 // c.x = real, c.y = imaginary component. Equation done is z^2 + c
uniform vec2 offset;            // Offset of the scale.
uniform float zoom;             // Zoom of the scale.

// NOTE: Maximum number of shader for-loop iterations depend on GPU,
// for example, on RasperryPi for this examply only supports up to 60
const int MAX_ITERATIONS = 48;  // Max iterations to do

// Square a complex number
vec2 ComplexSquare(vec2 z)
{
    return vec2(
        z.x * z.x - z.y * z.y,
        z.x * z.y * 2.0
    );
}

// Convert Hue Saturation Value (HSV) color into RGB
vec3 Hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    /**********************************************************************************************
      Julia sets use a function z^2 + c, where c is a constant.
      This function is iterated until the nature of the point is determined.

      If the magnitude of the number becomes greater than 2, then from that point onward
      the number will get bigger and bigger, and will never get smaller (tends towards infinity).
      2^2 = 4, 4^2 = 8 and so on.
      So at 2 we stop iterating.

      If the number is below 2, we keep iterating.
      But when do we stop iterating if the number is always below 2 (it converges)?
      That is what MAX_ITERATIONS is for.
      Then we can divide the iterations by the MAX_ITERATIONS value to get a normalized value that we can
      then map to a color.

      We use dot product (z.x * z.x + z.y * z.y) to determine the magnitude (length) squared.
      And once the magnitude squared is > 4, then magnitude > 2 is also true (saves computational power).
    *************************************************************************************************/
    
    // The pixel coordinates are scaled so they are on the mandelbrot scale
    // NOTE: fragTexCoord already comes as normalized screen coordinates but offset must be normalized before scaling and zoom
    vec2 z = vec2((fragTexCoord.x + offset.x/screenDims.x)*2.5/zoom, (fragTexCoord.y + offset.y/screenDims.y)*1.5/zoom);

    int iter = 0;
    for (int iterations = 0; iterations < 60; iterations++)
    {
        z = ComplexSquare(z) + c;  // Iterate function
        if (dot(z, z) > 4.0) break;

        iter = iterations;
    }

    // Another few iterations decreases errors in the smoothing calculation.
    // See http://linas.org/art-gallery/escape/escape.html for more information.
    z = ComplexSquare(z) + c;
    z = ComplexSquare(z) + c;
    
    // This last part smooths the color (again see link above).
    float smoothVal = float(iter) + 1.0 - (log(log(length(z)))/log(2.0));
    
    // Normalize the value so it is between 0 and 1.
    float norm = smoothVal/float(MAX_ITERATIONS);

    // If in set, color black. 0.999 allows for some float accuracy error.
    if (norm > 0.999) gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else gl_FragColor = vec4(Hsv2rgb(vec3(norm, 1.0, 1.0)), 1.0);
}
