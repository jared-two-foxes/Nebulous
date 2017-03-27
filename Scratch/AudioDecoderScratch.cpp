
DataHandle VoxUtils::LoadDataSourceFromFile( const c8* fileName, s32 decoderType, u32 groupId )
{
  VoxEngine& vox = VoxEngine::GetVoxEngine();
  return vox.LoadDataSource( k_nStreamTypeCFile, (void*)fileName, decoderType, 0, groupId );
}



/// ....
if(fileName)
{
  const c8* ext = strrchr(fileName, '.');
  if(ext == 0)
    return DataHandle();
  
  ext++;//skip '.'
  VOX_STRING fileext(ext);

  for(u32 i = 0; i < strlen(ext); i++)
  {
    fileext[i] = fileext[i] < 97 ? fileext[i] + 32 : fileext[i];
  }
  
  if(fileext == "wav")
  {
    return LoadDataSourceFromFile(fileName, k_nDecoderTypeMSWav, groupId);
  }
  else if(fileext == "ogg")
  {
    return LoadDataSourceFromFile(fileName, k_nDecoderTypeStbVorbis, groupId);
  }
  else if(fileext == "mpc")
  {
    return LoadDataSourceFromFile(fileName, k_nDecoderTypeMPC8, groupId);
  }
  else if(fileext == "vxn")
  {
    return LoadDataSourceFromFile(fileName, k_nDecoderTypeInteractiveMusic, groupId);
  }
}




// ---------------
// Usage Scenario. 
//
// This gets called from within the AudioBackend::CreateSound function thus instead of the 
// AudioSoundInterface child implementations being in charge of the loading of files instead they
// are supplied the data by the AudioBackend.
//            

//@todo pick which decoder to use.
AudioDecoder *decoder;
//@todo call some form of a parse file function
decoder->ParseFile(); ///???
//@todo return the trackParam data and send it to the AudioSoundInterface created.
TrackParams params = decoder->GetTrackParams();



//
// VOX equivalent to our AudioSoundInterface.
//
class DataObj : public Handlable
{
public:
  virtual ~DataObj(){}
protected:

  // Not used any more (used to be used for blank data sources, replaced by blank data handle)
  DataObj(HandleId id, StreamInterface* pStream, DecoderInterface* pDecoder, s32 groupId = VOX_GROUP_MASTER_ID, s32 priorityBank = 0) :     
    Handlable(id),
    m_groupId(groupId),
    m_priorityBank(priorityBank),
    m_uid(-1),
    m_pStream(pStream),
    m_pDecoder(pDecoder),
    m_isUpdating(false),
    m_needToDie(false),
    m_state(DataSource::STATE_INITIAL)
  {}

  // Used for async source load
  DataObj(HandleId id, StreamInterface* pStream, DecoderInterface* pDecoder, s32 groupId = VOX_GROUP_MASTER_ID, VoxSourceLoadingFlags loadingFlags = vox::k_nNone, s32 priorityBank = 0) :    
    Handlable(id),
    m_groupId(groupId),
    m_priorityBank(priorityBank),
    m_uid(-1),
    m_pStream(pStream),
    m_pDecoder(pDecoder),
    m_isUpdating(false),
    m_needToDie(false),
    m_state(DataSource::STATE_CONVERTING),
    m_loadingFlags(loadingFlags)
  {}

  // Used for sync source load
  DataObj(HandleId id, StreamInterface* pStream, DecoderInterface* pDecoder, const TrackParams &trackParams, s32 groupId = VOX_GROUP_MASTER_ID, s32 priorityBank = 0) :     
    Handlable(id),
    m_groupId(groupId),
    m_priorityBank(priorityBank),
    m_uid(-1),
    m_trackParams(trackParams),
    m_pStream(pStream),
    m_pDecoder(pDecoder), 
    m_isUpdating(false),
    m_needToDie(false),
    m_state(DataSource::STATE_READY)
  {}
protected:
  void Update();
  
protected:
  // THESE FUNCTIONS CAN RETURN 0 AT ANY MOMENT!! (NeedToDie sets m_state to dying and can run in user thread)
  StreamInterface*  GetStream() { return m_state == DataSource::STATE_READY ? m_pStream : 0;}
  DecoderInterface* GetDecoder() { return m_state == DataSource::STATE_READY ? m_pDecoder : 0;}

  StreamInterface*  GetStreamEx() { return m_pStream;}
  DecoderInterface* GetDecoderEx() { return m_pDecoder;}

  void RegisterEmitter(HandleId hid);
  void UnregisterEmitter(HandleId hid);
  EmitterInternalList* GetRegisteredEmitters(){return &m_currentEmitters;}

  u32 GetGroup();
  bool IsChild(u32 groupId);

  s32 GetPriorityBank(){return m_priorityBank;}
  void SetPriorityBank(s32 priorityBank){m_priorityBank = priorityBank;}

  void SetUid(s32 Uid){m_uid = Uid;}
  s32 GetUid(){return m_uid;}

  void NeedToDie();//mutex
  bool ShouldDie();
  
  void SetUpdating(bool needUpdate = true){m_isUpdating = needUpdate;}
  bool IsUpdating(){return m_isUpdating;}

  bool IsReady();
  f32 GetDuration();

  void SetUserData(DataHandleUserData &data);
  DataHandleUserData GetUserData();

  void PrintDebug();
  void GetDebugInfo(DebugChunk_dataSource &info);

  TrackParams GetTrackParams() const {return m_trackParams;}

protected:
  s32 m_groupId;
  s32 m_priorityBank;
  s32 m_uid;

  TrackParams m_trackParams;
  StreamInterface*  m_pStream;
  DecoderInterface* m_pDecoder;
  EmitterInternalList m_currentEmitters;

  DataHandleUserData m_userData;

  bool m_isUpdating;
  bool m_needToDie;
  s32 m_state;
  VoxSourceLoadingFlags m_loadingFlags;

  VOX_MUTEX_LEVEL_1(Mutex m_stateMutex;)

  friend class VoxEngine;
  friend class VoxEngineInternal;
  friend class DataHandle;
  friend class EmitterObj;
};