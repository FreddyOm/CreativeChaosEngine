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

// ------ Application -------

#include "CCE/Manager/Application.h"

// -------------------------

// -------- Manager ---------

#include "CCE/ECS/Systems/PhysicsSystem.h"

// -------------------------

// ------- Graphics --------

#include "CCE/Graphics/RenderPipeline.h"

// --------------------------

// ------- Utilities --------

#include "CCE/String/String.h"
#include "CCE/Utilities/Math/CCMath.h"
#include "CCE/Utilities/Math/CRCHash.h"
#include "CCE/Utilities/Color/Color.h"
#include "CCE/Utilities/IO/IO.h"
#include "CCE/Utilities/Serialization/ISerializable.h"

// --------------------------

// --------- Editor ----------

// @TODO: Add new editor interface using gRPC

// --------------------------