#include "Application.Renderer.h"
#include <map>
#include "Application.UIState.h"
#include "Graphics.Layouts.h"
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
	void Render(SDL_Renderer* renderer)
	{
		auto layoutName = ::application::Renderer::renderLayouts.find(::application::UIState::Read());
		if (layoutName != ::application::Renderer::renderLayouts.end())
		{
			graphics::Layouts::Draw(renderer, layoutName->second);
		}
	}
}
