#include <stdafx.h>

// General
#include "TransparentPass.h"

TransparentPass::TransparentPass(std::shared_ptr<Scene> scene, std::shared_ptr<PipelineState> pipeline)
	: base(scene, pipeline)
{}

TransparentPass::~TransparentPass()
{}

void TransparentPass::Visit(IMesh& mesh)
{
	std::shared_ptr<Material> pMaterial = mesh.GetMaterial();
	if (pMaterial /*&& pMaterial->IsTransparent()*/)
	{
		mesh.Render(GetRenderEventArgs(), nullptr);
	}
}
