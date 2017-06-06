
#include "Mesh.h"

#include <Nebulae/Beta/Mesh/SubMesh.h>

using namespace Nebulae;

  
Mesh::Mesh( const std::string& name )
  : m_name(name)
{
}


Mesh::~Mesh()
{
  for( int i = 0; i < MAX_LOD_COUNT; ++i ) {
    for( std::size_t j = 0, n = m_lods[i].m_SubMeshes.size(); j<n; ++j ) {
      delete m_lods[i].m_SubMeshes[j];
    }
  }
}


bool 
Mesh::LoadImpl_( File* is )
{
  // Create stream to read in from
  if( is ) {
    // Return success of import procedure
  	//Relic::MeshSerializer serializer;
  	//return serializer.ImportMesh(is, this);
  }

  return false;
}


SubMesh* 
Mesh::CreateSubMesh( uint16 lod )
{
  SubMesh* sm = new SubMesh(this);
  m_lods[lod].m_SubMeshes.push_back(sm);
  return sm;
}


std::size_t 
Mesh::GetSubMeshCount( uint16 lod ) const
{
  return m_lods[lod].m_SubMeshes.size();
}


std::vector<SubMesh*>& 
Mesh::GetSubMeshes( uint16 lod )
{
  return m_lods[lod].m_SubMeshes;
}


SubMesh* 
Mesh::GetSubMesh( uint16 lod, uint16 index ) const
{
  return m_lods[lod].m_SubMeshes[index];
}


/* void 
Mesh::AddFaceSet(unsigned short lod, Material* pMaterial, FaceSet* pFaceSet)
{
  for (int i = 0, n = m_lods[nLod].m_materialFaceSets.size(); i<n; ++i)
  {
    if (m_lods[nLod].m_materialFaceSets[i]->getMaterial() == pMaterial)
    {
      m_lods[nLod].m_materialFaceSets[i]->addFaceSet(pFaceSet);
      return;
    }
  }

  MaterialFaceSet* pMaterialFaceSet = new MaterialFaceSet(m_pd3dDevice, pMaterial);
  pMaterialFaceSet->addFaceSet(pFaceSet);

  m_lods[nLod].m_materialFaceSets.push_back(pMaterialFaceSet);
}
    
//TODO: could possibly 'empty' scratchArray before filling?
void 
FillMeshPoseBuffer(FaceSet* fs, const hkaPose& pose, btAlignedObjectArray<hkMatrix4>& scratchArray)
{
  const hkaSkeleton* skeleton = pose.getSkeleton();

  hkInt32 i;
  hkReal fMatrix[16];
  hkQsTransform transform;
  for (i = 0; i<fs->boneOrder.getSize();i++)
  {
    transform.setIdentity();

    // Store the required bone name length
    hkUint32 nRequiredBoneLen = fs->boneOrder[i].getLength();
    // Store the required bone name.
    char* szRequiredBone = new char[nRequiredBoneLen+1];
    memcpy(szRequiredBone, fs->boneOrder[i].cString(), nRequiredBoneLen);
    szRequiredBone[nRequiredBoneLen] = '\0';

    // Flag if skeleton bone was
    hkBool bFound = false;
    // Cycle skeleton bone names
    for (hkUint32 j = 0, n = skeleton->m_bones.getSize(); j<n; ++j)
    {
      // Store string array pointer
      const char* szSkeletonName = skeleton->m_bones[j].m_name.cString();
      // Store string length
      hkUint32 nSkeletonNameLen = skeleton->m_bones[j].m_name.getLength();

      // Create a scratch buffer for skeletonName
      char* szScratchName = new char[nSkeletonNameLen+1];
      // Convert the skeleton bone name to lower case
      std::transform(&szSkeletonName[0], &szSkeletonName[nSkeletonNameLen], szScratchName, (int(*)(int)) std::tolower);
      // force null termination
      szScratchName[nSkeletonNameLen] = '\0';

      // Attempt to match the name of jth bone with ith bone from fs.
      if (strncmp(szScratchName, szRequiredBone, nRequiredBoneLen) == 0)
      {
        // Grab the transforms for the jth bone
        transform = pose.getBoneModelSpace(j);
        // Stop cycle as we have found the boneTransform
        bFound = true;
      }
      
      // Delete create name buffer to prevent leak.
      delete [] szScratchName;

      // Break from inner loop as bone was found
      if (bFound) break;
    }

    // Convert the float array into a hkMatrix4
    transform.get4x4ColumnMajor(fMatrix);
    scratchArray[i].set4x4ColumnMajor(fMatrix);
    
    // Re-establish found as false.
    bFound = false;
  }
}

//TODO: note the useless redundancy that is occuring here.  There is really little reason to update ALL
//		of the lod's.  This could be streamlined by only updating the lod that is going to be rendered.
void
Mesh::setReferencePose(const hkaPose& pose)
{
  // store the reference pose.
  //m_referencePose = &pose;

  //Note! There is an issue with power_armour_common reference pose, at the very least, whereby the reference pose is
  //		facing  in the wrong direction.  Rotating reference pose, this may cause issues with other models.

  // Create rotation to rotate by 180 degrees
  hkRotation rotation;
  rotation.setAxisAngle(hkVector4(0,1,0), HK_REAL_PI);

  // Create transform from rotation
  hkTransform referenceTransform(rotation, hkVector4(0,0,0,0));
  // Create transform for inv rotation
  rotation.invert(0.1f);
  hkTransform referenceTransformInv(rotation, hkVector4(0,0,0,0));

  // Create array to hold mesh bone influence transforms
  btAlignedObjectArray<hkMatrix4> referencePoseArray(MAX_BONE_COUNT);
  // Cycle through level-of-details
  for (int i = 0; i < MAX_LOD_COUNT; ++i)
  {
    // Store the MaterialFaceSet for convenience.
    std::vector<MaterialFaceSet*>& materialFaceSet = m_lods[i].m_materialFaceSets;

    // Cycle through the MaterialFaceSet.
    for (int j = 0, n = materialFaceSet.size(); j<n; ++j)
    {
      // Cycle through the FaceSet's of the MaterialFaceSet
      for (int k = 0, m = materialFaceSet[j]->getFaceSetCount(); k<m; ++k)
      {
        // Grab the kth FaceSet
        FaceSet* fs = materialFaceSet[j]->getFaceSet(k);

        // Fill referencePoseArray with data from referencePose
        FillMeshPoseBuffer(fs, pose, referencePoseArray);

        // Rotate all the transforms in the reference pose.
        hkReal fMatrix[16];
        hkTransform original, tmp;
        for (hkUint32 i = 0; i < referencePoseArray.getSize(); i++)
        {
          referencePoseArray[i].get4x4ColumnMajor(fMatrix);
          hkTransform original; original.set4x4ColumnMajor(fMatrix);

          tmp.setMul(referenceTransform, original);
          tmp.setMulEq(referenceTransformInv);

          tmp.get4x4ColumnMajor(fMatrix);
          referencePoseArray[i].set4x4ColumnMajor(fMatrix);
          referencePoseArray[i].invert(0.01f);
        }

        // Grab the immediate context
        ID3D11DeviceContext* pImmediateContext;
        m_pd3dDevice->GetImmediateContext(&pImmediateContext);
        // Grab the ReferencePose buffer from fs.
        ID3D11Buffer* poseBuffer = fs->getConstantBufferByLocation(2);
        // Update the animationPose buffer.
        pImmediateContext->UpdateSubresource(poseBuffer, 0, NULL, referencePoseArray.begin(), 0, 0);
      }
    }
  }
}

//TODO: note the useless redundancy that is occuring here.  There is really little reason to update ALL
//		of the lod's.  This could be streamlined by only updating the lod that is going to be rendered.
void 
Mesh::setAnimationPose(const hkaPose& pose)
{
  // store the animation pose.
  //m_animationPose = &pose;

  // Create array to hold mesh bone influence transforms
  btAlignedObjectArray<hkMatrix4> animationPoseArray(MAX_BONE_COUNT);
  // Cycle through level-of-details
  for (int i = 0; i < MAX_LOD_COUNT; ++i)
  {
    // Store the MaterialFaceSet for convenience.
    std::vector<MaterialFaceSet*>& materialFaceSet = m_lods[i].m_materialFaceSets;

    // Cycle through the MaterialFaceSet.
    for (int j = 0, n = materialFaceSet.size(); j<n; ++j)
    {
      // Cycle through the FaceSet's of the MaterialFaceSet
      for (int k = 0, m = materialFaceSet[j]->getFaceSetCount(); k<m; ++k)
      {
        // Grab the kth FaceSet
        FaceSet* fs = materialFaceSet[j]->getFaceSet(k);

        // Fill referencePoseArray with data from referencePose
        FillMeshPoseBuffer(fs, pose, animationPoseArray);

        // Grab the immediate context
        ID3D11DeviceContext* pImmediateContext;
        m_pd3dDevice->GetImmediateContext(&pImmediateContext);
        // Grab the ReferencePose buffer from fs.
        ID3D11Buffer* poseBuffer = fs->getConstantBufferByLocation(3);
        // Update the animationPose buffer.
        pImmediateContext->UpdateSubresource(poseBuffer, 0, NULL, animationPoseArray.begin(), 0, 0);
      }
    }
  }
}

void 
Mesh::render(ID3D11DeviceContext* pContext, hkInt32 nLod, hkUint32 nBufferCount, ID3D11Buffer** ppConstantBuffers) const
{
  for (int i = 0, n = m_lods[nLod].m_materialFaceSets.size(); i < n; ++i)
  {
    m_lods[nLod].m_materialFaceSets[i]->render(pContext, nBufferCount, ppConstantBuffers);
  }
}*/