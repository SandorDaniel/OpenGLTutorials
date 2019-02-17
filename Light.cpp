#include "Light.h"


std::vector < Light* >    Light::lights{};

std::vector < GLfloat >   Light::m_source_light_power{};
std::vector < GLfloat >   Light::m_source_light_ambient_power_coefficient{};
std::vector < glm::vec4 > Light::m_source_light_posdir{};

std::vector < GLfloat >   Light::m_positional_light_angle_in_radians{};
std::vector < glm::vec3 > Light::m_positional_source_light_dir{};

std::vector < glm::vec3 > Light::m_source_light_diffuse_col{};
std::vector < glm::vec3 > Light::m_source_light_specular_col{};
std::vector < glm::vec3 > Light::m_source_light_ambient_col{};

GLint Light::m_source_light_powerID;
GLint Light::m_source_light_ambient_power_coefficientID;
GLint Light::m_source_light_posdirID;

GLint Light::m_positional_light_angle_in_radiansID;
GLint Light::m_positional_source_light_dirID;

GLint Light::m_source_light_diffuse_colID;
GLint Light::m_source_light_specular_colID;
GLint Light::m_source_light_ambient_colID;