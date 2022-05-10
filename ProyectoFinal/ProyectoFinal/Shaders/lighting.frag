#version 330 core

#define NUMBER_OF_POINT_LIGHTS 2 // se define un numero de luces puntuales

// El material 
// se define por una componente difusa la cual sera determinada por la textura, 
// Una componente especular que de igual forma se determina con base en la textura definida y esta a su vez le dara la pripiedad de brillo al objeto
// y finalmente el shininess para modular el nivel de brillo
struct Material 
{
    sampler2D diffuse; 
    sampler2D specular;
    float shininess;
};

// La luz direccional
// como afecta a todo el escenario  no requiere de una posicion en genaral, por lo tanto solo se define:
// la direccion hacia donde van estar dirigidos los rayos de luz
// las componentes ambiental, difusa y especular, las cuales siempre van a estar presentes en todo tipo de luz
struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Luz puntual
// En este caso si se requiere de una posicion en especifico por lo tanto se define:
// la posicion
// las constantes Kc, Kl & Kq las cuales definen la fuerza de atenuacion el cual le da la propiedad a la luz de atenuerce a cierta distancia lejana y mientras mas cercana sea, la intencidad de la luz aumenta 
// las componentes ambiental, difusa y especular
struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// SpotLight
// Esta tipo de luz se conforma por una luz puntual y a su vez de una direccional, por lo tanto se tiene:
// posicion, y las constantes para la atenuacion
// la direccion a la posicion a la cual estará apuntando la luz puntual
// El cono interior y un cono exterior las cuales definen que partes seran iluminadas y cuales no
// las componentes ambiental, difusa y especular
struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// este shader recibe el las posiciones de los fragmentos, normales y las textcoords
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// el shader retorna el color el cual sera la textura con todo y la iluminacion
out vec4 color;


uniform vec3 viewPos; // posicion de la vista
uniform DirLight dirLight; // la direccion de la luz
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS]; // un arreglo de pointlights, el cual sera de tamaño 4
// uniform SpotLight spotLight; // una spotlight
uniform Material material; // y un material
uniform int activaTransparencia;
uniform vec4 colorAlpha;

// Function prototypes
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main( )
{
    // Properties
    vec3 norm = normalize( Normal );
    vec3 viewDir = normalize( viewPos - FragPos );
    
    // Directional lighting
    vec3 result = CalcDirLight( dirLight, norm, viewDir );
    
    // Point lights
    for ( int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++ )
    {
        result += CalcPointLight( pointLights[i], norm, FragPos, viewDir );
    }
    
    // Spot light
    //result += CalcSpotLight( spotLight, norm, FragPos, viewDir );
 	
    color = colorAlpha*vec4( result,texture(material.diffuse, TexCoords).rgb );
	  if(color.a < 0.1 && activaTransparencia == 1)
        discard;

}

// Calculates the color when using a directional light.
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize( -light.direction ); // se define la direccion de la luz pero como se calcula a partir de cuando choca la luz hacia la camara, entonces se obtiene la direccion contraria inversa y se normaliza para efectos de economizar recursos
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 ); // se obtiene a partir del producto punto de la normal y la direccion de la luz que tanto se reflejara o refrectara 
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal ); // se obtiene el vector el cual sera la direccion hacia donde se reflejaran los rayos que chocan con el objeto a partir de la direccion original de la luz y la normal de la superficie
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess ); // se obtiene la especular
    
    // Combine results
    // se definen las componentes que caracterizan a la luz, ambiental, difusa y especular
    // no obstante ahora se multiplican los vectores de las compononentes de la luz que se recibe en esta funcion por vectores definidos por la textura la cual recibe tanto la componentes del material como las texCoords
    vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) ); 
    vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );
    
    return ( ambient + diffuse + specular ); // se regresa la suma de las componentes
}

// Calculates the color when using a point light.
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos ); // de igual forma se define la direccion contraria de donde choca hacia el origen del rayo de luz
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 ); // se obtiene que tanto se reflejara o refrectara
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal ); // vector que refleja la luz
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess ); // la especular
    
    // Attenuation
    float distance = length( light.position - fragPos ); // la distancia de atenuacion 
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) ); // ecuacion de la fuerza de atenuacion con base en las componente Kc, Kl & Kq
    
    // Combine results
    // componentes de la luz
    vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );
    vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );
    
    // se modifica cada componente, con base en la atenucacion definida anteriormente
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    // se retorna la suma de las componentes
    return ( ambient + diffuse + specular );
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos ); // de igual forma se define la direccion contraria de donde choca hacia el origen del rayo de luz
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 ); // que tanto se reflejara
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal ); // vector que refleja la luz
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess ); // la especular
    
    // Attenuation
    float distance = length( light.position - fragPos ); // distancia de atenuacion
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) ); // fuerza de atenuacion
    
    // Spotlight intensity
    float theta = dot( lightDir, normalize( -light.direction ) ); // se obtiene el angulo apartir de la direccion de la luz y la spotlight, para eso se realiza un producto punto de la direccion de la luz original y la que esta en direccion contraria
    float epsilon = light.cutOff - light.outerCutOff; // se obtiene el angulo que a partir de este se define el area de iluminacion, cual quier objeto cuya direccion sea tal que sea mayor o menor a este angulo no sera iluminado
    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 ); // se define la intensidad a partir de la ecuacion caracteristica de la spotlight
    
    // Combine results
    // se definen las componentes de la luz
    vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );
    vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );
    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );
    // se modifican estas componentes multiplicando por los escalares de atenuacion e intensidad 
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    // se retorna la suma de las componentes
    return ( ambient + diffuse + specular );
}