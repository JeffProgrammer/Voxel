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

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/D3D11/D3D11Renderer.hpp"
#include "core/cube.hpp"
#include "game/camera.hpp"

const char *vertCubeSrc =
	"struct Input {"
	"   float3 position : POSITION;"
	"};"

	"struct Output {"
	"   float4 position : SV_POSITION;"
	"   float4 color : COLOR;"
	"};"

	"cbuffer matrices : register(b0) {"
	"   matrix model : packoffset(c0);"
	"   matrix view : packoffset(c4);"
	"   matrix projection : packoffset(c8);"
	"};"

	"Output VSMain(Input input) {"
	"   matrix mvp = mul(mul(projection * view), model);"
	"   Output output;"
	"   output.position = mul(input.position, mvp);"
	"   output.color = float4(1.0, 0.0, 0.0, 1.0);"
	"   return output;"
	"};";

const char *fragCubeSrc =
	"struct Input {"
	"   float4 color : COLOR;"
	"};"

	"float4 PSMain(Input input) {"
	"   return input.color;"
	"};";

D3D11_INPUT_ELEMENT_DESC *gInputDescription;
ID3D11Buffer *gVBO;
ID3D11Buffer *gIBO;

void D3D11Renderer::initRenderer() {
	mCamera = nullptr;

	// Create a device, context and swap chain.
	{
		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapDesc.BufferCount = 1; // how many back buffers
		swapDesc.BufferDesc.Width = 1440; // window width
		swapDesc.BufferDesc.Height = 900; // window height
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // standard backbuffer format
		swapDesc.BufferDesc.RefreshRate.Numerator = 60; // vsync
		swapDesc.BufferDesc.RefreshRate.Denominator = 1; // vsync
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // yes we are rendering to a window
		swapDesc.OutputWindow = mWindow; // the window we are outputting to
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0; // Antialiasing
		swapDesc.Windowed = true; // we are not in fullscreen

		// request feature level 11_0
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
		UINT featureLevelCount = 1;
		D3D_FEATURE_LEVEL featureLevelSupported;

		HRESULT r = D3D11CreateDeviceAndSwapChain(
			NULL,								// adapter
			D3D_DRIVER_TYPE_HARDWARE,	// device type
			NULL,								// do we want software emulation for features not supported?
			0,									// device flags
			&featureLevel,					// feature level(s) requested
			featureLevelCount,			// amount of feature level(s) requested
			D3D11_SDK_VERSION,			// version of the DX11 SDK
			&swapDesc,						// swap chain description
			&mSwapChain,						// OUT the swap chain pointer
			&mDevice,							// OUT the d3d11 device
			&featureLevelSupported,		// OUT the d3d11 feature level that is supported on this computer.
			&mContext							// OUT the d3d11 context
		);

		assert(r == S_OK);
	}

	// Create the render target view.
	{
		HRESULT r;

		ID3D11Texture2D *backBuffer = nullptr;
		r = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer));
		assert(r == S_OK);

		r = mDevice->CreateRenderTargetView(backBuffer, NULL, &mRenderTargetView);
		// release back buffer texture as it already has been set.
		backBuffer->Release();
		assert(r == S_OK);
		mContext->OMSetRenderTargets(1, &mRenderTargetView, NULL);
	}

	// Set up the viewport.
	D3D11_VIEWPORT viewport;
	viewport.Width = 1440.0f;
	viewport.Height = 900.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 200.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	mContext->RSSetViewports(1, &viewport);

	//
	// Single cube stuffs
	//

	// Create input layout
	gInputDescription = new D3D11_INPUT_ELEMENT_DESC[1];
	gInputDescription[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};

	// Create Vertex buffer
	D3D11_BUFFER_DESC vbo;
	vbo.Usage = D3D11_USAGE_DEFAULT;
	vbo.ByteWidth = sizeof(F32) * 3;
	vbo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbo.CPUAccessFlags = 0;
	vbo.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vboData;
	vboData.pSysMem = cubeVertices;
	vboData.SysMemPitch = 0;
	vboData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&vbo, &vboData, &gVBO);

	// Create Index Buffer
	D3D11_BUFFER_DESC ibo;
	ibo.Usage = D3D11_USAGE_DEFAULT;
	ibo.ByteWidth = sizeof(U16);
	ibo.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vbo.CPUAccessFlags = 0;
	vbo.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iboData;
	iboData.pSysMem = cubeIndices;
	iboData.SysMemPitch = 0;
	iboData.SysMemSlicePitch = 0;

	mDevice->CreateBuffer(&ibo, &iboData, &gIBO);
}

void D3D11Renderer::destroyRenderer() {

}

void D3D11Renderer::beginFrame() {
	// set clear color.
	const float clearColor[4] = { 0.0f, 1.0f, 1.0f, 0.5f };
	mContext->ClearRenderTargetView(mRenderTargetView, clearColor);
}

void D3D11Renderer::renderChunks() {

}

void D3D11Renderer::endFrame() {
	
}

void D3D11Renderer::renderSingleCube() {
	if (mCamera == nullptr)
		return;
	
	glm::mat4 view = glm::lookAt(mCamera->getPosition(), mCamera->getPosition() + mCamera->getFrontVector(), mCamera->getUpVector());
	glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1440.f/900.f, 0.02f, 200.0f);

	mContext->IASetVertexBuffers(0, 1, &gVBO, nullptr, nullptr);
	mContext->IASetIndexBuffer(gIBO, DXGI_FORMAT_R16_UINT, 0);
	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mContext->DrawIndexed(36, 0, 0);
}

void D3D11Renderer::setActiveSceneCamera(Camera *camera) {
	mCamera = camera;
}

void D3D11Renderer::swapBuffers() {
	mSwapChain->Present(1, 0);
}

void D3D11Renderer::setWindowHandle(HWND window) {
	mWindow = window;
}