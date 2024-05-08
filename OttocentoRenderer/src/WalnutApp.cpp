#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include "Camera.h"
#include "Renderer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
ExampleLayer() : m_Camera(45.0f, 0.1f, 100.0f) {}
//----------------------------------------------------------------------------
virtual void OnUpdate(float ts) override
{
	m_Camera.OnUpdate(ts);
}

//----------------------------------------------------------------------------
virtual void OnUIRender() override
{
	ImGui::Begin("Settings");
	ImGui::Text("Last Render: %.3fms", m_LastRenderTime);
	ImGui::SliderFloat("Light.X", &LightPos.x, 0.0f, 1.0f);
	ImGui::SliderFloat("Light.Y", &LightPos.y, 0.0f, 1.0f);
	ImGui::SliderFloat("Light.Z", &LightPos.z, 0.0f, 1.0f);
	if (ImGui::Button("Render"))
	{
		Render();
	}
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport");

	m_ViewportWidth = ImGui::GetContentRegionAvail().x;
	m_ViewportHeight = ImGui::GetContentRegionAvail().y;

	auto image = m_Renderer.GetFinalImage();
	if (image)
		ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0,1), ImVec2(1,0));

	ImGui::End();
	ImGui::PopStyleVar();

	Render();
}

//----------------------------------------------------------------------------
void Render()
{
	Timer timer;

	m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
	m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
	m_Renderer.Render(m_Camera);

	m_LastRenderTime = timer.ElapsedMillis();
}
private:
	Renderer m_Renderer;
	Camera m_Camera;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_LastRenderTime = 0.0f;
	glm::vec3 LightPos;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ottocento Renderer";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}