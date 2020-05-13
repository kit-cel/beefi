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

#include <string>
#include <gnuradio/io_signature.h>
#include "param_logger_impl.h"

namespace gr {
  namespace beefi {

      param_logger::sptr param_logger::make(const char* dir, int log_period, int output_power, int period, int duty_cycle, int channels) {
          return gnuradio::get_initial_sptr(new param_logger_impl(dir, log_period, output_power, period, duty_cycle, channels));
      }

      /*
       * The private constructor
       */
      param_logger_impl::param_logger_impl(const char* dir, int log_period, int output_power, int period, int duty_cycle, int channels)
          : gr::block("param_logger", gr::io_signature::make(channels, channels, sizeof(gr_complex)),
                      gr::io_signature::make(0, 0, 0)), dir(dir), log_period(log_period), output_power(output_power), period(period),
            duty_cycle(duty_cycle), channels(channels) {
          
          this->processed_items.resize(channels);
          this->last_log_time = std::time(nullptr);
          
          write_header();

          std::string logfile_name = dir;
          logfile_name = logfile_name + "/beefi_log_" + std::to_string(static_cast<uint64_t>(std::time(nullptr))) + ".csv";
          this->logfile.open(logfile_name, std::ios::out);
      }
      
      /*
       * Our virtual destructor.
       */
      param_logger_impl::~param_logger_impl() {
          this->log_current_state();
      }
      
      void param_logger_impl::update(int output_power, int period, int duty_cycle) {
          gr::thread::scoped_lock lock(this->fp_mutex);
          this->output_power = output_power;
          this->period = period;
          this->duty_cycle = duty_cycle;
      }

      /**
       * Only to be invoked when this->fp_mutex has already been locked!
       */
      void param_logger_impl::log_current_state() {
          uint64_t since_last_log = static_cast<uint64_t>(std::time(nullptr) - this->last_log_time);
          this->last_log_time = std::time(nullptr);

          std::string human_time = std::ctime(&this->last_log_time);
          human_time.resize(human_time.size() - 1);

          this->logfile
              << this->last_log_time << ";"
              << human_time << ";"
              << this->output_power << ";"
              << this->period << ";"
              << this->duty_cycle << ";";

          for (int i = 0; i < this->channels; i++) {
              this->logfile << static_cast<uint64_t>(this->processed_items[i] / since_last_log) << ";";
              this->processed_items[i] = 0;
          }

          this->logfile << std::endl;
          this->logfile.flush();
      }

      int param_logger_impl::general_work(int noutput_items, gr_vector_int &ninput_items,
                                          gr_vector_const_void_star &input_items, gr_vector_void_star &output_items) {
          
          gr::thread::scoped_lock lock(this->fp_mutex);
          // Consume all available input items
          for (int i = 0; i < this->channels; i++) {
              processed_items[i] += ninput_items[i];
              consume(i, ninput_items[i]);
          }

          if (std::time(nullptr) - last_log_time > log_period)
              this->log_current_state();
          
          return 0;
      }
      
      
      void param_logger_impl::write_header() {
          std::string header_file_name = dir;
          header_file_name += "/header.csv";
          
          std::fstream header(header_file_name, std::ios::out);
          header << "Time (unix epoch);Time (human readable);Output Power (dBm, per Channel);Period (Packets);Duty cycle (%);";
          
          for (int i = 0; i < channels; i++)
              header << "Channel " << i << ";";
          
          header << std::endl;
          header.close();
      }
      
  } /* namespace beefi */
} /* namespace gr */

