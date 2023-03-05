//
// Created by standa on 4.3.23.
//
#include <LayerStack.h>


namespace VulkanEngine {
    LayerStack::LayerStack() = default;

    LayerStack::~LayerStack() {
        for (Layer *layer: layers_)
            delete layer;
    }

    void LayerStack::PushLayer(Layer *layer) {
        layers_.emplace(layers_.begin() + layerInsertIndex_, layer);
        layerInsertIndex_++;
    }

    void LayerStack::PushOverlay(Layer *layer) {
        layers_.emplace_back(layer);
    }

    void LayerStack::PopLayer(Layer *layer) {
        auto it = std::find(layers_.begin(), layers_.end(), layer);
        if (it != layers_.end()) {
            layers_.erase(it);
            layerInsertIndex_--;
        }
    }

    void LayerStack::PopOverlay(Layer *layer) {
        auto it = std::find(layers_.begin(), layers_.end(), layer);
        if (it != layers_.end())
            layers_.erase(it);
    }
}