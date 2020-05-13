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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "pwm_source_impl.h"
#include "beefi_util.h"

namespace gr {
  namespace beefi {

    pwm_source::sptr pwm_source::make(int period, float duty) {
      return gnuradio::get_initial_sptr(new pwm_source_impl(period, duty));
    }

    namespace {
        inline void validate(int period, float duty) {
            if (period < 1)
                throw std::runtime_error("PWM period < 1");
            
            if (!(0 <= duty && duty <= 1))
                throw std::runtime_error("PWM duty cycle not in [0,1]");
        }
    }


    /*
     * The private constructor
     */
    pwm_source_impl::pwm_source_impl(int period, float duty)
      : gr::sync_block("pwm_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(uint8_t))),
              period(period), duty(duty) {
        validate(period, duty);
    }

    void pwm_source_impl::set(int period, float duty) {
        gr::thread::scoped_lock lock(fp_mutex);

        validate(period, duty);
        this->period = period;
        this->duty = duty;
        this->offset = 0;
    }

    /*
     * Our virtual destructor.
     */
    pwm_source_impl::~pwm_source_impl() {
    }

    int pwm_source_impl::work(int noutput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items) {
      gr::thread::scoped_lock lock(fp_mutex);
      
      uint8_t *const out = reinterpret_cast<uint8_t*>(output_items[0]);
      const int delim = static_cast<int>(this->period * this->duty);
      const int todo = min<int>(period - offset, noutput_items);

      for (int i = 0; i < todo; i++)
          out[i] = offset + i < delim;

      offset += todo;

      if (offset >= period)
          offset = 0;

      // Tell runtime system how many output items we produced.
      return todo;
    }

  } /* namespace beefi */
} /* namespace gr */

