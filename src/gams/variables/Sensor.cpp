/**
 * Copyright (c) 2014 Carnegie Mellon University. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following acknowledgments and disclaimers.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. The names �Carnegie Mellon University,� "SEI� and/or �Software
 *    Engineering Institute" shall not be used to endorse or promote products
 *    derived from this software without prior written permission. For written
 *    permission, please contact permission@sei.cmu.edu.
 * 
 * 4. Products derived from this software may not be called "SEI" nor may "SEI"
 *    appear in their names without prior written permission of
 *    permission@sei.cmu.edu.
 * 
 * 5. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 * 
 *      This material is based upon work funded and supported by the Department
 *      of Defense under Contract No. FA8721-05-C-0003 with Carnegie Mellon
 *      University for the operation of the Software Engineering Institute, a
 *      federally funded research and development center. Any opinions,
 *      findings and conclusions or recommendations expressed in this material
 *      are those of the author(s) and do not necessarily reflect the views of
 *      the United States Department of Defense.
 * 
 *      NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 *      INSTITUTE MATERIAL IS FURNISHED ON AN �AS-IS� BASIS. CARNEGIE MELLON
 *      UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
 *      IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
 *      FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
 *      OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES
 *      NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
 *      TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 * 
 *      This material has been approved for public release and unlimited
 *      distribution.
 **/
#include "Sensor.h"

#include <float.h>

typedef  Madara::Knowledge_Record::Integer  Integer;


gams::variables::Sensor::Sensor ()
{
}

gams::variables::Sensor::~Sensor ()
{
}

void
gams::variables::Sensor::operator= (const Sensor & rhs)
{
  if (this != &rhs)
  {
    this->range = rhs.range;
    this->name = rhs.name;
    this->covered = rhs.covered;
  }
}


void
gams::variables::Sensor::init_vars (
  Madara::Knowledge_Engine::Knowledge_Base & knowledge,
  const std::string & sensor_name)
{
  name = sensor_name;

  // swarm commands are prefixed with "swarm.movement_command"
  std::string prefix ("sensor");
  prefix += ".";
  prefix += name;

  // initialize the variable containers
  range.set_name (prefix + ".range", knowledge);
  covered.set_name (prefix + ".covered", knowledge);
}

void
gams::variables::Sensor::init_vars (
  Madara::Knowledge_Engine::Variables & knowledge,
  const std::string & sensor_name)
{
  name = sensor_name;

  // swarm commands are prefixed with "swarm.movement_command"
  std::string prefix ("sensor");
  prefix += ".";
  prefix += name;

  // initialize the variable containers
  range.set_name (prefix + ".range", knowledge);
  covered.set_name (prefix + ".covered", knowledge);
}

double gams::variables::get_min_sensor_range (
  const gams::variables::Sensors & s)
{
  double min_range = DBL_MAX;
  for (gams::variables::Sensors::const_iterator it = s.begin();
       it != s.end(); ++it)
  {
    min_range = min_range > *(it->second.range) ? *(it->second.range) : min_range;
  }
  return min_range;
}

void gams::variables::init_vars (Sensor & variables,
  Madara::Knowledge_Engine::Knowledge_Base & knowledge,
  const std::string & sensor_name)
{
  variables.init_vars (knowledge, sensor_name);
}
