#include "StdAfx.h"
#include "InstanceBase.h"

CActorInstance::IEventHandler& CInstanceBase::GetEventHandlerRef()
{
	return m_GraphicThingInstance.__GetEventHandlerRef();
}

CActorInstance::IEventHandler* CInstanceBase::GetEventHandlerPtr()
{
	return m_GraphicThingInstance.__GetEventHandlerPtr();
}

void CInstanceBase::SetEventHandler(CActorInstance::IEventHandler* pkEventHandler)
{
	m_GraphicThingInstance.SetEventHandler(pkEventHandler);
}
//martysama0134's cc449580f8a0ea79d66107125c7ee3d3
