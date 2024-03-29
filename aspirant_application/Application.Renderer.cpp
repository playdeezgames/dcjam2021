#include "Application.Renderer.h"
#include <map>
#include "Application.UIState.h"
#include "Visuals.Layouts.h"
namespace application::Renderer
{
	static std::map<::UIState, std::string> renderLayouts;

	void SetRenderLayout(const ::UIState& state, const std::string& layoutName)
	{
		renderLayouts[state] = layoutName;
	}

}
namespace common::Application
{
	void Render(const std::shared_ptr<SDL_Renderer>& renderer)
	{
		auto layoutName = ::application::Renderer::renderLayouts.find(::application::UIState::Read());
		if (layoutName != ::application::Renderer::renderLayouts.end())
		{
			visuals::Layouts::Draw(renderer, layoutName->second);
		}
	}
}
