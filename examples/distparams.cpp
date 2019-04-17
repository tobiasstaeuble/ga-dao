/*
    
  This file is a part of MOGAL, a Multi-Objective Genetic Algorithm
  Library.
  
  Copyright (C) 2008-2012 Jori Liesenborgs

  Contact: jori.liesenborgs@gmail.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  
  USA

*/

#include "distparams.h"

DistGAFactoryParams::DistGAFactoryParams(double x, double y, double width)
{
	m_x = x;
	m_y = y;
	m_width = width;
}

DistGAFactoryParams::~DistGAFactoryParams()
{
}

bool DistGAFactoryParams::write(serut::SerializationInterface &si) const
{
	if (!si.writeDouble(m_x))
	{
		setErrorString("Couldn't write X coordinate for test function");
		return false;
	}
	if (!si.writeDouble(m_y))
	{
		setErrorString("Couldn't write Y coordinate for test function");
		return false;
	}
	if (!si.writeDouble(m_width))
	{
		setErrorString("Couldn't write initial search width");
		return false;
	}
	return true;
}

bool DistGAFactoryParams::read(serut::SerializationInterface &si)
{
	if (!si.readDouble(&m_x))
	{
		setErrorString("Couldn't read X coordinate for test function");
		return false;
	}
	if (!si.readDouble(&m_y))
	{
		setErrorString("Couldn't read Y coordinate for test function");
		return false;
	}
	if (!si.readDouble(&m_width))
	{
		setErrorString("Couldn't read initial search width");
		return false;
	}

	return true;
}


