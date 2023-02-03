#ifndef CD3_RENDER_VBO_CONFIG
#define CD3_RENDER_VBO_CONFIG

#pragma once
struct VBOConfiguration
{
public:
	int size;
	bool normalize;
	//stride and offset will be calculated when the configuration is added - offset by the order this is added and the size of previous configurations

	VBOConfiguration(int size, bool normalize = false) : size(size), normalize(normalize) {}
	bool operator==(VBOConfiguration config);
};

#endif
