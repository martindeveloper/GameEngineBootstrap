#if RENDERER_VULKAN

#include "VulkanRenderer.h"

using namespace Renderer;

VulkanRenderer::VulkanRenderer()
{
	Swapchain = nullptr;
}

VulkanRenderer::~VulkanRenderer()
{
	vkDeviceWaitIdle(PrimaryDevice);
	vkDestroyDevice(PrimaryDevice, nullptr);

	vkDestroyInstance(Instance, nullptr);

	delete[] PhysicalDevices;
	delete Swapchain;
}

void VulkanRenderer::BeforeStart(HDC WindowDeviceContext, const bool isWindowed)
{
	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.pNext = nullptr;
	ApplicationInfo.pApplicationName = "";
	ApplicationInfo.pEngineName = nullptr;
	ApplicationInfo.engineVersion = 1;
	ApplicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 13);

	InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	InstanceInfo.pNext = nullptr;
	InstanceInfo.flags = NULL;
	InstanceInfo.pApplicationInfo = &ApplicationInfo;
	InstanceInfo.enabledLayerCount = 0;
	InstanceInfo.ppEnabledLayerNames = nullptr;
	InstanceInfo.enabledExtensionCount = 0;
	InstanceInfo.ppEnabledExtensionNames = nullptr;

	std::vector<const char*> enabledExtensions;
	enabledExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#ifdef _WIN32
	enabledExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

	InstanceInfo.enabledExtensionCount = enabledExtensions.size();
	InstanceInfo.ppEnabledExtensionNames = &enabledExtensions[0];

	VkResult instanceResult = vkCreateInstance(&InstanceInfo, nullptr, &Instance);

	assert(instanceResult == VK_SUCCESS);

	FindAndSetPrimaryPhysicalDevice();

	assert(PrimaryPhysicalDevice != nullptr);

#if IS_DEBUG
	OutputDeviceProperties(PrimaryPhysicalDevice);
#endif

	CreateDevice();
	CreateSwapchain(WindowDeviceContext);
}

void VulkanRenderer::FindAndSetPrimaryPhysicalDevice()
{
	VkResult devicesResult = vkEnumeratePhysicalDevices(Instance, &DevicesCount, nullptr);
	
	assert(devicesResult == VK_SUCCESS);
	assert(DevicesCount > 0);

	PhysicalDevices = new VkPhysicalDevice[DevicesCount];
	devicesResult = vkEnumeratePhysicalDevices(Instance, &DevicesCount, PhysicalDevices);

	// Set first device as primary
	PrimaryPhysicalDevice = &PhysicalDevices[0];
}

void VulkanRenderer::OutputDeviceProperties(VkPhysicalDevice* device)
{
	VkPhysicalDeviceProperties properties;
	ZeroMemory(&properties, sizeof(VkPhysicalDeviceProperties));

	vkGetPhysicalDeviceProperties(*PrimaryPhysicalDevice, &properties);

	Log::Write("VulkanRenderer", "Driver Version: %d", Log::Severity::Notice, properties.driverVersion);
	Log::Write("VulkanRenderer", "Device Name: %s", Log::Severity::Notice, properties.deviceName);
	Log::Write("VulkanRenderer", "Device Type: %d", Log::Severity::Notice, properties.deviceType);
	Log::Write("VulkanRenderer", "API Version: %d.%d.%d", Log::Severity::Notice,
		VK_VER_MAJOR(properties.apiVersion),
		VK_VER_MINOR(properties.apiVersion),
		VK_VER_PATCH(properties.apiVersion));
}

void Renderer::VulkanRenderer::CreateDevice()
{
	VkDeviceCreateInfo info;

	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.pNext = nullptr;
	info.flags = NULL;
	info.enabledLayerCount = 0;
	info.ppEnabledLayerNames = nullptr;
	info.enabledExtensionCount = 0;
	info.ppEnabledExtensionNames = nullptr;
	info.pEnabledFeatures = nullptr;

	VkDeviceQueueCreateInfo queueInfo;
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext = nullptr;
	queueInfo.flags = NULL;
	queueInfo.queueFamilyIndex = 0;

	float queuePriorities[] = { 1.0f };
	queueInfo.queueCount = 1;
	queueInfo.pQueuePriorities = queuePriorities;

	info.queueCreateInfoCount = 1;
	info.pQueueCreateInfos = &queueInfo;

	VkResult result = vkCreateDevice(*PrimaryPhysicalDevice, &info, nullptr, &PrimaryDevice);

	assert(result == VK_SUCCESS);
}

void VulkanRenderer::CreateSwapchain(HDC hdc)
{
	assert(Swapchain == nullptr);

	Swapchain = new VulkanSwapchain(this, hdc);

	Swapchain->Initialize();
}

void VulkanRenderer::ClearWindow(const double deltaTime)
{

}

void VulkanRenderer::Update(const double deltaTime)
{

}

void VulkanRenderer::Render(const double deltaTime)
{

}

#endif
