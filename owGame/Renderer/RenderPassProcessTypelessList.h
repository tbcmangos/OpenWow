#pragma once

class ZN_API CRenderPassProcessTypelessList
	: public RenderPass
	, public IVisitor
{
public:
	CRenderPassProcessTypelessList(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& CreateTypelessList);
	virtual ~CRenderPassProcessTypelessList();

	// IRenderPassPipelined
	void Render(RenderEventArgs& e);

	// IVisitor
	virtual EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) override;
	virtual EVisitResult Visit(const std::shared_ptr<IUIControl>& UIControl) override;
	virtual EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
	virtual EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs) override;
	virtual EVisitResult Visit(const std::shared_ptr<ILight>& light) override;
	virtual EVisitResult Visit(const std::shared_ptr<IParticleSystem>& ParticleSystem) override;
	virtual EVisitResult Visit(const std::shared_ptr<ITerrain>& Terrain) override;

protected:
	const std::shared_ptr<IRenderPassCreateTypelessList>& GetSceneNodeListPass() const;

protected:
	std::shared_ptr<IRenderPassCreateTypelessList> m_CreateTypelessList;
};