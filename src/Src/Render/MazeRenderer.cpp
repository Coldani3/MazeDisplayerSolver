#include <Render/MazeRenderer.h>

MazeRenderer::MazeRenderer(Maze maze, int centerX, int centerY, int centerZ) {
    selectedPath = MazePath(maze.width, maze.height, maze.depth, maze.hyperDepth);
    mazeCenterX = centerX;
    mazeCenterY = centerY;
    mazeCenterZ = centerZ;
    this->maze = maze;
}

MazeRenderer::~MazeRenderer() {
    //I think openGL deletes these automatically but it's better to be safe than sorry
    cleanup();
}

void MazeRenderer::render() {
    for (int x = 0; x < maze.width; x++) {
        for (int y = 0; y < maze.height; y++) {
            for (int z = 0; z < maze.depth; z++) {
                std::vector<int> coords = { x, y, z, currentW };

                if (maze[coords] > 0) {
                    drawMazeCellPaths(maze[coords], x, y, z, currentW);
                    drawMazeCellCenter(x, y, z, currentW);
                }
            }
        }
    }
}

void MazeRenderer::setup() {
    //create shaders
    std::cout << "Initialising Shaders..." << std::endl;
    std::cout << "genericCubeShaderVert..." << std::endl;
    unsigned int genericCubeShaderVert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(genericCubeShaderVert, 1, &mazeCellPathVertexShader, NULL);
    glCompileShader(genericCubeShaderVert);
    checkShaderCompileSuccess(genericCubeShaderVert);

    std::cout << "genericCubeShaderFrag..." << std::endl;
    unsigned int genericCubeShaderFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(genericCubeShaderFrag, 1, &mazeCellPathFragmentShader, NULL);
    glCompileShader(genericCubeShaderFrag);
    checkShaderCompileSuccess(genericCubeShaderFrag);

    std::cout << "cellCenterCubeShaderVert..." << std::endl;
    unsigned int cellCenterCubeShaderVert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(cellCenterCubeShaderVert, 1, &mazeCellCenterCubeVertexShader, NULL);
    glCompileShader(cellCenterCubeShaderVert);
    checkShaderCompileSuccess(cellCenterCubeShaderVert);

    std::cout << "cellCenterCubeShaderFrag..." << std::endl;
    unsigned int cellCenterCubeShaderFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(cellCenterCubeShaderFrag, 1, &mazeCellCenterCubeFragmentShader, NULL);
    glCompileShader(cellCenterCubeShaderFrag);
    checkShaderCompileSuccess(cellCenterCubeShaderFrag);

    /*unsigned int genericNormalsGeometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(genericNormalsGeometryShader, 1, &normalsGeometryShader, NULL);
    glCompileShader(genericNormalsGeometryShader);
    checkShaderCompileSuccess(genericNormalsGeometryShader);*/
    std::cout << "Done." << std::endl;

    //create programs
    std::cout << "Creating OpenGL Programs..." << std::endl;
    mazePathProgram = glCreateProgram();
    glAttachShader(mazePathProgram, genericCubeShaderVert);
    glAttachShader(mazePathProgram, genericCubeShaderFrag);
    glLinkProgram(mazePathProgram);
    checkProgramCompileSuccess(mazePathProgram);

    cellCenterProgram = glCreateProgram();
    glAttachShader(cellCenterProgram, cellCenterCubeShaderVert);
    glAttachShader(cellCenterProgram, cellCenterCubeShaderFrag);
    glLinkProgram(cellCenterProgram);
    checkProgramCompileSuccess(cellCenterProgram);
    std::cout << "Done." << std::endl;

    std::cout << "Cleaning up Shaders..." << std::endl;
    //clean up unecessary shaders
    glDeleteShader(genericCubeShaderVert);
    glDeleteShader(genericCubeShaderFrag);
    glDeleteShader(cellCenterCubeShaderVert);
    glDeleteShader(cellCenterCubeShaderFrag);
    std::cout << "Done." << std::endl;

    // we will transform the cubes into the appropriate positions in the shader
    std::cout << "Creating and populating buffers..." << std::endl;

    //cube buffers
    glGenBuffers(1, &mazeCenterVBO);
    glGenVertexArrays(1, &mazeCenterVAO);

    glBindVertexArray(mazeCenterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mazeCenterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesNormals), cubeVerticesNormals, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //path buffers
    glGenBuffers(1, &mazePathVBO);
    glGenVertexArrays(1, &mazePathVAO);

    glBindVertexArray(mazePathVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mazePathVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mazePathVerticesNormals), mazePathVerticesNormals, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::cout << "Done." << std::endl;
}

void MazeRenderer::cleanup() {
    std::cout << "Cleaning up Maze Renderer..." << std::endl;
    int cellCenterExists, mazePathExists;

    if (cellCenterProgram != 0) {
        glGetProgramiv(cellCenterProgram, GL_DELETE_STATUS, &cellCenterExists);

        if (cellCenterExists != GL_TRUE) {
            std::cout << "Cleaning up cellCenterProgram..." << std::endl;
            glDeleteProgram(cellCenterProgram);
        } else {
            std::cout << "cellCenterProgram was already deleted, skipping..." << std::endl;
        }
    }

    if (mazePathProgram != 0) {
        glGetProgramiv(mazePathProgram, GL_DELETE_STATUS, &mazePathExists);

        if (mazePathExists != GL_TRUE) {
            std::cout << "Cleaning up mazePathProgram..." << std::endl;
            glDeleteProgram(mazePathProgram);
        } else {
            std::cout << "mazePathProgram was already deleted, skipping..." << std::endl;
        }
    }
}

void MazeRenderer::setCamera(std::shared_ptr<Camera> camera) {
    this->camera = camera;
}

void MazeRenderer::setMazeCenterProgram(int program) {
    cellCenterProgram = program;
}

void MazeRenderer::setMazePathProgram(int program) {
    mazePathProgram = program;
}

void MazeRenderer::setShowPath(bool showPath) {
    this->showPath = showPath;
}

void MazeRenderer::setWViewing(int w) {
    if (w < maze.hyperDepth && w >= 0) {
        currentW = w;
    }
}

int MazeRenderer::getWViewing() {
    return currentW;
}

glm::vec3 MazeRenderer::getCellColour(std::vector<int> coords) {
    if (coords == maze.mazeEntrance) {
        return mazeEntranceColour;
    }
    else if (coords == maze.mazeExit) {
        return mazeExitColour;
    }
    else {
        if (selectedPath.pathSize() > 0 && showPath) {
            if (selectedPath.visitedCell(coords)) {
                return visitedCellColour;
            }
        }

        return defaultCellColour;
    }
}

glm::mat4 MazeRenderer::mazeCellPathTransform(glm::vec3 initialCoords, float rotateAngleX, float rotateAngleY, float translateX, float translateY, float translateZ) {
    glm::mat4 identity = glm::mat4(1.0f);

    glm::mat4 toOrigin = glm::translate(identity, -initialCoords);
    glm::mat4 andBack = glm::translate(identity, initialCoords);

    glm::mat4 rotateY = glm::rotate(identity, glm::radians(rotateAngleY), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateX = glm::rotate(identity, glm::radians(rotateAngleX), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 translate = glm::translate(identity, glm::vec3(translateX, translateY, translateZ));

    return andBack * translate * rotateX * rotateY * toOrigin;
}

void MazeRenderer::drawMazeCellCenter(int mazeX, int mazeY, int mazeZ, int mazeW) {
    if (mazeW == currentW) {
        //TODO: store these vecs in a lookup buffer to save performance and memory
        glm::vec3 coords = glm::vec3((maze.width - mazeX) + mazeCenterX, mazeY + mazeCenterY, mazeZ + mazeCenterZ);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), coords);
        glm::mat4 view = camera->getViewMatrix();

        glm::vec3 lightPos = glm::vec3(camera->getXPos(), camera->getYPos(), camera->getZPos());
        glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);

        glUseProgram(cellCenterProgram);

        glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));

        glUniform3fv(glGetUniformLocation(cellCenterProgram, "cellColour"), 1, glm::value_ptr(getCellColour({ mazeX, mazeY, mazeZ, mazeW })));
        glUniform3fv(glGetUniformLocation(cellCenterProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(cellCenterProgram, "lightColour"), 1, glm::value_ptr(lightColour));

        glBindVertexArray(mazeCenterVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //unbind VA
        glBindVertexArray(0);
    }
    else {
        std::cout << "Not in this 4D slice" << std::endl;
    }
}

void MazeRenderer::drawMazeCellPaths(unsigned char mazeCellData, int mazeX, int mazeY, int mazeZ, int mazeW) {
    if (mazeW == currentW) {
        std::vector<int> mazeCoords = { mazeX, mazeY, mazeZ, mazeW };
        glm::vec3 modelCoords = glm::vec3((maze.width - mazeX) + mazeCenterX, mazeY + mazeCenterY, mazeZ + mazeCenterZ);
        //translation to get it to the same modelCoords as the center piece, from which we then translate it again into the proper position
        glm::mat4 initialTranslate = glm::translate(glm::mat4(1.0f), modelCoords);
        glm::mat4 view = camera->getViewMatrix();
        float wLerp = 1.0f;

        glm::vec3 lightPos = glm::vec3(camera->getXPos(), camera->getYPos(), camera->getZPos());
        glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);

        glUseProgram(mazePathProgram);

        glUniform3fv(glGetUniformLocation(mazePathProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(mazePathProgram, "lightColour"), 1, glm::value_ptr(lightColour));

        glUniformMatrix4fv(glGetUniformLocation(mazePathProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(mazePathProgram, "projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));

        glBindVertexArray(mazePathVAO);

        for (unsigned int i = 0; i < 8; i++) {
            unsigned int bitChecking = 1 << i;

            if ((mazeCellData & bitChecking) > 0) {
                std::vector<float> transformation = cellPathTransformations[i];
                //TODO: move all matrix multiplications into the shaders?
                glm::mat4 model = mazeCellPathTransform(modelCoords, transformation[0], transformation[1], transformation[2], transformation[3], transformation[4]) * initialTranslate;
                glm::vec3 cellColour = getCellColour(mazeCoords);
                //perform this out of the shader as inverse is bad on the GPU due to performance
                glm::mat3 normalTransform = glm::transpose(glm::inverse(glm::mat3(model)));

                if (i >= 6) {
                    if (bitChecking == ANA) {
                        cellColour = anaColour;
                    }
                    else {
                        cellColour = kataColour;
                    }
                }

                glUniform3fv(glGetUniformLocation(mazePathProgram, "cellColour"), 1, glm::value_ptr(cellColour));

                glUniformMatrix4fv(glGetUniformLocation(mazePathProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix3fv(glGetUniformLocation(mazePathProgram, "normalTransform"), 1, GL_FALSE, glm::value_ptr(normalTransform));

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        glBindVertexArray(0);
    }
}
