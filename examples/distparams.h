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

#ifndef DISTPARAMS_H

#define DISTPARAMS_H

#include <mogal/gafactory.h>

class DistGAFactoryParams : public mogal::GAFactoryParams
{
public:
	DistGAFactoryParams(double x, double y, double width);
	~DistGAFactoryParams();

	double getX() const								{ return m_x; }
	double getY() const								{ return m_y; }
	double getWidth() const								{ return m_width; }

	bool write(serut::SerializationInterface &si) const;
	bool read(serut::SerializationInterface &si);
private:
	double m_x, m_y, m_width;
};

#endif // DISTPARAMS_H

