/*
 *  Exception.h
 *  LusionEngine
 *
 *  Created by Erik Engheim on 7.11.07.
 *  Copyright 2007 Translusion. All rights reserved.
 *
 */

#pragma once

class Exception
{
public:
  Exception();
  virtual ~Exception();
  
private:

};

/*!
  Thrown by methods which should not be called to signal that subclass does not implement
  method and that client should not call this method.
*/
class UnsuportedMethodException : public Exception
{

};