#ifndef NEBULAE_ALPHA_OPERATIONTYPE_H__
#define NEBULAE_ALPHA_OPERATIONTYPE_H__

namespace Nebulae {

enum OperationType
{
  OT_UNKNOWN = -1,
  OT_POINTS,
  OT_LINES,
  OT_LINELIST,
  OT_TRIANGLES,
  OT_TRIANGLELIST,
  OT_TRIANGLEFAN
};

}

#endif // NEBULAE_ALPHA_OPERATIONTYPE_H__