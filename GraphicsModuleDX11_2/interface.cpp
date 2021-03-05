#include "pch.h"
#include "interface.h"
#include "GraphicsModule.hpp"
#include "helperClasses.hpp"
HRESULT __stdcall CreateGraphicsModule(IGraphicsModule** out)
{
    if (out == nullptr)
    {
        return E_POINTER;
    }
    *out = new RefCountImpl<GraphicsModule, IGraphicsModule, IUnknown>();
    return S_OK;
}
