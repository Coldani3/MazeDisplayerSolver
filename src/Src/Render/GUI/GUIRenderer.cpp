#include <Render/GUI/GUIRenderer.h>

GUIRenderer::GUIRenderer(std::shared_ptr<TwoDCamera> camera) {
    this->camera = camera;
}

/*Coordinates are provided the same as if they were drawing directly to a square screen via OpenGL, so -1 to 1 on both
  2D axes. It's a bit weird and unnecessary given that the ortho transformation just ends up doing this anyways but it's
  either this or awful hardcoded dimensions and an unresizeable window. Don't use this for sizes though, that will cause
  your stuff to be skewed. */
glm::vec3 GUIRenderer::screenRelativeCoords(float x, float y) {
    float outX = ((x / 2) + 0.5f) * camera->screenWidth;
    float outY = ((y / 2) + 0.5f) * camera->screenHeight;
    glm::vec3 out = glm::vec3(outX, outY, 0.0f);
    return out;
}
