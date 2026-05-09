uniform sampler2D texture;
uniform float time;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, uv);

    // Ignore transparent pixels
    if (color.a < 0.1)
    {
        discard;
    }

    // Pulse animation
    float pulse = 0.75 + sin(time * 4.0) * 0.25;

    // Cyan sci-fi tint
    color.rgb *= vec3(0.4, 0.9, 1.5);

    // Animated transparency
    color.a *= 0.45 * pulse;

    gl_FragColor = color;
}