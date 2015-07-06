/**
 * Copyright (c) 2015 Carnegie Mellon University. All Rights Reserved.
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
 * 3. The names "Carnegie Mellon University," "SEI" and/or "Software
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
 *      INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
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

/**
 * @file GPS_Frame.cpp
 * @author James Edmondson <jedmondson@gmail.com>
 *
 * This file contains the GPS reference frame class
 **/

#include "GPS_Frame.h"

namespace gams
{
  namespace utility
  {
    const double GPS_Frame::EARTH_RADIUS = 6371000.0;
    const double GPS_Frame::MOON_RADIUS  = 1737100.0;
    const double GPS_Frame::MARS_RADIUS  = 3389500.0;

    void GPS_Frame::transform_to_origin(Location_Vector &in) const
    {
      (void)in;
      throw undefined_transform(*this, origin().frame(), true);
    }

    void GPS_Frame::transform_from_origin(Location_Vector &in) const
    {
      (void)in;
      throw undefined_transform(*this, origin().frame(), false);
    }

    double GPS_Frame::calc_distance(
          const Location_Vector &loc1, const Location_Vector &loc2) const
    {
      double alt = loc1.z();
      double alt_diff = loc2.z() - loc1.z();
      if(loc2.z() < alt)
        alt = loc2.z();

      /**
       * Calculate great circle distance using numerically stable formula from
       * http://en.wikipedia.org/w/index.php?title=Great-circle_distance&oldid=659855779
       * Second formula in "Computational formulas"
       **/
      double lat1 = DEG_TO_RAD(loc1.y());
      double lng1 = DEG_TO_RAD(loc1.x());
      double lat2 = DEG_TO_RAD(loc2.y());
      double lng2 = DEG_TO_RAD(loc2.x());

      double sin_lat1 = sin(lat1);
      double sin_lat2 = sin(lat2);
      double cos_lat1 = cos(lat1);
      double cos_lat2 = cos(lat2);

      double delta_lng = lng2 - lng1;
      if(delta_lng < 0)
        delta_lng = -delta_lng;

      double sin_delta_lng = sin(delta_lng);
      double cos_delta_lng = cos(delta_lng);

      double top_first = cos_lat2 * sin_delta_lng;
      double top_second =
          cos_lat1 * sin_lat2 - sin_lat1 * cos_lat2 * cos_delta_lng;

      double top = sqrt(top_first * top_first + top_second * top_second);

      double bottom = sin_lat1 * cos_lat2 + cos_lat1 * cos_lat2 * cos_delta_lng;

      /**
       * atan2(0, 0) is undefined, but for our purposes, we can treat it as 0
       **/
      const double epsilon = 0.000001;
      double central_angle =
          (fabs(top) < epsilon && fabs(bottom) < epsilon)
               ? 0 : atan2(top, bottom);

      double great_circle_dist = (_planet_radius + alt) * central_angle;

      if(alt_diff == 0)
        return great_circle_dist;
      else
        return sqrt(great_circle_dist * great_circle_dist + alt_diff*alt_diff);
    }

    void GPS_Frame::do_normalize(Location_Vector &in) const
    {
      while(in.y() > 90.000001)
      {
        in.y(in.y() - 180);
        in.x(in.x() + 180);
      }
      while(in.y() < -90.000001)
      {
        in.y(in.y() + 180);
        in.x(in.x() - 180);
      }
      while(in.x() > 180.000001)
        in.x(in.x() - 180);
      while(in.x() < -180.000001)
        in.x(in.x() + 180);
    }
  }
}
