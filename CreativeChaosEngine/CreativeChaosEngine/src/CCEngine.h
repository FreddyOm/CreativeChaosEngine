#pragma once

// For use by applications

#include "CCE/core.h"
#include "CCE/Memory/stack-allocator.h"
#include "CCE/Memory/pool-allocator.h"

// ------- Analysis --------

#include "CCE/analysis/debug.h"
#include "CCE/analysis/logger.h"
#include "CCE/analysis/time.h"

// -------------------------

// ------ Application -------

#include "CCE/manager/application.h"

// -------------------------

// -------- Manager ---------

#include "CCE/ecs/systems/physics-system.h"
#include "CCE/input/input.h"

// -------------------------

// ------- Graphics --------

#include "CCE/graphics/rendering.h"

// --------------------------

// ------- Utilities --------

#include "CCE/string/string.h"
#include "CCE/utilities/math/CCMath.h"
#include "CCE/utilities/math/CRCHash.h"
#include "CCE/utilities/Color/Color.h"
#include "CCE/utilities/io/io.h"
#include "CCE/utilities/serialization/ISerializable.h"

// --------------------------

// --------- Editor ----------

// @TODO: Add new editor interface using gRPC

// --------------------------