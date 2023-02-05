#pragma once

// For use by applications

#include "CCE/Core.h"
#include "CCE/Memory/StackAllocator.h"
#include "CCE/Memory/PoolAllocator.h"

// ------- Analysis --------

#include "CCE/Analysis/Debug.h"
#include "CCE/Analysis/Logger.h"
#include "CCE/Analysis/Time.h"

// -------------------------

#if defined(DEBUG) || defined(DEBUG_PROFILE)

// -------- Testing ---------

#include "CCE/Analysis/UnitTesting/UnitTest.h"
#include "CCE/Analysis/UnitTesting/UnitTestMath.h"
#include "CCE/Analysis/UnitTesting/UnitTestStackAlloc.h"
#include "CCE/Analysis/UnitTesting/UnitTestPoolAlloc.h"
#include "CCE/Analysis/UnitTesting/UnitTestString.h"

// -------------------------

#endif // Only include Debug and Testing facilities when in debug or profile config (?)

// -------- Manager ---------

#include "CCE/Manager/ProfilingManager.h"
#include "CCE/Manager/RenderManager.h"
#include "CCE/Manager/PhysicsManager.h"
#include "CCE/Manager/JobManager.h"
 
// --------------------------

// ------- Utilities --------

#include "CCE/String/String.h"
#include "CCE/Utilities/CCMath.h"
#include "CCE/Utilities/CRCHash.h"

// --------------------------