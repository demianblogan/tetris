uniform sampler2D texture;
uniform float time;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    // =====================================================
    // CRT curvature
    // =====================================================

    vec2 curvedUV = uv * 2.0 - 1.0;

    float distortion = 0.03;

    curvedUV += curvedUV * length(curvedUV) * distortion;

    curvedUV = (curvedUV + 1.0) * 0.5;

    // Outside screen
    if (curvedUV.x < 0.0 || curvedUV.x > 1.0 ||
        curvedUV.y < 0.0 || curvedUV.y > 1.0)
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // =====================================================
    // Chromatic aberration
    // =====================================================

    float chromaStrength = 0.0005;

    float r = texture2D(texture, curvedUV + vec2(chromaStrength, 0.0)).r;
    float g = texture2D(texture, curvedUV).g;
    float b = texture2D(texture, curvedUV - vec2(chromaStrength, 0.0)).b;

    vec3 color = vec3(r, g, b);

    // =====================================================
    // Scanlines
    // =====================================================

    float scanline =  sin(curvedUV.y * 1080.0 * 1.5) * 0.04;

    color -= scanline;

    // =====================================================
    // Vignette
    // =====================================================

    float dist = distance(curvedUV, vec2(0.5));

    color *= 1.0 - dist * 0.5;

    gl_FragColor = vec4(color, 1.0);
}