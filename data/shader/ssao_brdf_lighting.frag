#version 330 core
out vec4 FragColor;

 //material parameters
uniform float metallic;
uniform float roughness;


 //SSAO input
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

 //lights
uniform vec3 lightPositionsInView;
uniform vec3 lightColors;
const float PI = 3.14159265359;
 //shadow

//shadow
uniform mat4 view;
uniform samplerCube depthMap;
uniform float far_plane;             
uniform float near_plane;
uniform float light_size=0.005;
uniform bool shadowsFlag;

float blockRadiusCalculation(vec3 fragPos)
{
    vec3 lightpose=(inverse(view)*vec4(lightPositionsInView,1.0)).xyz;
    float fragToLightdistance = length(fragPos -lightPositionsInView);
    float Radius=light_size/fragToLightdistance*(fragToLightdistance-near_plane);
    return Radius;
}

float blockDepthCalculation(vec3 fragPos,float Radius )
{
     vec3 lightpose=(inverse(view)*vec4(lightPositionsInView,1.0)).xyz;
     float bias = 0.01; 
     float samples = 6.0;
     vec3 fragToLight = fragPos - lightpose;
     float closestDepth = texture(depthMap, fragToLight).r;
     closestDepth *= far_plane;
     float currentDepth = length(fragToLight);
     int count=0;
     float depth=0;
     for(float x = -Radius; x < Radius; x += Radius / (samples * 0.5))
     {
         for(float y = -Radius; y < Radius; y += Radius / (samples * 0.5))
         {
             for(float z = -Radius; z < Radius; z += Radius / (samples * 0.5))
             {
                 float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r; // use lightdir to lookup cubemap
                 closestDepth *= far_plane;   // Undo mapping [0;1]
                 if(currentDepth - bias > closestDepth)
                 {
                    depth+=closestDepth;
                    count++;
                 }  
             }
         }
     }
     if(count>0)return depth/count;
     else return 0;
}


float offsetCalculation(vec3 fragPos,float depth)
{
    
    vec3 lightpose=(inverse(view)*vec4(lightPositionsInView,1.0)).xyz;
    vec3 fragToLight = fragPos - lightpose;
    float currentDepth = length(fragToLight);
    float offset=light_size/depth*(currentDepth-depth+0.0001);
    return offset;
}




float ShadowCalculation(vec3 fragPos)
{
      
     vec3 lightpose=(inverse(view)*vec4(lightPositionsInView,1.0)).xyz;
      fragPos=(inverse(view)*vec4(fragPos,1.0)).xyz;
      // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightpose;
    // use the fragment to light vector to sample from the depth map    
     float closestDepth = texture(depthMap, fragToLight).r;
     closestDepth *= far_plane;
     float currentDepth = length(fragToLight);


     float blockRadius=blockRadiusCalculation(fragPos);
     float blockDepth=blockDepthCalculation(fragPos,blockRadius);
     if(blockDepth==0)
     return 0;
     float offset=offsetCalculation(fragPos,blockDepth);

 

    // PCF
     float shadow = 0.0;
     float bias = 0.01; 
     float samples = 6.0;
   
     for(float x = -offset; x < offset; x += offset / (samples * 0.5))
     {
         for(float y = -offset; y < offset; y += offset / (samples * 0.5))
         {
             for(float z = -offset; z < offset; z += offset / (samples * 0.5))
             {
                 float closestDepth = texture(depthMap, fragToLight + vec3(x, y, 0)).r; // use lightdir to lookup cubemap
                 closestDepth *= far_plane;   // Undo mapping [0;1]
                 if(currentDepth - bias > closestDepth)
                     shadow += 1.0;
             }
         }
     }
     shadow /= (samples * samples * samples);   
  
    return shadow;
}
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;


}
 
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
 
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{		
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    
    vec3 albedo = texture(gAlbedo, TexCoords).rgb;
    float AmbientOcclusion = texture(ssao, TexCoords).r;
   
    
    vec3 N = normalize(Normal);
    vec3 V = normalize( - FragPos);



    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

        // calculate per-light radiance
        vec3 L = normalize(lightPositionsInView - FragPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositionsInView - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors * attenuation;
      


        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again


    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).

     vec3 ambient = vec3(0.03) * albedo*AmbientOcclusion ;


     float shadow = shadowsFlag ? ShadowCalculation(FragPos) : 0.0;   
     vec3 color = ambient +(1-shadow)* Lo;
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);

    }