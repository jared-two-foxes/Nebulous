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