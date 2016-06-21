#include "VulkanSwapchain.h"

using namespace Renderer;

VulkanSwapchain::VulkanSwapchain(VulkanRenderer* renderer, void* deviceContext)
{
	Handle = (HINSTANCE)deviceContext;
	WindowHandle = WindowFromDC((HDC)deviceContext);
	Renderer = renderer;
}

VulkanSwapchain::~VulkanSwapchain()
{

}

void VulkanSwapchain::Initialize()
{
	VkResult result = VK_SUCCESS;

#ifdef _WIN32
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo;
	ZeroMemory(&surfaceCreateInfo, sizeof(VkWin32SurfaceCreateInfoKHR));

	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hinstance = Handle;
	surfaceCreateInfo.hwnd = WindowHandle;

	result = vkCreateWin32SurfaceKHR(Renderer->Instance, &surfaceCreateInfo, nullptr, &Surface);
#elif
#pragma error("Implement next platform here");
#endif

	assert(result == VK_SUCCESS);

	uint32_t formatCount = 0;

	result = vkGetPhysicalDeviceSurfaceFormatsKHR(*(Renderer->PrimaryPhysicalDevice), Surface, &formatCount, nullptr);

	assert(result == VK_SUCCESS);

	std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
	result = vkGetPhysicalDeviceSurfaceFormatsKHR(*(Renderer->PrimaryPhysicalDevice), Surface, &formatCount, &surfaceFormats[0]);

	assert(result == VK_SUCCESS);

	if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else
	{
		assert(formatCount >= 1);
		ColorFormat = surfaceFormats[0].format;
	}

	ColorSpace = surfaceFormats[0].colorSpace;
}