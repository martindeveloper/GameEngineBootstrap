#pragma once
#include "../../Common.h"

#include "../../Image/Image.h"
#include "../../Image/ImageBMP.h"

#include "../../Graphic/Buffer/ConstantBuffer.h"

#include "../../Math/Math.h"

#include "../../Graphic/Vertex.h"
#include "../../Core/GameEntity.h"
#include "../../Game/Entities/CubeEntity.h"

#define VK_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.h>

#include "VulkanSwapchain.h"

// Libraries
#pragma comment (lib, "vulkan-1.lib")

#define VK_VER_MAJOR(X) ((((uint32_t)(X))>>22)&0x3FF)
#define VK_VER_MINOR(X) ((((uint32_t)(X))>>12)&0x3FF)
#define VK_VER_PATCH(X) (((uint32_t)(X)) & 0xFFF)

namespace Renderer
{
	class VulkanRenderer : public IWindowRenderer
	{
		friend class VulkanSwapchain;

	private:
		VkApplicationInfo ApplicationInfo;
		VkInstance Instance;
		VkInstanceCreateInfo InstanceInfo;

		uint32_t DevicesCount = 0;
		VkPhysicalDevice* PhysicalDevices;
		VkPhysicalDevice* PrimaryPhysicalDevice;
		VkDevice* PrimaryDevice;

		VulkanSwapchain* Swapchain;

	public:
		VulkanRenderer();
		virtual ~VulkanRenderer();

		void BeforeStart(HDC WindowDeviceContext, const bool isWindowed) override;
		void ClearWindow(const double deltaTime) override;
		void Update(const double deltaTime) override;
		void Render(const double deltaTime) override;

		#pragma message("TODO: Implement CreateMaterial, UploadTexture, CreateShaderForEntity and PrepareBuffers in VulkanRenderer")

		Renderer::Material* CreateMaterial() override { return nullptr; };
		void UploadTexture(Core::GameEntity* entity, Image::Image* image) override {};

	private:
		void CreateDevice();
		void FindAndSetPrimaryPhysicalDevice();
		void OutputDeviceProperties(VkPhysicalDevice* device);
		void CreateSwapchain(HDC hdc);

	protected:
		void CreateShaderForEntity(Core::GameEntity* entity) override {};
		void PrepareBuffers() override {};
	};
}