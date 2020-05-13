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

#ifndef INCLUDED_BEEFI_PARAM_LOGGER_IMPL_H
#define INCLUDED_BEEFI_PARAM_LOGGER_IMPL_H

#include <vector>
#include <fstream>
#include <ctime>
#include <cstdint>

#include <beefi/param_logger.h>
#include <boost/thread/mutex.hpp>

namespace gr {
  namespace beefi {

    class param_logger_impl : public param_logger
    {
     private:
        const char *dir;
        int output_power;
        int period;
        int duty_cycle;
        int channels;
        int log_period;
        boost::mutex fp_mutex;
        time_t last_log_time;
        std::vector<uint64_t> processed_items = {};
        std::fstream logfile;

        void write_header();
        void log_current_state();

     public:
        param_logger_impl(const char* dir, int log_period, int output_power, int period, int duty_cycle, int channels);
        ~param_logger_impl();

        int general_work(
                         int noutput_items,
                         gr_vector_int &ninput_items,
                         gr_vector_const_void_star &input_items,
                         gr_vector_void_star &output_items
                         );

        void update(int output_power, int period, int duty_cycle);
    };

  } // namespace beefi
} // namespace gr

#endif /* INCLUDED_BEEFI_PARAM_LOGGER_IMPL_H */

