uniform sampler2D texture;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    float offset = 0.0015;

    vec4 color = vec4(0.0);

    color += texture2D(texture, uv);
    color += texture2D(texture, uv + vec2(offset, 0.0));
    color += texture2D(texture, uv - vec2(offset, 0.0));
    color += texture2D(texture, uv + vec2(0.0, offset));
    color += texture2D(texture, uv - vec2(0.0, offset));

    color /= 5.0;

    gl_FragColor = color;
}