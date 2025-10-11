// Shader.frag
#version 330

in vec4 vColor;
in vec2 TexCoord0;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight
{
    vec3 color;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition; // camera position

void main()
{
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;
    
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
    
    vec4 specularColor = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0f) // only apply specular color if diffuse lighting is present
    {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));
        
        float specularFactor = dot(fragToEye, reflectedVertex);
        
        if (specularFactor > 0)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }
    
    color = texture(theTexture, TexCoord0) * (ambientColor + diffuseColor + specularColor); // combine texture with RGB value
}