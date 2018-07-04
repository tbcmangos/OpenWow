#include "stdafx.h"

// General
#include "BindingController.h"

CBindingController::CBindingController()
{
	m_InputListenerObjectCollection = new CInputListenerObjectCollection();
	m_Renderable3DObjectCollection = new CRenderable3DObjectCollection();
	m_RenderableUIObjectCollection = new CRenderableUIObjectCollection();
	m_UpdatableObjectCollection = new CUpdatableObjectCollection();
}

CBindingController::~CBindingController()
{
	OutputDebugString(L"CBindingController destroyed.\n");
}

void CBindingController::RegisterInputListener(IInputListener* _object, uint8 _priority)
{
	m_InputListenerObjectCollection->RegisterObject(_object, _priority);
}

void CBindingController::RegisterRenderable3DObject(IRenderable3D* _object, uint32 _DrawOrder)
{
	m_Renderable3DObjectCollection->RegisterObject(_object, _DrawOrder);
}

void CBindingController::RegisterRenderableUIObject(IRenderableUI* _object, uint32 _DrawOrder)
{
	m_RenderableUIObjectCollection->RegisterObject(_object, _DrawOrder);
}

void CBindingController::RegisterUpdatableObject(IUpdatable* _object)
{
	m_UpdatableObjectCollection->RegisterObject(_object);
}

void CBindingController::UnregisterInputListener(IInputListener * _object)
{
	m_InputListenerObjectCollection->UnregisterObject(_object);
}

void CBindingController::UnregisterRenderable3DObject(IRenderable3D* _object)
{
	m_Renderable3DObjectCollection->UnregisterObject(_object);
}

void CBindingController::UnregisterRenderableUIObject(IRenderableUI* _object)
{
	m_RenderableUIObjectCollection->UnregisterObject(_object);
}

void CBindingController::UnregisterUpdatableObject(IUpdatable * _object)
{
	m_UpdatableObjectCollection->UnregisterObject(_object);
}