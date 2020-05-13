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

#include <cstdio>
#include <sys/mman.h>
#include <cstdint>
#include <cstring>
#include <cerrno>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <gnuradio/io_signature.h>

#include "packet_gen_impl.h"
#include "beefi_util.h"

namespace gr {
  namespace beefi {

    packet_gen::sptr packet_gen::make(const char* dir) {
      return gnuradio::get_initial_sptr(new packet_gen_impl(dir));
    }

    namespace {
        template <typename T>
        bool read_file(const std::string &base, int i, std::vector<T> &out) {
            std::string name = base + std::to_string(i) + ".iq"; // {fmt} anyone?
            FILE *file = fopen(name.c_str(), "rb");
            if (file == nullptr)
                return false;

            if (fseek(file, 0L, SEEK_END)) {
                fclose(file);
                return false;
            }
            size_t length = ftell(file);

            if (fseek(file, 0L, SEEK_SET)) {
                fclose(file);
                return false;
            }
            
            size_t item_size = sizeof(T);
            if (length % item_size) {
                std::cerr << "Invalid file size! file: \"" << name << "\", length: " << length << ", item_size: " << item_size << ")" << std::endl;
                fclose(file);
                return false;
            }

            size_t n = length / item_size;
            int fd = fileno(file);

            // Don't ask why this is a mmap ...
            void *mregion = mmap(nullptr, length, PROT_READ, MAP_PRIVATE, fd, 0);
            if (mregion == MAP_FAILED) {
                std::cerr << "Failed to map \"" << name << "\" into memory: " << strerror(errno) << std::endl;
                fclose(file);
            }

            out.resize(n);
            memcpy(out.data(), mregion, length);

            munmap(mregion, length);
            fclose(file);
            return true;
        }
    }

    /*
     * The private constructor
     */
    packet_gen_impl::packet_gen_impl(const char *dir) : gr::block("packet_gen",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              directory(std::string(dir) + "/") {
        int i;
        
        for (i = 0; i < 256 && read_file(this->directory, i, this->recordings[i]); i++);
    }

    /*
     * Our virtual destructor.
     */
    packet_gen_impl::~packet_gen_impl() {}

    void packet_gen_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required) {
        ninput_items_required[0] = this->state == IDLE;
    }

    int packet_gen_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items) {
      const uint8_t *in = (const uint8_t *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      switch (this->state) {
      case IDLE:
          // Start new transfer
          if (ninput_items[0] < 1)
              return 0;

          this->i = in[0];
          this->consume(0, 1);

          this->offset = 0;
          this->state = gen_state::IN_PROGRESS;
          
          // Fallthrough
      case IN_PROGRESS:
          auto &data = this->recordings[this->i];
          size_t to_transfer = min<size_t>(noutput_items, data.size() - offset);

          for (size_t j = 0; j < to_transfer; j++)
              out[j] = data[this->offset+j];

          offset += to_transfer;
          
          if (offset >= data.size())
              this->state = gen_state::IDLE;

          return to_transfer;
      }
      return 0;
    }
  } /* namespace beefi */
} /* namespace gr */

