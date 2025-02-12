#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"

// General
#include "M2_Skin.h"

// Additional
#include "M2_Part_Material.h"

CM2_Skin::CM2_Skin(const IBaseManager& BaseManager, IRenderDevice& RenderDevice, const CM2& M2Model, const SM2_SkinProfile& M2SkinProfile)
	: ModelProxie(RenderDevice.GetObjectsFactory().CreateModel())
	, m_BaseManager(BaseManager)
	, m_RenderDevice(RenderDevice)
	, m_M2Model(M2Model)
	, m_M2SkinProfile(M2SkinProfile)
{}

CM2_Skin::~CM2_Skin()
{
}

void CM2_Skin::Load(const std::shared_ptr<IFile>& File, const std::vector<SM2_Vertex>& Vertices)
{
	// Skin data
	const uint16*			t_verticesIndexes   = (const uint16*)         (File->getData() + m_M2SkinProfile.vertices.offset);
	const uint16*			t_indexesIndexes    = (const uint16*)         (File->getData() + m_M2SkinProfile.indices.offset);
	const SM2_SkinBones*	t_bonesIndexes      = (const SM2_SkinBones*)  (File->getData() + m_M2SkinProfile.bones.offset);
	const SM2_SkinSection*	t_sections          = (const SM2_SkinSection*)(File->getData() + m_M2SkinProfile.submeshes.offset);
	const uint32	        t_bonesMax          = m_M2SkinProfile.boneCountMax;
	const SM2_SkinBatch*    t_skinBatchesProtos = (const SM2_SkinBatch*)  (File->getData() + m_M2SkinProfile.batches.offset);

	_ASSERT(m_M2SkinProfile.vertices.size == m_M2SkinProfile.bones.size);

	for (uint32 sectionIndex = 0; sectionIndex < m_M2SkinProfile.submeshes.size; sectionIndex++)
	{
		const SM2_SkinSection& sectionProto = t_sections[sectionIndex];

		std::vector<SM2_Vertex> vertexes;
		vertexes.resize(sectionProto.vertexCount);
		for (uint32 i = 0; i < sectionProto.vertexCount; i++)
		{
			size_t indexIntoVertices = static_cast<size_t>(sectionProto.vertexStart) + static_cast<size_t>(i);

			_ASSERT(indexIntoVertices < m_M2SkinProfile.vertices.size);
			SM2_Vertex newVertex = Vertices[t_verticesIndexes[indexIntoVertices]];

			_ASSERT(indexIntoVertices < m_M2SkinProfile.bones.size);
			const SM2_SkinBones& newBones = t_bonesIndexes[indexIntoVertices];

			for (uint16 bone = 0; bone < sectionProto.boneInfluences; bone++)
			{
				_ASSERT(newBones.index[bone] < sectionProto.boneCount);
				newVertex.bone_indices[bone] = newBones.index[bone];
			}

			vertexes[i] = newVertex;
		}

		std::vector<uint16> indexes;
		indexes.resize(sectionProto.indexCount);
		for (uint16 i = 0; i < sectionProto.indexCount; i++)
		{
			size_t indexIntoIndexes = static_cast<size_t>(sectionProto.indexStart) + static_cast<size_t>(i);
			_ASSERT(indexIntoIndexes < m_M2SkinProfile.indices.size);
			uint16 index = t_indexesIndexes[indexIntoIndexes];

			_ASSERT(index >= sectionProto.vertexStart);
			_ASSERT(index < sectionProto.vertexStart + sectionProto.vertexCount);
			indexes[i] = index - sectionProto.vertexStart;
		}

		m_Sections.push_back(MakeShared(CM2_SkinSection, m_RenderDevice, m_M2Model, sectionIndex, sectionProto, vertexes, indexes));
	}

	// BATCHES
	
	for (uint32 i = 0; i < m_M2SkinProfile.batches.size; i++)
	{
		auto skinBatchObject = MakeShared(CM2_Skin_Batch, m_BaseManager, m_RenderDevice, m_M2Model, t_skinBatchesProtos[i]);
		m_Batches.push_back(skinBatchObject);

		m_GeometryMaterials[t_skinBatchesProtos[i].skinSectionIndex].push_back(skinBatchObject);
	}

	for (auto& it : m_GeometryMaterials)
	{
		auto& materials = it.second; // aka batch
		std::sort(materials.begin(), materials.end(), [](const std::shared_ptr<CM2_Skin_Batch>& left, const std::shared_ptr<CM2_Skin_Batch>& right) {
			return left->GetPriorityPlan() < right->GetPriorityPlan();
		});
	}

	//std::sort(m_TTT.begin(), m_TTT.end(), [](const std::pair<std::shared_ptr<CM2_SkinSection>, std::vector<std::shared_ptr<CM2_Skin_Batch>>>& left, const std::pair<std::shared_ptr<CM2_SkinSection>, std::vector<std::shared_ptr<CM2_Skin_Batch>>>& right) -> bool
	//{
	//	return left.first->getIndex() < right.first->getIndex();
	//});
}



//
// IModel
//
void CM2_Skin::Accept(IVisitor * visitor)
{
	__super::Accept(visitor);
}