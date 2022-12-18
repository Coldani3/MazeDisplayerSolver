#include <Render/MazeRenderer.h>

#include <algorithm>

MazeRenderer::MazeRenderer(std::shared_ptr<PerspectiveCamera> camera, std::shared_ptr<Maze> maze, int centerX, int centerY, int centerZ) {
    selectedPath = MazePath(maze->width, maze->height, maze->depth, maze->hyperDepth);
    renderedPath = MazePath(maze->width, maze->height, maze->depth, maze->hyperDepth);
    mazeCenterX = static_cast<float>(centerX);
    mazeCenterY = static_cast<float>(centerY);
    mazeCenterZ = static_cast<float>(centerZ);
    this->maze = maze;
    this->camera = camera;
    glm::mat4 identity = glm::mat4(1.0f);
    
    //precompute cell path transformation matrices
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
    double now = glfwGetTime();
    //handle animation of solved path being displayed
    int selectedPathSize = selectedPath.pathSize();
    int renderedPathSize = renderedPath.pathSize();

    if (showPath && 
        now > lastPathAddTime + pathUpdateSpeed && 
        selectedPathSize > 0 && 
        selectedPathSize > renderedPathSize) {
        
        std::vector<int> coords = selectedPath[renderedPathSize];

        if (coords[3] != mazeRenderInfo->wViewing) {
            mazeRenderInfo->wViewing = coords[3];
        }

        renderedPath.markCellVisited(coords);
    }

    //actually draw the maze
    for (int x = 0; x < maze->width; x++) {
        for (int y = 0; y < maze->height; y++) {
            for (int z = 0; z < maze->depth; z++) {
                std::vector<int> coords = { x, y, z, mazeRenderInfo->wViewing};

                unsigned char data = (*maze)[coords];

                //default to 1.0f to avoid funny stuck slightly wrong mazes because of the animation.
                float scale = 1.0f;

                if (data > 0) {
                    if (mazeRenderInfo->wTransitioning) {
                        /*
                        * FUTURE ME NOTE: This transition stuff seems overly complicated. Just lerp between this w and the next w.
                        * 
                        * For w transition animation, get change with formula 1.0 - ((wChangeAnimStart + speed - glfwGetTime()) / speed)
                        * and clamp between 0.0 and 1.0. Then check to see the change on each 3d side - if both that side of this
                        * cell and the touching side of this cell are both still present (ie. there's a path between the cells in that
                        * direction) in the next w slice then pass an unchanged transition value to the shader.
                        */

                        float endTransitionTime = mazeRenderInfo->wChangeAnimStart + mazeRenderInfo->mazeTransitionAnimationSpeed;

                        scale = calculateScale(endTransitionTime, now, mazeRenderInfo->mazeTransitionAnimationSpeed);

                        if (!wasTransitioning) {
                            wasTransitioning = true;
                        }

                        if (now > endTransitionTime) {
                            mazeRenderInfo->finishWTransitionAnim();
                        }

                        //std::vector<std::vector<int>> 

                        //now check the cells on all sides
                        for (const std::vector<int> direction : touchingSides) {
                            //TODO: use Coordinate here
                            std::vector<int> addedCoords = { 
                                coords[0] + direction[0], 
                                coords[1] + direction[1], 
                                coords[2] + direction[2], 
                                coords[3] + direction[3] 
                            };

                            if (maze->inBounds(addedCoords)) {
                                unsigned char directionData = (*maze)[addedCoords];

                                //TODO: something here I think, not sure if I actually need this though? I think I do though
                            }
                        }
                    } else if (!mazeRenderInfo->wTransitioning && wasTransitioning) {
                        wasTransitioning = false;
                        mazeRenderInfo->finishWTransitionAnim();
                    }

                    drawMazeCellPaths(data, x, y, z, mazeRenderInfo->wViewing, mazeRenderInfo->lastW, scale);
                    drawMazeCellCenter(x, y, z, mazeRenderInfo->wViewing);
                }

                //TODO: large box indicator for where the path head currently is.
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
    std::cout << "Done." << '\n';

    std::cout << "Cleaning up Shaders..." << '\n';
    //clean up unecessary shaders
    glDeleteShader(genericCubeShaderVert);
    glDeleteShader(genericCubeShaderFrag);
    glDeleteShader(cellCenterCubeShaderVert);
    glDeleteShader(cellCenterCubeShaderFrag);
    std::cout << "Done." << '\n';

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

//TODO: what is the point of these?
void MazeRenderer::setMazeCenterProgram(int program) {
    cellCenterProgram = program;
}

void MazeRenderer::setMazePathProgram(int program) {
    mazePathProgram = program;
}

void MazeRenderer::setShowPath(bool showPath) {
    this->showPath = showPath;
}

void MazeRenderer::getRenderPollInput(GLFWwindow* window, double delta, const InputManager& inputManager) {
    float deltaFloat = static_cast<float>(delta);
    float camSpeed = 0.1f * deltaFloat;
    float zoomSpeed = 2.5f * deltaFloat;

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

    if (inputManager.getKeyPressed(GLFW_KEY_A)) {
        camera->rotateAround(
            camera->getXLookingAt(),
            camera->getYLookingAt(),
            camera->getZLookingAt(),
            -360.0f * camSpeed, 0.0f, 0.0f);
    }

    if (inputManager.getKeyPressed(GLFW_KEY_D)) {
        camera->rotateAround(
            camera->getXLookingAt(),
            camera->getYLookingAt(),
            camera->getZLookingAt(),
            360.0f * camSpeed, 0.0f, 0.0f);
    }

    if (inputManager.getKeyPressed(GLFW_KEY_S)) {
        camera->rotateAround(
            camera->getXLookingAt(),
            camera->getYLookingAt(),
            camera->getZLookingAt(),
            0.0f, -360.0f * camSpeed, 0.0f);
    }

    if (inputManager.getKeyPressed(GLFW_KEY_W)) {
        camera->rotateAround(
            camera->getXLookingAt(),
            camera->getYLookingAt(),
            camera->getZLookingAt(),
            0.0f, 360.0f * camSpeed, 0.0f);
    }

    if (inputManager.getKeyPressed(GLFW_KEY_R)) {
        camera->reset();
    }

    if (inputManager.getKeyPressed(GLFW_KEY_Z)) {
        camera->zoom(zoomSpeed);
    }

    if (inputManager.getKeyPressed(GLFW_KEY_X)) {
        camera->zoom(-zoomSpeed);
    }

    if (inputManager.getKeyPressed(GLFW_KEY_I) && glfwGetTime() >= (lastIndicatorToggle + 0.2)) {
        show4DIndicators = !show4DIndicators;
        lastIndicatorToggle = glfwGetTime();
    }

    if (glfwGetTime() > lastPathShowChange + 0.2) {
        if (inputManager.getKeyPressed(GLFW_KEY_P)) {
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
        if (renderedPath.pathSize() > 0 && showPath) {
            if (renderedPath.visitedCell(coords)) {
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
    //TODO: store these vecs in a lookup buffer to save performance and memory?

    glm::vec3 coords = coordsFromMazeCenter(mazeX, mazeY, mazeZ);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), coords);
    glm::mat4 view = camera->getView();

    glm::vec3 lightPos = camera->getCoords();
    glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);

    prepMazeCenterDraw(model, view, getCellColour({ mazeX, mazeY, mazeZ, mazeW }));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //unbind VA
    glBindVertexArray(0);
}

void MazeRenderer::drawMazeCellPaths(unsigned char mazeCellData, int mazeX, int mazeY, int mazeZ, int mazeW, int lastW, float transitionScale) {
    std::vector<int> mazeCoords = { mazeX, mazeY, mazeZ, mazeW };
    unsigned char prevWData = (*maze)[{mazeX, mazeY, mazeZ, lastW}];
    glm::vec3 modelCoords = coordsFromMazeCenter(mazeX, mazeY, mazeZ);
    //translation to get it to the same modelCoords as the center piece, from which we then translate it again into the proper position
    glm::mat4 initialTranslate = glm::translate(glm::mat4(1.0f), modelCoords);

    glm::vec3 lightPos = camera->getCoords();

    useMazePathProgram(lightPos, defaultLightColour);

    glBindVertexArray(mazePathVAO);

    for (unsigned int i = 0; i < 8; i++) {
        if (!drawMazeCellPath(mazeCellData, prevWData, i, initialTranslate, modelCoords, mazeCoords, transitionScale)) {
            break;
        }
    }

    glBindVertexArray(0);
}

bool MazeRenderer::drawMazeCellPath(unsigned char mazeCellData, unsigned char prevWData, unsigned int cellPath, const glm::mat4& initialTranslate, const glm::vec3& modelCoords, const std::vector<int>& mazeCoords, float transitionScale) {
    unsigned char bitChecking = 1 << cellPath;

    if (hasCellPathBit(mazeCellData, bitChecking)) {
        float adjustedScale = calculateAdjustedScale(prevWData, mazeCellData, bitChecking, cellPath, transitionScale);

        //TODO: move all matrix multiplications into the shaders?
        //we can scale it like this because the path is the exact same for all directions until the maze cell path transform step
        //scale it in the +z direction as that is where the path 'points' by default
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, adjustedScale));
        glm::mat4 model = mazeCellPathTransform(modelCoords, cellPathTransformations[cellPath]) * initialTranslate * scale;
        glm::vec3 cellColour = getCellColour(mazeCoords);
        glm::mat3 normalTransform = calculateNormalTransform(model);

        if (cellPath >= 6) {
            if (!show4DIndicators) {
                return false;
            }

            cellColour = bitChecking == ANA ? anaColour : kataColour;
        }

        prepMazeDrawUniforms(cellColour, model, normalTransform);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    return true;
}

float MazeRenderer::calculateScale(float endTransitionTime, float now, float mazeTransitionAnimationSpeed) {
    return std::clamp<float>(1.0f - ((endTransitionTime - now) / mazeTransitionAnimationSpeed), 0.0f, 1.0f);
}

glm::vec3 MazeRenderer::coordsFromMazeCenter(int mazeX, int mazeY, int mazeZ) {
    return glm::vec3(
        (maze->width - mazeX) + mazeCenterX,
        mazeY + mazeCenterY,
        mazeZ + mazeCenterZ
    );;
}

void MazeRenderer::prepMazeCenterDraw(const glm::mat4& model, const glm::mat4& view, const glm::vec3& cellColour) {
    glUseProgram(cellCenterProgram);

    glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(cellCenterProgram, "projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));

    glUniform3fv(glGetUniformLocation(cellCenterProgram, "cellColour"), 1, glm::value_ptr(cellColour));
    glUniform3fv(glGetUniformLocation(cellCenterProgram, "lightPos"), 1, glm::value_ptr(camera->getCoords()));
    glUniform3fv(glGetUniformLocation(cellCenterProgram, "lightColour"), 1, glm::value_ptr(defaultLightColour));

    glBindVertexArray(mazeCenterVAO);
}

void MazeRenderer::useMazePathProgram(const glm::vec3& lightPos, const glm::vec3& lightColour) {
    glUseProgram(mazePathProgram);

    glUniform3fv(glGetUniformLocation(mazePathProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(mazePathProgram, "lightColour"), 1, glm::value_ptr(defaultLightColour));

    glUniformMatrix4fv(glGetUniformLocation(mazePathProgram, "view"), 1, GL_FALSE, glm::value_ptr(camera->getView()));
    glUniformMatrix4fv(glGetUniformLocation(mazePathProgram, "projection"), 1, GL_FALSE, glm::value_ptr(camera->getProjection()));
}

glm::mat3 MazeRenderer::calculateNormalTransform(const glm::mat4& model) {
    //perform this out of the shader as inverse is bad on the GPU due to performance
    return glm::transpose(glm::inverse(glm::mat3(model)));
}

void MazeRenderer::prepMazeDrawUniforms(const glm::vec3& cellColour, const glm::mat4& model, const glm::mat3& normalTransform) {
    glUniform3fv(glGetUniformLocation(mazePathProgram, "cellColour"), 1, glm::value_ptr(cellColour));

    glUniformMatrix4fv(glGetUniformLocation(mazePathProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix3fv(glGetUniformLocation(mazePathProgram, "normalTransform"), 1, GL_FALSE, glm::value_ptr(normalTransform));
}

inline float MazeRenderer::calculateAdjustedScale(unsigned char prevWData, unsigned char mazeCellData, unsigned char bitChecking, int i, float transitionScale) {
    //TODO: uuuuh wait doesn't this mean that if they're different the transitionScale is 0 so the path doesn't display?
        /*
        * 1111000 - a
        * 0001111 - b
        * &
        * 0001000 - bit checking
        * =
        * 0001000
        * >> 3 =
        * 0000001
        *
        */
    return ((prevWData & mazeCellData & bitChecking) >> i) == 1 ? 1.0f : transitionScale;
}

inline bool MazeRenderer::hasCellPathBit(unsigned char mazeCellData, unsigned char bitChecking) {
    return (mazeCellData & bitChecking) > 0;
}
