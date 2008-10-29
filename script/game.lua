--[[
  This script is always called by game engine after
  setup scripts have been called. Typically this
  will contain the code for your main game. 
  
  We chosen to call the main game script from here
  rather than put it in so we can easily switch
  between running game and unittests.
]]--
-- dofile("script/unittester.lua")
dofile("script/maingame.lua")