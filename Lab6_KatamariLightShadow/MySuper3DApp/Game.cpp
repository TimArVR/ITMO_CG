#include "Game.h"
#include "GameComponent.h"
#include "InputDevice.h"
#include "DirectionalLight.h"

InputDevice* Game::inputDevice = nullptr;

Game::Game(LPCWSTR applicationName, int clientHeight, int clientWidth) {
	camera = Camera();
	display = new DisplayWin32(applicationName, GetModuleHandle(nullptr), clientHeight, clientWidth);
	inputDevice = new InputDevice(this);
	
	Initialize();
}

void Game::Initialize() {
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = display->GetClientWidth();
	swapDesc.BufferDesc.Height = display->GetClientWidth();
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = display->GetHwnd();
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		swapChain.GetAddressOf(),
		device.GetAddressOf(),
		nullptr,
		context.GetAddressOf()
	);

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backTex.GetAddressOf());
	res = device->CreateRenderTargetView(backTex.Get(), nullptr, rtv.GetAddressOf());
		
	CD3D11_TEXTURE2D_DESC depthTexDesc = {};
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.MiscFlags = 0;
	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.Width = display->GetClientWidth();
	depthTexDesc.Height = display->GetClientHeight();
	depthTexDesc.SampleDesc = { 1, 0 };

	res = device->CreateTexture2D(&depthTexDesc, nullptr, depthStencilTexture.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Flags = 0;
	D3D11_TEX2D_DSV tex2DDSV = {};
	tex2DDSV.MipSlice = 0;
	depthViewDesc.Texture2D = tex2DDSV;
	res = device->CreateDepthStencilView(depthStencilTexture.Get(), &depthViewDesc, depthStencilView.GetAddressOf());

	D3D11_DEPTH_STENCIL_DESC depthStateDesc = {};
	depthStateDesc.DepthEnable = true;
	depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStateDesc.StencilEnable = false;

	res = device->CreateDepthStencilState(&depthStateDesc, depthStencilState.GetAddressOf());
	context->OMSetDepthStencilState(depthStencilState.Get(), 0);

	defaultViewPort.Width = static_cast<float>(display->GetClientWidth());
	defaultViewPort.Height = static_cast<float>(display->GetClientHeight());
	defaultViewPort.TopLeftX = 0;
	defaultViewPort.TopLeftY = 0;
	defaultViewPort.MinDepth = 0;
	defaultViewPort.MaxDepth = 1.0f;

	_cameraController = new CameraController(this);

	//BeShadow set shadows details
	directionalLight = new DirectionalLight(
		this,
		8196,
		100,
		{ 1.0f ,1.0f ,1.0f, 0.25f },
		Quaternion::CreateFromAxisAngle(Vector3::Right, XM_PIDIV4)
	);

	shadowsViewPort.Width = directionalLight->shadowSize;
	shadowsViewPort.Height = directionalLight->shadowSize;
	shadowsViewPort.TopLeftX = 0;
	shadowsViewPort.TopLeftY = 0;
	shadowsViewPort.MinDepth = 0;
	shadowsViewPort.MaxDepth = 1.0f;


	// BeShadow init

	ID3DBlob* pixelShaderByteCode = nullptr;
	ID3DBlob* vertexShaderByteCode = nullptr;
	ID3DBlob* errorVertexCode = nullptr;
	ID3DBlob* errorPixelCode = nullptr;

	// BeShadow to file
	res = D3DCompileFromFile(
		L"./Shaders/ShadowShader.hlsl",
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode
	);

	res = device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr,
		shadowsVertexShader.GetAddressOf()
	);

	res = D3DCompileFromFile(
		L"./Shaders/LightShader.hlsl",
		nullptr,
		nullptr,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode
	);


	res = D3DCompileFromFile(
		L"./Shaders/LightShader.hlsl",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorPixelCode
	);

	res = device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr,
		lightVertexShader.GetAddressOf()
	);

	res = device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr,
		lightPixelShader.GetAddressOf()
	);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC {
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		D3D11_INPUT_ELEMENT_DESC {
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		}
	};

	res = device->CreateInputLayout(
		inputElements,
		3,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		layout.GetAddressOf()
	);

	context->IASetInputLayout(layout.Get());

	D3D11_BUFFER_DESC lightFirstConstBufferDesc = {};
	lightFirstConstBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	lightFirstConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightFirstConstBufferDesc.CPUAccessFlags = 0;
	lightFirstConstBufferDesc.MiscFlags = 0;
	lightFirstConstBufferDesc.StructureByteStride = 0;
	lightFirstConstBufferDesc.ByteWidth = sizeof(XMMATRIX) * 4;
	device->CreateBuffer(&lightFirstConstBufferDesc, nullptr, lightFirstConstBuffer.GetAddressOf());

	D3D11_BUFFER_DESC lightSecondConstBufferDesc = {};
	lightSecondConstBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	lightSecondConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightSecondConstBufferDesc.CPUAccessFlags = 0;
	lightSecondConstBufferDesc.MiscFlags = 0;
	lightSecondConstBufferDesc.StructureByteStride = 0;
	lightSecondConstBufferDesc.ByteWidth = sizeof(XMFLOAT4) * 3;
	device->CreateBuffer(&lightSecondConstBufferDesc, nullptr, lightSecondConstBuffer.GetAddressOf());

	//BeShadow cons buffer for shadow
	D3D11_BUFFER_DESC shadowConstBufferDesc = {};
	shadowConstBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	shadowConstBufferDesc.CPUAccessFlags = 0;
	shadowConstBufferDesc.MiscFlags = 0;
	shadowConstBufferDesc.StructureByteStride = 0;
	shadowConstBufferDesc.ByteWidth = sizeof(XMMATRIX) * 2;
	device->CreateBuffer(&shadowConstBufferDesc, nullptr, shadowsConstBuffer.GetAddressOf());

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void Game::Update() {
	for (int i = 0; i < components.size(); i++)
		components[i]->Update(deltaTime);
}

void Game::UpdateInternal() {
	auto curTime = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
	PrevTime = curTime;

	totalTime += deltaTime;
	frameCount++;

	if (inputDevice->IsKeyDown(Keys::P))
		camera.SetIsPerspective(true);
	if (inputDevice->IsKeyDown(Keys::O))
		camera.SetIsPerspective(false);

	// BeShadow gameloop render Shad DirPhong
	RenderShadows();
	RenderLight();
}

void Game::Run() {
	MSG msg = {};
	PrevTime = std::chrono::steady_clock::now();
	totalTime = 0;
	frameCount = 0;

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			_cameraController->Update(deltaTime);
			Update();
			UpdateInternal();
		}
	}
}

void Game::RenderLight() {
	context->RSSetViewports(1, &defaultViewPort);
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), depthStencilView.Get());

	//Background color
	float backgroundColor[] = { 0.2f, 0.2f, 0.2f };
	context->ClearRenderTargetView(rtv.Get(), backgroundColor);
	context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1, 0);

	context->VSSetShader(lightVertexShader.Get(), nullptr, 0);
	context->PSSetShader(lightPixelShader.Get(), nullptr, 0);

	context->VSSetConstantBuffers(0, 1, lightFirstConstBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, lightFirstConstBuffer.GetAddressOf());

	context->VSSetConstantBuffers(1, 1, lightSecondConstBuffer.GetAddressOf());
	context->PSSetConstantBuffers(1, 1, lightSecondConstBuffer.GetAddressOf());

	//to second const buff in lightshader
	Vector4 lightDirection = directionalLight->GetVectorForward();
	lightDirection.w = 0;
	lightDirection.Normalize();
	vectors[0] = lightDirection;
	vectors[1] = directionalLight->lightColorAmb;
	vectors[2] = { camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z, 0.0f };

	context->UpdateSubresource(lightSecondConstBuffer.Get(), 0, nullptr, &vectors, 0, 0);

	//BeShadow set to first const buff in lightshader
	context->PSSetShaderResources(1, 1, &directionalLight->shaderResourceView);

	for (int i = 0; i < components.size(); i++) {
		Object* _object = dynamic_cast<Object*>(components[i]);

		if (_object) {
			matrices[0] = camera.GetView() * camera.GetProjectionMatrix();
			matrices[1] = _object->GetObject();
			matrices[2] = Matrix(_object->GetObject()).Invert().Transpose();
			matrices[3] =  directionalLight->GetLightSpace();
			context->UpdateSubresource(lightFirstConstBuffer.Get(), 0, nullptr, &matrices, 0, 0);
			_object->Render();
		}
	}
	ID3D11ShaderResourceView* srvnull[] = { nullptr };
	context->PSSetShaderResources(1, 1, srvnull);

	swapChain->Present(1, 0);
}

//BeShadow render
void Game::RenderShadows() {
	context->RSSetViewports(1, &shadowsViewPort);

	context->OMSetRenderTargets(0, nullptr, directionalLight->depthStencilView);
	context->ClearDepthStencilView(directionalLight->depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);

	context->VSSetShader(shadowsVertexShader.Get(), nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

	context->VSSetConstantBuffers(0, 1, shadowsConstBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 0, nullptr);

	//BeShadow to shadowshader
	for (int i = 0; i < components.size(); i++) {
		Object* _object = dynamic_cast<Object*>(components[i]);

		if (_object) {
			matrices[0] = directionalLight->GetLightSpace();
			matrices[1] = _object->GetObject();
			context->UpdateSubresource(shadowsConstBuffer.Get(), 0, nullptr, &matrices, 0, 0);
			_object->Render();
		}
	}

	context->Flush();
}


LRESULT Game::ProccesMessage(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage) {
		case WM_KEYDOWN: {
			if (static_cast<unsigned int>(wparam) == 27)
				PostQuitMessage(0);

			return 0;
		}
		case WM_INPUT: {
			UINT dwSize = 0;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];

			if (lpb == nullptr)
				return 0;

			if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
				OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

			if (raw->header.dwType == RIM_TYPEKEYBOARD) {
				inputDevice->OnKeyDown({
					raw->data.keyboard.MakeCode,
					raw->data.keyboard.Flags,
					raw->data.keyboard.VKey,
					raw->data.keyboard.Message
					});
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE) {
				inputDevice->OnMouseMove({
					raw->data.mouse.usFlags,
					raw->data.mouse.usButtonFlags,
					static_cast<int>(raw->data.mouse.ulExtraInformation),
					static_cast<int>(raw->data.mouse.ulRawButtons),
					static_cast<short>(raw->data.mouse.usButtonData),
					raw->data.mouse.lLastX,
					raw->data.mouse.lLastY
					});
			}

			delete[] lpb;
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
		default: {
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}