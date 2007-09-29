-- Setup view
-- This is the file where we define resolution, full screen or not etc.
function setupScreen()
  xmin = -20
  xmax = 20
  ymin = xmin
  ymax = xmax
  Engine.setViewportWidth(640)
  Engine.setViewportHeight(640)  
  Engine.setView(Rect:new(xmin, ymin, xmax, ymax))
  Engine.setFrameRate(60)  
end

setupScreen()