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

#include "Common/Array.h"
#include "Common/List.h"
#include "Common/Set.h"
#include "Common/Map.h"
#include "Common/Stack.h"
#include "Common/Queue.h"
#include "Common/LinkedList.h"

#include "Utility/Sort.h"
#include "Utility/Tuple.h"
#include "Utility/Random.h"
#include "Utility/StringUtils.h"

#ifdef MTK_EXPERIMENTAL
#include "Structures/BinaryTree.h"
//#include "Structures/Graph.h"
#endif
