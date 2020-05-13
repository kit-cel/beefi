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

#ifndef INCLUDED_BEEFI_PACKET_GEN_IMPL_H
#define INCLUDED_BEEFI_PACKET_GEN_IMPL_H

#include <beefi/packet_gen.h>

namespace gr {
  namespace beefi {

    namespace {
      enum gen_state { IDLE, IN_PROGRESS };
    }

    class packet_gen_impl : public packet_gen
    {
     private:
      const std::string directory;
      std::array<std::vector<gr_complex>, 256> recordings = {};
      gen_state state = gen_state::IDLE;
      uint8_t i;
      size_t offset = 0;

     public:
      packet_gen_impl(const char *directory);
      ~packet_gen_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace beefi
} // namespace gr

#endif /* INCLUDED_BEEFI_PACKET_GEN_IMPL_H */

