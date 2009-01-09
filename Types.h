/*
 *  Types.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 5.1.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */
#pragma once

#include <set>
#include <vector>
#include <map>
#include <list>
#include <string>

#include <Core/Core.h>
#include <Geometry/Vector2.hpp>
#include <Geometry/Rect2.hpp>
#include <Geometry/Matrix2.hpp>
#include <Geometry/Segment2.hpp>
#include <Core/Core.h>

class Group;

typedef std::set<Group*>       GroupSet;

typedef std::list<std::string>      StringList;
typedef std::list<std::string>      StringList;
typedef unsigned char               ubyte;
typedef unsigned int                uint32;

typedef void (*EngineLoopCallback)(uint32 start_ticks);
