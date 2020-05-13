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

#ifndef INCLUDED_BEEFI_PARAM_LOGGER_H
#define INCLUDED_BEEFI_PARAM_LOGGER_H

#include <beefi/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace beefi {

    /*!
     * \brief Loggs PWM parameters and channel throughput
     * \ingroup beefi
     *
     */
    class BEEFI_API param_logger : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<param_logger> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of beefi::param_logger.
       *
       * To avoid accidental use of raw pointers, beefi::param_logger's
       * constructor is in a private implementation
       * class. beefi::param_logger::make is the public interface for
       * creating new instances.
       */
      static sptr make(const char *log_dir, int log_period, int output_power, int period, int duty_cycle, int channels);

      virtual void update(int output_power, int period, int duty_cycle) = 0;
    };

  } // namespace beefi
} // namespace gr

#endif /* INCLUDED_BEEFI_PARAM_LOGGER_H */

