//
// Created by Stanislav Svědiroh on 15.06.2022.
//

#include "VulkanEngineDescriptors.h"

// *************** Descriptor Set Layout Builder *********************

VulkanEngineDescriptorSetLayout::Builder
&VulkanEngineDescriptorSetLayout::Builder::addBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count) {
    assert(bindings.count(binding) == 0 && "Binding already in use");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = count;
    layoutBinding.stageFlags = stageFlags;
    bindings[binding] = layoutBinding;
    return *this;
}

std::unique_ptr<VulkanEngineDescriptorSetLayout> VulkanEngineDescriptorSetLayout::Builder::build() const {
    return std::make_unique<VulkanEngineDescriptorSetLayout>(engineDevice, bindings);
}

// *************** Descriptor Set Layout *********************

VulkanEngineDescriptorSetLayout::VulkanEngineDescriptorSetLayout(VulkanEngineDevice &device,
                                                                 std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings) : engineDevice{device},
                                                                                                                                        bindings{bindings} {
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
    for (auto kv: bindings) {
        setLayoutBindings.push_back(kv.second);
    }

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
    descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutInfo.bindingCount = static_cast<uint32_t>(setLayoutBindings.size());
    descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(
            engineDevice.device(),
            &descriptorSetLayoutInfo,
            nullptr,
            &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

VulkanEngineDescriptorSetLayout::~VulkanEngineDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(engineDevice.device(), descriptorSetLayout, nullptr);
}

// *************** Descriptor Pool Builder *********************

VulkanEngineDescriptorPool::Builder &VulkanEngineDescriptorPool::Builder::addPoolSize(
        VkDescriptorType descriptorType, uint32_t count) {
    poolSizes.push_back({descriptorType, count});
    return *this;
}

VulkanEngineDescriptorPool::Builder &VulkanEngineDescriptorPool::Builder::setPoolFlags(
        VkDescriptorPoolCreateFlags flags) {
    poolFlags = flags;
    return *this;
}

VulkanEngineDescriptorPool::Builder &VulkanEngineDescriptorPool::Builder::setMaxSets(uint32_t count) {
    maxSets = count;
    return *this;
}

std::unique_ptr<VulkanEngineDescriptorPool> VulkanEngineDescriptorPool::Builder::build() const {
    return std::make_unique<VulkanEngineDescriptorPool>(engineDevice, maxSets, poolFlags, poolSizes);
}

// *************** Descriptor Pool *********************

VulkanEngineDescriptorPool::VulkanEngineDescriptorPool(VulkanEngineDevice &device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags,
                                                       const std::vector<VkDescriptorPoolSize> &poolSizes) : engineDevice{device} {
    VkDescriptorPoolCreateInfo descriptorPoolInfo{};
    descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    descriptorPoolInfo.pPoolSizes = poolSizes.data();
    descriptorPoolInfo.maxSets = maxSets;
    descriptorPoolInfo.flags = poolFlags;

    if (vkCreateDescriptorPool(engineDevice.device(), &descriptorPoolInfo, nullptr, &descriptorPool) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

VulkanEngineDescriptorPool::~VulkanEngineDescriptorPool() {
    vkDestroyDescriptorPool(engineDevice.device(), descriptorPool, nullptr);
}

bool VulkanEngineDescriptorPool::allocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const {
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.pSetLayouts = &descriptorSetLayout;
    allocInfo.descriptorSetCount = 1;

    // Might want to create a "DescriptorPoolManager" class that handles this case, and builds
    // a new pool whenever an old pool fills up. But this is beyond our current scope
    if (vkAllocateDescriptorSets(engineDevice.device(), &allocInfo, &descriptor) != VK_SUCCESS) {
        return false;
    }
    return true;
}

void VulkanEngineDescriptorPool::freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const {
    vkFreeDescriptorSets(
            engineDevice.device(),
            descriptorPool,
            static_cast<uint32_t>(descriptors.size()),
            descriptors.data());
}

void VulkanEngineDescriptorPool::resetPool() {
    vkResetDescriptorPool(engineDevice.device(), descriptorPool, 0);
}

// *************** Descriptor Writer *********************

VulkanEngineDescriptorWriter::VulkanEngineDescriptorWriter(VulkanEngineDescriptorSetLayout &setLayout, VulkanEngineDescriptorPool &pool)
        : setLayout{setLayout}, pool{pool} {}

VulkanEngineDescriptorWriter &VulkanEngineDescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo) {
    assert(setLayout.getBindings().count(binding) == 1 && "Layout does not contain specified binding");

    auto &bindingDescription = setLayout.getBindingAt(binding);

    assert(
            bindingDescription.descriptorCount == 1 &&
            "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pBufferInfo = bufferInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

VulkanEngineDescriptorWriter &VulkanEngineDescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo) {
    assert(setLayout.getBindings().count(binding) == 1 && "Layout does not contain specified binding");

    auto &bindingDescription = setLayout.getBindingAt(binding);

    assert(
            bindingDescription.descriptorCount == 1 &&
            "Binding single descriptor info, but binding expects multiple");

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.descriptorType = bindingDescription.descriptorType;
    write.dstBinding = binding;
    write.pImageInfo = imageInfo;
    write.descriptorCount = 1;

    writes.push_back(write);
    return *this;
}

bool VulkanEngineDescriptorWriter::build(VkDescriptorSet &set) {
    bool success = pool.allocateDescriptor(setLayout.getDescriptorSetLayout(), set);
    if (!success) {
        return false;
    }
    overwrite(set);
    return true;
}

void VulkanEngineDescriptorWriter::overwrite(VkDescriptorSet &set) {
    for (auto &write: writes) {
        write.dstSet = set;
    }
    vkUpdateDescriptorSets(pool.getEngineDevice().device(), writes.size(), writes.data(), 0, nullptr);
}

