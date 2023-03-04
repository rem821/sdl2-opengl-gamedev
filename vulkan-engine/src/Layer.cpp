//
// Created by standa on 4.3.23.
//
#include <Layer.h>

namespace VulkanEngine {
    Layer::Layer(std::string name) : debugName_(std::move(name)) {

    }

    Layer::~Layer() = default;

}