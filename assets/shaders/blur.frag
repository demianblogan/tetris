uniform sampler2D texture;

void main()
{
    vec2 textureCoordinates = gl_TexCoord[0].xy;
    float blurOffset = 0.0015;

    vec4 blurredColor = vec4(0.0);
    blurredColor += texture2D(texture, textureCoordinates);
    blurredColor += texture2D(texture, textureCoordinates + vec2(blurOffset, 0.0));
    blurredColor += texture2D(texture, textureCoordinates - vec2(blurOffset, 0.0));
    blurredColor += texture2D(texture, textureCoordinates + vec2(0.0, blurOffset));
    blurredColor += texture2D(texture, textureCoordinates - vec2(0.0, blurOffset));
    blurredColor /= 5.0;

    gl_FragColor = blurredColor;
}