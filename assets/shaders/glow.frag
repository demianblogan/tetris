uniform sampler2D texture;
uniform float time;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    vec4 tex = texture2D(texture, uv);

    float pulse =
        0.85 + sin(time * 4.0) * 0.15;

    vec3 glowTint =
        vec3(0.4, 0.9, 1.0);

    vec3 color =
        tex.rgb * pulse;

    color += glowTint * 0.22 * pulse;

    gl_FragColor =
        vec4(color, tex.a);
}