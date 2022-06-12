#include <Render/Renderer.h>
#include <iostream>

void Renderer::checkShaderCompileSuccess(unsigned int shader) {
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "SHADER ERROR: " << infoLog << std::endl;
    }
}

void Renderer::checkProgramCompileSuccess(unsigned int program) {
    int success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "PROGRAM ERROR: " << infoLog << std::endl;
    }
}

void Renderer::deleteProgramIfExists(unsigned int program, std::string name = "a program") {
    int programSuccess;
    glGetProgramiv(program, GL_DELETE_STATUS, &programSuccess);

    if (programSuccess != GL_TRUE) {
        std::cout << "Cleaning up " << name << "..." << std::endl;
        glDeleteProgram(program);
    } else {
        std::cout << name << " was already deleted, skipping..." << std::endl;
    }
}
