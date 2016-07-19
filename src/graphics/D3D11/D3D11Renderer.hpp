//-----------------------------------------------------------------------------
// Copyright (c) 2016, Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the project nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef _GRAPHICS_D3D11_D3D11RENDERER_HPP_
#define _GRAPHICS_D3D11_D3D11RENDERER_HPP_

#include <d3d11.h>
#include "graphics/renderer.hpp"
#include "game/camera.hpp"

class D3D11Renderer : public Renderer {
public:
	virtual void initRenderer() override;
	
	virtual void destroyRenderer() override;
	
	virtual void beginFrame() override;
	
	virtual void renderChunks() override;
	
	virtual void endFrame() override;
	
	virtual void renderSingleCube() override;
	
	virtual void setActiveSceneCamera(Camera *camera) override;

	void swapBuffers();
	void setWindowHandle(HWND window);
	
protected:
	Camera *mCamera;

	HWND mWindow;
	IDXGISwapChain *mSwapChain;
	ID3D11Device *mDevice;
	ID3D11DeviceContext *mContext;
	ID3D11RenderTargetView *mRenderTargetView;
};

#endif // _GRAPHICS_D3D11_D3D11RENDERER_HPP_