Import('parent_env')
from scons_build.helpers import *
import os, sys

env = parent_env.Clone()

ConfigureInstallDirectory(env, 'LusionEngine')

name = 'lusionengine'

UseLibraries(env, ['qt.core', 'qt.gui', 'qt.script', 'qt.opengl', 'qt.uitools', 'qt.xml', 'opengl', 'lua', 'glew'])
UseTool(env, 'qt4')

#CONFIG += uitools

HandleUi4Files(env, ['Forms/MainForm.ui'])
#FORMS += Forms/MainForm.ui

includes = """
    Geometry
    Core
    Base
    Utils
    Lua/Base
    Lua
    Lua/Geometry""".split()

env.AppendUnique(CPPPATH = includes)

env.AppendUnique(CPPDEFINES = ['M_PI=3.14159265358979', 'NOMINMAX=1'])

# Input
HEADERS = """
    Engine.h
    Timing.h
    Types.h
    Base/Action.h
    Base/CircleShape.h
    Base/Group.h
    Base/MotionState.h
    Base/PointsView.h
    Base/PolygonView.h
    Base/RectShape2.h
    Base/SegmentShape2.h
    Base/SegmentView.h
    Base/Shape.h
    Base/ShapeGroup.h
    Base/ShapeIterator.h
    Base/ShapeListener.h
    Base/Sprite.h
    Base/View.h
    Core/AutoreleasePool.hpp
    Core/Core.h
    Core/SharedObject.hpp
    Geometry/Circle.hpp
    Geometry/IO.hpp
    Geometry/Line2.hpp
    Geometry/Matrix2.hpp
    Geometry/Polygon2.hpp
    Geometry/QuadNode.h
    Geometry/Ray2.hpp
    Geometry/Rect2.hpp
    Geometry/Segment2.hpp
    Geometry/Vector2.hpp
    Lua/LuaEngine.h
    Lua/LuaUtils.h
    Utils/Algorithms.h
    Utils/Exception.h
    Utils/GLUtils.h
    Utils/Iterator.h
    Utils/PolygonUtils.h
    Utils/RoadMap.h
    Lua/Base/LuaShape.h
    Lua/Base/LuaSprite.h
    Lua/Base/LuaView.h
    Lua/Geometry/LuaCGALGeometry.h
    Lua/Geometry/LuaCircle.h
    Lua/Geometry/LuaGeometry.h
    Lua/Geometry/LuaMatrix2.h
    Lua/Geometry/LuaMotionState.h
    Lua/Geometry/LuaRay2.h
    Lua/Geometry/LuaRect2.h
    Lua/Geometry/LuaSegment2.h
    Lua/Geometry/LuaVector2.h
    Gui/RenderWidget.h
    Gui/MainForm.h""".split()

SOURCES = """
    Engine.cpp
    main.cpp
    Timing.cpp
    Base/Action.cpp
    Base/CircleShape.cpp
    Base/Group.cpp
    Base/MotionState.cpp
    Base/PointsView.cpp
    Base/PolygonView.cpp
    Base/RectShape2.cpp
    Base/SegmentShape2.cpp
    Base/SegmentView.cpp
    Base/Shape.cpp
    Base/ShapeGroup.cpp
    Base/ShapeListener.cpp
    Base/Sprite.cpp
    Base/View.cpp
    Core/AutoreleasePool.cpp
    Core/SharedObject.cpp
    Geometry/Circle.cpp
    Geometry/IO.cpp
    Geometry/Line2.cpp
    Geometry/Matrix2.cpp
    Geometry/Polygon2.cpp
    Geometry/Ray2.cpp
    Geometry/Rect2.cpp
    Geometry/Segment2.cpp
    Geometry/Vector2.cpp
    Lua/LuaEngine.cpp
    Lua/LuaUtils.cpp
    Utils/Algorithms.cpp
    Utils/Exception.cpp
    Utils/GLUtils.cpp
    Utils/Iterator.cpp
    Utils/PolygonUtils.cpp
    Utils/RoadMap.cpp
    Lua/Base/LuaShape.cpp
    Lua/Base/LuaSprite.cpp
    Lua/Base/LuaView.cpp
    Lua/Geometry/LuaCircle.cpp
    Lua/Geometry/LuaGeometry.cpp
    Lua/Geometry/LuaMatrix2.cpp
    Lua/Geometry/LuaMotionState.cpp
    Lua/Geometry/LuaRay2.cpp
    Lua/Geometry/LuaRect2.cpp
    Lua/Geometry/LuaSegment2.cpp
    Lua/Geometry/LuaVector2.cpp
    Gui/RenderWidget.cpp
    Gui/MainForm.cpp""".split()

build_target = ProgramTarget(env, name = name, sources = SOURCES, console = True)
install_target = InstallTarget(env, build_target, env['INSTALL_BIN'])

InstallTree(env, 'script', os.path.join(env['INSTALL_BIN'], 'script'))
InstallTree(env, 'Forms', os.path.join(env['INSTALL_BIN'], 'Forms'))
