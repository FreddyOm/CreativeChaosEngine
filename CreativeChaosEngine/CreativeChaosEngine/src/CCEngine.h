#pragma once

// For use by applications

#include "CCE/Core.h"

#if defined(DEBUG) || defined(DEBUG_PROFILE)

// ------- Analysis --------

#include "CCE/Analysis/Debug.h"
#include "CCE/Analysis/Logger.h"
#include "CCE/Analysis/Time.h"

// -------------------------

// -------- Testing ---------

#include "CCE/Analysis/UnitTesting/UnitTest.h"
#include "CCE/Analysis/UnitTesting/UnitTestMath.h"

// -------------------------

#endif // Only include Debug and Testing facilities when in debug or profile config

// -------- Manager ---------

#include "CCE/Manager/ProfilingManager.h"
#include "CCE/Manager/RenderManager.h"
#include "CCE/Manager/PhysicsManager.h"
#include "CCE/Manager/JobManager.h"
 
// --------------------------

