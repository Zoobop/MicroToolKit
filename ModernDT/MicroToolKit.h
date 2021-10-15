#pragma once

// IMPORTANT: To enable logging and profiling, use the respective preprocessors:
// #define ENABLE_LOGGING
// #define ENABLE_PROFILING

#include "Core/Core.h"
#include "Core/Function.h"
#include "Core/Memory.h"
#include "Core/Typedef.h"

#include "DebugUtils/Timer.h"
#include "DebugUtils/Log.h"

#include "File/FileHandler.h"

#include "Structures/Array.h"
#include "Structures/List.h"
#include "Structures/Set.h"
#include "Structures/Map.h"
#include "Structures/Stack.h"
#include "Structures/Queue.h"
#include "Structures/LinkedList.h"

#include "Utility/Sort.h"
#include "Utility/Tuple.h"
#include "Utility/Random.h"
#include "Utility/StringUtils.h"

#ifdef MDT_EXPERIMENTAL
#include "Structures/BinaryTree.h"
//#include "Structures/Graph.h"
#endif
