#include "VulkanRenderer.h"

using namespace Renderer;

VulkanRenderer::VulkanRenderer()
{
}

VulkanRenderer::~VulkanRenderer()
{
	vkDestroyInstance(Instance, nullptr);
	
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

	VkResult instanceResult = vkCreateInstance(&InstanceInfo, nullptr, &Instance);

	assert(instanceResult == VK_SUCCESS);

	FindPrimaryDevice();

	assert(PrimaryDevice != nullptr);

#if IS_DEBUG
	OutputDeviceProperties(PrimaryDevice);
#endif
}

void VulkanRenderer::FindPrimaryDevice()
{
	VkResult devicesResult = vkEnumeratePhysicalDevices(Instance, &DevicesCount, nullptr);
	
	assert(devicesResult == VK_SUCCESS);
	assert(DevicesCount > 0);

	Devices = new VkPhysicalDevice[DevicesCount];
	devicesResult = vkEnumeratePhysicalDevices(Instance, &DevicesCount, Devices);

	// Set first device as primary
	PrimaryDevice = &Devices[0];
}

void VulkanRenderer::OutputDeviceProperties(VkPhysicalDevice* device)
{
	VkPhysicalDeviceProperties properties;
	ZeroMemory(&properties, sizeof(VkPhysicalDeviceProperties));

	vkGetPhysicalDeviceProperties(*PrimaryDevice, &properties);

	Log::Write("VulkanRenderer", "Driver Version: %d", Log::Severity::Notice, properties.driverVersion);
	Log::Write("VulkanRenderer", "Device Name: %s", Log::Severity::Notice, properties.deviceName);
	Log::Write("VulkanRenderer", "Device Type: %d", Log::Severity::Notice, properties.deviceType);
	Log::Write("VulkanRenderer", "API Version: %d.%d.%d", Log::Severity::Notice,
		(properties.apiVersion >> 22) & 0x3FF,
		(properties.apiVersion >> 12) & 0x3FF,
		(properties.apiVersion & 0xFFF));
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