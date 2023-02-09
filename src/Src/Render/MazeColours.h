#include <glm/glm.hpp>

#pragma once

class MazeColours {
private:
	constexpr static glm::vec3 hexColour(const int hex) {
		int red, green, blue;
		blue = hex & 0xff;
		green = (hex & 0xff00) >> 8;
		red = (hex & 0xff000) >> 16;

		return glm::vec3(red / 255.0f, green / 255.0f, blue / 255.0f);
	}

public:
	//blue #0000ff
	static constexpr glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	//red #ff0000
	static constexpr glm::vec3 backward = glm::vec3(1.0f, 0.0f, 0.0f);
	//green #00ff00
	static constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//yellow #ffff00
	static constexpr glm::vec3 down = glm::vec3(1.0f, 1.0f, 0.0f);
	//aqua #00ffff
	static constexpr glm::vec3 left = glm::vec3(0.0f, 1.0f, 1.0f);
	//brown #8b4513
	static constexpr glm::vec3 right = glm::vec3(0.545f, 0.271f, 0.075f);


	//yellowy #e5cb55
	static constexpr glm::vec3 ana = glm::vec3(0.901f, 0.796f, 0.333f);
	//bluey #656ac9
	static constexpr glm::vec3 kata = glm::vec3(0.396f, 0.415f, 0.788f);

	//grey #8a8a8a
	static constexpr glm::vec3 defaultColour = glm::vec3(0.54f, 0.54f, 0.54f);
	//purpley #b103fc
	static constexpr glm::vec3 visitedColour = glm::vec3(0.694f, 0.0117f, 0.988f);

	//medium spring green #00ffa9
	static constexpr glm::vec3 entranceColour = glm::vec3(0.000f, 0.980f, 0.604f);
	//crimson #dc143c
	static constexpr glm::vec3 exitColour = glm::vec3(0.863f, 0.078f, 0.235f);

};