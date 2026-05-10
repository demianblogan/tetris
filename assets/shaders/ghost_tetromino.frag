uniform sampler2D texture;
uniform float time;

void main()
{
    vec2 textureCoordinates = gl_TexCoord[0].xy;
    vec4 textureColor = texture2D(texture, textureCoordinates);

    // =====================================================
    // Ignore transparent pixels
    // =====================================================

    if (textureColor.a < 0.1)
    {
        discard;
    }

    // =====================================================
    // Pulse animation
    // =====================================================

    float pulseIntensity = 0.75 + sin(time * 4.0) * 0.25;

    // =====================================================
    // Cyan sci-fi tint
    // =====================================================

    vec3 ghostTintColor =  vec3(0.4, 0.9, 1.5);
    textureColor.rgb *= ghostTintColor;

    // =====================================================
    // Animated transparency
    // =====================================================

    float ghostTransparency = 0.45 * pulseIntensity;

    textureColor.a *= ghostTransparency;

    gl_FragColor =  textureColor;
}