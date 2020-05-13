/* -*- c++ -*- */
/*
 * Copyright 2020 David Winter.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_BEEFI_PWM_SOURCE_H
#define INCLUDED_BEEFI_PWM_SOURCE_H

#include <beefi/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace beefi {

    /*!
     * \brief Generates a PWM signal oscillating between 0 and 1
     * \ingroup beefi
     *
     */
    class BEEFI_API pwm_source : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<pwm_source> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of beefi::pwm_source.
       *
       * To avoid accidental use of raw pointers, beefi::pwm_source's
       * constructor is in a private implementation
       * class. beefi::pwm_source::make is the public interface for
       * creating new instances.
       */
      static sptr make(int period, float duty);

      virtual void set(int period, float duty) = 0;
    };

  } // namespace beefi
} // namespace gr

#endif /* INCLUDED_BEEFI_PWM_SOURCE_H */

