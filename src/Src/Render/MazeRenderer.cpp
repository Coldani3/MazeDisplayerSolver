#include <Render/MazeRenderer.h>

MazeRenderer::MazeRenderer(std::shared_ptr<PerspectiveCamera> camera, std::shared_ptr<Maze> maze, int centerX, int centerY, int centerZ) {
    selectedPath = MazePath(maze->width, maze->height, maze->depth, maze->hyperDepth);
    renderedPath = MazePath(maze->width, maze->height, maze->depth, maze->hyperDepth);
    mazeCenterX = centerX;
    mazeCenterY = centerY;
    mazeCenterZ = centerZ;
    this->maze = maze;
    this->camera = camera;
    glm::mat4 identity = glm::mat4(1.0f);

    for (int i = 0; i < cellPathTransformations.size(); i++) {
        
        float rotateAngleX = cellPathTransformationsValues[i][0];
        float rotateAngleY = cellPathTransformationsValues[i][1];
        float translateX = cellPathTransformationsValues[i][2];
        float translateY = cellPathTransformationsValues[i][3];
        float translateZ = cellPathTransformationsValues[i][4];
        

        glm::mat4 rotateY = glm::rotate(identity, glm::radians(rotateAngleY), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotateX = glm::rotate(identity, glm::radians(rotateAngleX), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 translate = glm::translate(identity, glm::vec3(translateX, translateY, translateZ));

        cellPathTransformations[i] = translate * rotateX * rotateY;
    }
}

MazeRenderer::~MazeRenderer() {
    //I think openGL deletes these automatically but it's better to be safe than sorry
    cleanup();
}

void MazeRenderer::render(std::shared_ptr<MazeRenderInfo> mazeRenderInfo) {
    //TODO: separate thread for timings? also account for lag spikes

    if (showPath && 
        glfwGetTime() > lastPathAddTime + pathUpdateSpeed && 
        selectedPath.pathSize() > 0 && 
        selectedPath.pathSize() > renderedPath.pathSize()) {
        
        std::vector<int> coords = selectedPath[renderedPath.pathSize()];

        if (coords[3] != mazeRenderInfo->wViewing) {
            //setWViewing(coords[3]);
            mazeRenderInfo->wViewing = coords[3];
        }

        renderedPath.markCellVisited(coords);
    }

    for (int x = 0; x < maze->width; x++) {
        for (int y = 0; y < maze->height; y++) {
            for (int z = 0; z < maze->depth; z++) {
                std::vector<int> coords = { x, y, z, mazeRenderInfo->wViewing};

                unsigned char data = (*maze)[coords];

                if (data > 0) {
                    drawMazeCellPaths(data, x, y, z, mazeRenderInfo->wViewing);
                    drawMazeCellCenter(x, y, z, mazeRenderInfo->wViewing);
                }
            }
        }
    }
}

void MazeRenderer::setup() {
    //create shaders
    std::cout << "Initialising Shaders..." << '\n';
    std::cout << "genericCubeShaderVert..." << '\n';
    unsigned int genericCubeShaderVert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(genericCubeShaderVert, 1, &mazeCellPathVertexShader, NULL);
    glCompileShader(genericCubeShaderVert);
    checkShaderCompileSuccess(genericCubeShaderVert);

    std::cout << "genericCubeShaderFrag..." << '\n';
    unsigned int genericCubeShaderFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(genericCubeShaderFrag, 1, &mazeCellPathFragmentShader, NULL);
    glCompileShader(genericCubeShaderFrag);
    checkShaderCompileSuccess(genericCubeShaderFrag);

    std::cout << "cellCenterCubeShaderVert..." << '\n';
    unsigned int cellCenterCubeShaderVert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(cellCenterCubeShaderVert, 1, &mazeCellCenterCubeVertexShader, NULL);
    glCompileShader(cellCenterCubeShaderVert);
    checkShaderCompileSuccess(cellCenterCubeShaderVert);

    std::cout << "cellCenterCubeShaderFrag..." << '\n';
    unsigned int cellCenterCubeShaderFrag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(cellCenterCubeShaderFrag, 1, &mazeCellCenterCubeFragmentShader, NULL);
    glCompileShader(cellCenterCubeShaderFrag);
    checkShaderCompileSuccess(cellCenterCubeShaderFrag);

    std::cout << "Done." << '\n';

    //create programs
    std::cout << "Creating OpenGL Programs..." << '\n';
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

    std::cout << "Cleaning up Shaders..." << '\n';
    //clean up unecessary shaders
    glDeleteShader(genericCubeShaderVert);
    glDeleteShader(genericCubeShaderFrag);
    glDeleteShader(cellCenterCubeShaderVert);
    glDeleteShader(cellCenterCubeShaderFrag);
    std::cout << "Done." << std::endl;

    std::cout << "Creating and populating buffers..." << '\n';

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

    if (cellCenterProgram != 0) {
        deleteProgramIfExists(cellCenterProgram, "cellCenterProgram");
    }

    if (mazePathProgram != 0) {
        deleteProgramIfExists(mazePathProgram, "mazePathProgram");
    }
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

void MazeRenderer::getRenderPollInput(GLFWwindow* window, double delta) {
    float camSpeed = 0.1 * delta;
    float zoomSpeed = 2.5 * delta;

    /*if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        camMoveSpeedMod += 1.0 * delta;
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        camMoveSpeedMod -= 1.0 * delta;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        camMoveSpeedMod = 1.0;
    }*/

    /*camSpeed += camMoveSpeedMod;
    zoomSpeed += camMoveSpeedMod;*/

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->rotateAround(
            camera->getXLookingAt(),
            camera->getYLookingAt(),
            camera->getZLookingAt(),
            -360.0f * camSpeed, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->rotateAround(
            camera->getXLookingAt(),
            camera->getYLookingAt(),
            camera->getZLookingAt(),
            360.0f * camSpeed, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->rotateAround(
            camera->getXLookingAt(),
            camera->getYLookingAt(),
            camera->getZLookingAt(),
            0.0f, -360.0f * camSpeed, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->rotateAround(
            camera->getXLookingAt(),
            camera->getYLookingAt(),
            camera->getZLookingAt(),
            0.0f, 360.0f * camSpeed, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera->reset();
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera->zoom(zoomSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera->zoom(-zoomSpeed);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && glfwGetTime() >= (lastIndicatorToggle + 0.2)) {
        show4DIndicators = !show4DIndicators;
        lastIndicatorToggle = glfwGetTime();
    }

    if (glfwGetTime() > lastPathShowChange + 0.2) {
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            setShowPath(!showPath);
            lastPathShowChange = glfwGetTime();
        }
    }
}

std::shared_ptr<Camera> MazeRenderer::getCamera() {
    return camera;
}

glm::vec3 MazeRenderer::getCellColour(std::vector<int> coords) {
    if (coords == maze->mazeEntrance) {
        return mazeEntranceColour;
    } else if (coords == maze->mazeExit) {
        return mazeExitColour;
    } else {
        if (/*selectedPath*/renderedPath.pathSize() > 0 && showPath) {
            if (/*selectedPath*/renderedPath.visitedCell(coords)) {
                return visitedCellColour;
            }
        }

        return defaultCellColour;
    }
}

glm::mat4 MazeRenderer::mazeCellPathTransform(glm::vec3 initialCoords, glm::mat4 transformation) {
    glm::mat4 identity = glm::mat4(1.0f);

    glm::mat4 toOrigin = glm::translate(identity, -initialCoords);
    glm::mat4 andBack = glm::translate(identity, initialCoords);

    

    return andBack * transformation * toOrigin;
}

void MazeRenderer::drawMazeCellCenter(int mazeX, int mazeY, int mazeZ, int mazeW) {
    //TODO: store these vecs in a lookup buffer to save performance and memory

    glm::vec3 coords = glm::vec3((maze->width - mazeX) + mazeCenterX, mazeY + mazeCenterY, mazeZ + mazeCenterZ);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), coords);
    glm::mat4 view = camera->getView();

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

void MazeRenderer::drawMazeCellPaths(unsigned char mazeCellData, int mazeX, int mazeY, int mazeZ, int mazeW) {
    //PerspectiveCamera perspCam = static_cast<PerspectiveCamera&>(camera);
    //std::shared_ptr<PerspectiveCamera> perspCamera = std::dynamic_pointer_cast<PerspectiveCamera>(camera);

    std::vector<int> mazeCoords = { mazeX, mazeY, mazeZ, mazeW };
    glm::vec3 modelCoords = glm::vec3((maze->width - mazeX) + mazeCenterX, mazeY + mazeCenterY, mazeZ + mazeCenterZ);
    //translation to get it to the same modelCoords as the center piece, from which we then translate it again into the proper position
    glm::mat4 initialTranslate = glm::translate(glm::mat4(1.0f), modelCoords);
    glm::mat4 view = camera->getView();
    float wLerp = 1.0f;

    float debug = camera->getXPos();

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
            //TODO: move all matrix multiplications into the shaders?
            glm::mat4 model = mazeCellPathTransform(modelCoords, cellPathTransformations[i]) * initialTranslate;
            glm::vec3 cellColour = getCellColour(mazeCoords);
            //perform this out of the shader as inverse is bad on the GPU due to performance
            glm::mat3 normalTransform = glm::transpose(glm::inverse(glm::mat3(model)));

            if (i >= 6) {
                if (!show4DIndicators) {
                    break;
                }

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
