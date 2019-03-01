#include "Light.h"



std::vector<PositionalLight*> PositionalLight::lights{};

std::vector<GLfloat>          PositionalLight::powers{};
std::vector<glm::vec3>        PositionalLight::positions{};

std::vector<GLfloat>          PositionalLight::angles_in_radians{};
std::vector<glm::vec3>        PositionalLight::dirs{};

std::vector<glm::vec3>        PositionalLight::diffuse_cols{};
std::vector<glm::vec3>        PositionalLight::specular_cols{};
std::vector<glm::vec3>        PositionalLight::ambient_cols{};

GLint PositionalLight::power_id;
GLint PositionalLight::pos_id;

GLint PositionalLight::angle_in_radians_id;
GLint PositionalLight::dir_id;

GLint PositionalLight::diffuse_col_id;
GLint PositionalLight::specular_col_id;
GLint PositionalLight::ambient_col_id;


std::vector<DirectionalLight*> DirectionalLight::lights{};

std::vector<GLfloat>           DirectionalLight::powers{};
std::vector<glm::vec3>         DirectionalLight::dirs{};

std::vector<glm::vec3>         DirectionalLight::diffuse_cols{};
std::vector<glm::vec3>         DirectionalLight::specular_cols{};
std::vector<glm::vec3>         DirectionalLight::ambient_cols{};

GLint DirectionalLight::power_id;
GLint DirectionalLight::dir_id;

GLint DirectionalLight::diffuse_col_id;
GLint DirectionalLight::specular_col_id;
GLint DirectionalLight::ambient_col_id;
