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

#ifndef INCLUDED_BEEFI_PWM_SOURCE_IMPL_H
#define INCLUDED_BEEFI_PWM_SOURCE_IMPL_H

#include <beefi/pwm_source.h>
#include <boost/thread/mutex.hpp>

namespace gr {
    namespace beefi {

        class pwm_source_impl : public pwm_source {
        private:
            int period;
            float duty;
            boost::mutex fp_mutex;
            int offset = 0;
            
        public:
            pwm_source_impl(int period, float duty);
            ~pwm_source_impl();

            int work(int noutput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);

            void set(int period, float duty);
        };
        
    } // namespace beefi
} // namespace gr

#endif /* INCLUDED_BEEFI_PWM_SOURCE_IMPL_H */

