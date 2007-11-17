--[[
	UnitTester module
	-------------------------
	
	created: 4/10/2007
	author: Erik Engheim
	
  Performs unit tests. Tests are peformed by UnitTester.testFiles()
  Provide a list of filename as input to UnitTester.testFiles and it
  will load and run the unit tests in each file. 
	
	For the testing to work it is required that the files contain executable lua code.
	The lua code files need to retun a table containing the functions that constitute
	the unit tests. 
	
	A unit test can easily be written like this:
	
	testunitfile.lua:
	
	-------------------------
	TestUnit = {}
	
	function TestUnit.test1()
	  -- test code for test 1
	end
	
	function TestUnit.test2()
	  -- test code for test 2	
	end
	
	return TestUnit
	-------------------------
	
--]]

local testfiles = {
  "script/UnitTest/circletest.lua",
  "script/UnitTest/spritetest.lua"
}

UnitTester = {}

function UnitTester.testFiles(files)
  for _, filename in pairs(files) do
    UnitTester.testUnit(dofile(filename))
  end
end

function UnitTester.testUnit(unit)
  for name, func in pairs(unit) do 
    print(name,":")
    func()
  end
end

UnitTester.testFiles(testfiles)