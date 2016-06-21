#pragma once
#include "../../Common.h"

#include "../../Image/Image.h"
#include "../../Image/ImageBMP.h"

#include "../../Graphic/Buffer/ConstantBuffer.h"

#include "../../Math/Math.h"

#include "../../Graphic/Vertex.h"
#include "../../Core/GameEntity.h"
#include "../../Game/Entities/CubeEntity.h"

#include <vulkan/vulkan.h>

// Libraries
#pragma comment (lib, "vulkan-1.lib")

namespace Renderer
{
	class VulkanFrameBuffer;

	class VulkanRenderer : public IWindowRenderer
	{
	private:
		VkApplicationInfo ApplicationInfo;
		VkInstance Instance;
		VkInstanceCreateInfo InstanceInfo;

		uint32_t DevicesCount = 0;
		VkPhysicalDevice* Devices;
		VkPhysicalDevice* PrimaryDevice;

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
		void FindPrimaryDevice();
		void OutputDeviceProperties(VkPhysicalDevice* device);

	protected:
		void CreateShaderForEntity(Core::GameEntity* entity) override {};
		void PrepareBuffers() override {};
	};
}