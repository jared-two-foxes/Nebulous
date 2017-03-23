/** \file AlignmentFlags.h \brief Contains the Alignment flag type and the
    global alignment flag constants. **/

#ifndef __NEBULAE_ALIGNMENTFLAGS_H__
#define __NEBULAE_ALIGNMENTFLAGS_H__

#include <Nebulae/Common/Common.h>

namespace Nebulae {
  
/** Alignment flags. */
NE_FLAG_TYPE(Alignment);
extern const Alignment ALIGN_NONE;     ///< No alignment selected.
extern const Alignment ALIGN_VCENTER;  ///< Vertically-centered.
extern const Alignment ALIGN_TOP;      ///< Aligned to top.
extern const Alignment ALIGN_BOTTOM;   ///< Aligned to bottom.
extern const Alignment ALIGN_CENTER;   ///< Horizontally-centered.
extern const Alignment ALIGN_LEFT;     ///< Aligned to left.
extern const Alignment ALIGN_RIGHT;    ///< Aligned to right.

}

#endif // __NEBULAE_ALIGNMENTFLAGS_H__