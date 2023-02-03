#include "VBOConfiguration.h"

bool VBOConfiguration::operator==(VBOConfiguration config) {
    return config.size == this->size && config.normalize == this->normalize;
}
