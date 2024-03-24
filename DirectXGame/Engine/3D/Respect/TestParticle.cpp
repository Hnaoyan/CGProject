//#include "TestParticle.h"
//#include "MathCalc.h"
//#include "CBuffer.h"
//
//void TestParticle::Initialize(uint32_t Drawparticle)
//{
//	dxCommon_ = DirectXCommon::GetInstance();
//	srv_ = dxCommon_->GetSRV();
//	Drawparticle;
//	textureManager_ = TextureManager::GetInstance();
//	//srvHeap_ = SrvDescriptorHeap::GetInstance();
//	//directionalLight_ = DirectionalLight::GetInstance();
//	knumInstance_ = 300;
//	SettingVertex();
//	SetColor();
//	TransformMatrix();
//
//}
//
//void TestParticle::PreDraw(ID3D12GraphicsCommandList* commandList)
//{
//	assert(sCommandList_ == nullptr);
//	sCommandList_ = commandList;
//}
//
//void TestParticle::Draw(const ViewProjection& viewProjection, const Vector4& material, uint32_t index)
//{
//	Transform uvTransform = { { 1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} };
//
//	Matrix4x4 uvtransformMtrix = MatLib::MakeScaleMatrix(uvTransform.scale);
//	uvtransformMtrix = MatLib::Multiply(uvtransformMtrix, MatLib::MakeRotateZMatrix(uvTransform.rotate.z));
//	uvtransformMtrix = MatLib::Multiply(uvtransformMtrix, MatLib::MakeTranslateMatrix(uvTransform.translate));
//	Matrix4x4 bakToFront = MatLib::MakeRotateYMatrix(std::numbers::pi_v<float>);
//	Matrix4x4 billboard = MatLib::Multiply(bakToFront, MatLib::MakeInverse(viewProjection.matView));
//	billboard.m[3][0] = 0.0f;
//	billboard.m[3][1] = 0.0f;
//	billboard.m[3][2] = 0.0f;
//	instanceCount = 0;
//	for (std::list<ParticleData_Test>::iterator iterator = particles_.begin();
//		iterator != particles_.end(); ++iterator) {
//
//
//		instancingData[instanceCount].World = MatLib::MakeBillBoard((*iterator).transform.scale, billboard, (*iterator).transform.translate); //MakeAffineMatrix(particles_[i].transform.scale, particles_[i].transform.rotate, particles_[i].transform.translate);
//		//instancingData[instanceCount].World = Multiply(instancingData[instanceCount].World, MatLib::MakeAffineMatrix((*iterator).emitter.transform.scale, { 0.0f,0.0f,0.0f }, (*iterator).emitter.transform.translate));
//		instanceCount++;
//	}
//	*materialData_ = { material,false };
//	materialData_->uvTransform = uvtransformMtrix;
//
//	sCommandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
//	//形状を設定。PS0にせっていしているものとはまた別。同じものを設定すると考えておけばいい
//	sCommandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	//material
//	sCommandList_->SetGraphicsRootConstantBufferView(kMaterial, materialResource_->GetGPUVirtualAddress());
//	//worldTransform
//	sCommandList_->SetGraphicsRootDescriptorTable(kWorldTransform, srv_->GetGPUHandle(index_));
//
//	sCommandList_->SetGraphicsRootConstantBufferView(kViewProjection, viewProjection.constBuff_->GetGPUVirtualAddress());
//	//Light
//	//dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(kLight, directionalLight_->GetResource()->GetGPUVirtualAddress());
//	index;
//	//texture
//	sCommandList_->SetGraphicsRootDescriptorTable(kTexture, srv_->GetGPUHandle(index));
//	//TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(
//	//	dxCommon_->GetCommandList(), kTexture, index);
//
//
//	//描画！(DrawCall/ドローコール)・3頂点で1つのインスタンス。インスタンスについては今後
//	sCommandList_->DrawInstanced(6, DrawInstanceNum_, 0, 0);
//
//}
//
//void TestParticle::Update()
//{
//	const float kDeltTime = 1.0f / 60.0f;
//	DrawInstanceNum_ = 0;
//	instanceCount = 0;
//	for (std::list<ParticleData_Test>::iterator iterator = particles_.begin();
//		iterator != particles_.end(); ) {
//		if (DrawInstanceNum_ < kNumMaxInstance_) {
//			if ((*iterator).lifeTime <= (*iterator).currentTime) {
//				(*iterator).isAlive = false;
//				iterator = particles_.erase(iterator);
//				continue;
//			}
//			instancingData[instanceCount].World = MatLib::MakeIdentity4x4();
//			(*iterator).transform.translate = VectorLib::Add((*iterator).transform.translate, (*iterator).velocity * kDeltTime);
//			(*iterator).currentTime += kDeltTime;
//
//			instancingData[instanceCount].Color = (*iterator).color;
//			++DrawInstanceNum_;
//			++instanceCount;
//			++iterator;
//		}
//	}
//}
//
//void TestParticle::SettingVertex()
//{
//	vertexResource_ = CreateBufferResource(dxCommon_->GetDevice(), sizeof(VertexData) * 6);
//	//リソースの先頭のアドレスから使う
//	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
//	//使用するリソースのサイズは頂点3つ分のサイズ
//	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
//	//1頂点当たりのサイズ
//	vertexBufferView_.StrideInBytes = sizeof(VertexData);
//	//書き込むためのアドレスを取得
//	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
//	vertexData_[0].position = { -0.5f,-0.5f,0.0f,1.0f };
//
//	//上
//	vertexData_[1].position = { -0.5f,0.5f,0.0f,1.0f };
//
//	//右下
//	vertexData_[2].position = { 0.5f,-0.5f,0.0f,1.0f };
//
//	vertexData_[3].position = { -0.5f,0.5,0.0f,1.0f };
//	vertexData_[4].position = { 0.5f,0.5f,0.0f,1.0f };
//	vertexData_[5].position = { 0.5f,-0.5f,0.0f,1.0f };
//	vertexData_[0].texcoord = { 0.0f,1.0f };
//	vertexData_[1].texcoord = { 0.0f,0.0f };
//	vertexData_[2].texcoord = { 1.0f,1.0f };
//	vertexData_[3].texcoord = { 0.0f,0.0f };
//	vertexData_[4].texcoord = { 1.0f,0.0f };
//	vertexData_[5].texcoord = { 1.0f,1.0f };
//	for (int i = 0; i < 6; i++) {
//		vertexData_[i].normal = { 0.0f,0.0f,-1.0f };
//	}
//}
//
//void TestParticle::SetColor()
//{
//	materialResource_ = CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material_Test));
//
//	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
//}
//
//void TestParticle::TransformMatrix()
//{
//	instancingResource_ = CreateBufferResource(dxCommon_->GetDevice(), sizeof(ParticleForGPU) * kNumMaxInstance_);
//
//	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
//	CreateSRV(1);
//}
//
//void TestParticle::CreateSRV(uint32_t num)
//{
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
//	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;//2Dテクスチャ
//	srvDesc.Buffer.FirstElement = 0;
//	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
//	srvDesc.Buffer.NumElements = kNumMaxInstance_;
//	srvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
//	srv_->AddIndex();
//	srv_->SetGPUHandle(srv_->GetHeap(), srv_->GetSizeSRV(), srv_->GetIndex());//direct_->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
//	srv_->SetCPUHandle(srv_->GetHeap(), srv_->GetSizeSRV(), srv_->GetIndex());//srvHandleCPU_ = textureManager_->GettextureSrvHandleCPU(direct_->GetSrvHeap().Get(), textureManager_->GetSizeSRV(), num);
//	//srvHandleGPU_ = textureManager_->GettextureSrvHandleGPU(direct_->GetSrvHeap().Get(), textureManager_->GetSizeSRV(), num);
//	num;
//	index_ = srv_->GetIndex();
//	dxCommon_->GetDevice()->CreateShaderResourceView(instancingResource_.Get(), &srvDesc, srv_->GetCPUHandle(srv_->GetIndex()));
//
//}
