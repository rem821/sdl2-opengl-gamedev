//
// Created by standa on 4.3.23.
//
#pragma once

#include <precompiled_headers/PCH.h>

namespace VulkanEngine {

    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

        std::vector<Layer*>::iterator begin() { return layers_.begin(); }
        std::vector<Layer*>::iterator end() { return layers_.end(); }
    private:
        std::vector<Layer*> layers_;
        unsigned int layerInsertIndex_ = 0;
    };
}