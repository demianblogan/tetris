uniform sampler2D texture;
uniform float time;

void main()
{
    vec2 textureCoordinates = gl_TexCoord[0].xy;
    vec4 textureColor = texture2D(texture, textureCoordinates);

    // =====================================================
    // Pulse animation
    // =====================================================

    float pulseIntensity = 0.85 + sin(time * 4.0) * 0.15;

    // =====================================================
    // Glow tint
    // =====================================================

    vec3 glowTintColor = vec3(0.4, 0.9, 1.0);

    // =====================================================
    // Base color
    // =====================================================

    vec3 finalColor = textureColor.rgb * pulseIntensity;

    // =====================================================
    // Additional glow
    // =====================================================

    finalColor += glowTintColor * 0.22 * pulseIntensity;

    gl_FragColor = vec4(finalColor, textureColor.a);
}