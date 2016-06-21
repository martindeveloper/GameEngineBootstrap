#pragma once

#include "../../Common.h"
#include "VulkanRenderer.h"

namespace Renderer
{
	class VulkanRenderer;

	class VulkanSwapchain
	{
	private:
		VulkanRenderer* Renderer;

#ifdef _WIN32
		HINSTANCE Handle;
		HWND WindowHandle;
#endif
	public:
		VkColorSpaceKHR ColorSpace;
		VkFormat ColorFormat;
		VkSurfaceKHR Surface;

	public:
		VulkanSwapchain(VulkanRenderer* renderer, void* deviceContext);
		~VulkanSwapchain();

		void Initialize();
	};
}