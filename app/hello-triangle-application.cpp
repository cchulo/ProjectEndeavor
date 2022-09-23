#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

const uint32_t kWidth = 800;
const uint32_t kHeight = 600;
const std::vector<const char *> kValidationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const bool kEnableValidationLayers =
#ifdef NDEBUG
    false;
#else
    true;
#endif

class HelloTriangleApplication {
 public:
  void Run() {
    InitWindow();
    InitVulkan();
    MainLoop();
    Cleanup();
  }

 private:

  GLFWwindow *_window;
  VkInstance _instance;
  void InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = glfwCreateWindow(kWidth, kHeight, "Vulkan", nullptr, nullptr);
  }

  void CreateInstance() {
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Hello Triangle";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions;
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    std::cout << "available extensions:\n";

    for (const auto &kExtension : extensions) {
      std::cout << '\t' << kExtension.extensionName << '\n';
    }

    create_info.enabledExtensionCount = glfw_extension_count;
    create_info.ppEnabledExtensionNames = glfw_extensions;

    create_info.enabledLayerCount = 0;

    if (vkCreateInstance(&create_info, nullptr, &_instance) != VK_SUCCESS) {
      throw std::runtime_error("failed to create instance!");
    }
  }

  bool CheckValidationLayerSupport() {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
    bool layer_found = false;

    for (const char *layer_name : kValidationLayers) {
      for (const auto &layer_properties : available_layers) {
        if (strcmp(layer_name, layer_properties.layerName) == 0) {
          layer_found = true;
          break;
        }
      }
    }

    return layer_found;
  }

  void InitVulkan() {
    CreateInstance();
  }

  void MainLoop() {
    while (!glfwWindowShouldClose(_window)) {
      glfwPollEvents();
    }
  }

  void Cleanup() {
    vkDestroyInstance(_instance, nullptr);
    glfwDestroyWindow(_window);
    glfwTerminate();
  }
};

int main() {
  HelloTriangleApplication app{};

  try {
    app.Run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

#pragma clang diagnostic pop