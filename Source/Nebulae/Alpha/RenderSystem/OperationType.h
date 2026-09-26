#ifndef NEBULAE_ALPHA_RENDERSYSTEM_OPERATIONTYPE_H_
#define NEBULAE_ALPHA_RENDERSYSTEM_OPERATIONTYPE_H_

namespace Nebulae
{

enum OperationType
{
  OT_UNKNOWN = -1,
  OT_POINTS,
  OT_LINES,
  OT_LINELIST, // Legacy name for a connected line strip.
  OT_TRIANGLES,
  OT_TRIANGLELIST, // Legacy name for a triangle strip.
  OT_TRIANGLEFAN
};

}

#endif // NEBULAE_ALPHA_RENDERSYSTEM_OPERATIONTYPE_H_