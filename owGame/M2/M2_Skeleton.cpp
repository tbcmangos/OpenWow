#include "stdafx.h"

// Include
#include "M2.h"
#include "M2_Base_Instance.h"

// General
#include "M2_Skeleton.h"

CM2SkeletonBone3D::CM2SkeletonBone3D(const SM2_Part_Bone_Wrapper& M2Bone)
	: m_M2Bone(M2Bone)
	, m_PivotPoint(glm::vec3(0.0f))
	, m_Matrix(glm::mat4(1.0f))
	, m_RotateMatrix(glm::mat4(1.0f))
	, m_IsCalculated(false)
{
}

CM2SkeletonBone3D::~CM2SkeletonBone3D()
{
}

const std::weak_ptr<ISkeletonBone3D>& CM2SkeletonBone3D::GetParentBone() const
{
	return m_ParentBone;
}

const std::vector<std::shared_ptr<ISkeletonBone3D>>& CM2SkeletonBone3D::GetChilds() const
{
	return m_Childs;
}

glm::vec3 CM2SkeletonBone3D::GetPivotPoint() const
{
	return m_M2Bone.getPivot();
}

const glm::mat4& CM2SkeletonBone3D::GetMatrix() const
{
	return m_Matrix;
}

const glm::mat4& CM2SkeletonBone3D::GetRotateMatrix() const
{
	return m_RotateMatrix;
}

//
// Internal
//
void CM2SkeletonBone3D::SetParentAndChildsInternals(const std::vector<std::shared_ptr<CM2SkeletonBone3D>>& Bones)
{
	if (m_M2Bone.getParentBoneID() != -1)
	{
		_ASSERT(m_M2Bone.getParentBoneID() < Bones.size());
		auto parentBone = Bones[m_M2Bone.getParentBoneID()];
		_ASSERT(std::find(parentBone->m_Childs.begin(), parentBone->m_Childs.end(), shared_from_this()) == parentBone->m_Childs.end());
		parentBone->m_Childs.push_back(shared_from_this());
		m_ParentBone = parentBone;
	}
}

void CM2SkeletonBone3D::Calculate(const CM2_Base_Instance * M2Instance, uint32 GlobalTime)
{
	if (m_IsCalculated)
		return;

	auto parentBone = m_ParentBone.lock();
	if (parentBone)
		std::dynamic_pointer_cast<CM2SkeletonBone3D>(parentBone)->Calculate(M2Instance, GlobalTime);

	glm::mat4 matrix = m_M2Bone.calcMatrix(M2Instance, GlobalTime);

	if (parentBone)
		matrix = parentBone->GetMatrix() * matrix;

	m_Matrix = matrix;
	m_IsCalculated = true;
}

void CM2SkeletonBone3D::Reset()
{
	m_IsCalculated = false;
}



//
// CM2SkeletonComponent3D
//
CM2SkeletonComponent3D::CM2SkeletonComponent3D(const ISceneNode3D& OwnerNode, const CM2& M2Model)
	: CComponentBase(OwnerNode)
{
	for (const auto& m2Bone : M2Model.getSkeleton().GetBones())
		m_Bones.push_back(std::make_shared<CM2SkeletonBone3D>(m2Bone));

	for (const auto& bone : m_Bones)
		bone->SetParentAndChildsInternals(m_Bones);
}

CM2SkeletonComponent3D::~CM2SkeletonComponent3D()
{
}

std::vector<glm::mat4> CM2SkeletonComponent3D::CreatePose(size_t BoneStartIndex, size_t BonesCount) const
{
	std::vector<glm::mat4> result;
	result.reserve(BonesCount);
	for (size_t i = BoneStartIndex; i < BoneStartIndex + BonesCount; i++)
		result.push_back(m_Bones[GetM2OwnerNode().getM2().getSkeleton().getBoneLookupIndex(i)]->GetMatrix());
	return result;
}

//
// ISkeletonComponent3D
//
std::shared_ptr<ISkeletonBone3D> CM2SkeletonComponent3D::GetBone(size_t Index) const
{
	_ASSERT(Index < m_Bones.size());
	return m_Bones.at(Index);
}

void CM2SkeletonComponent3D::Calculate(uint32 GlobalTime)
{
	for (const auto& b : m_Bones)
		b->Reset();

	for (const auto& b : m_Bones)
		b->Calculate(&GetM2OwnerNode(), GlobalTime);
}

//
// Protected
//
const CM2_Base_Instance& CM2SkeletonComponent3D::GetM2OwnerNode() const
{
	return reinterpret_cast<const CM2_Base_Instance&>(GetOwnerNode());
}