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

#ifndef INCLUDED_BEEFI_PACKET_GEN_H
#define INCLUDED_BEEFI_PACKET_GEN_H

#include <beefi/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace beefi {

    /*!
     * \brief Translates incoming file index bytes to a packet stream
     * \ingroup beefi
     *
     */
    class BEEFI_API packet_gen : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<packet_gen> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of beefi::packet_gen.
       *
       * To avoid accidental use of raw pointers, beefi::packet_gen's
       * constructor is in a private implementation
       * class. beefi::packet_gen::make is the public interface for
       * creating new instances.
       */
      static sptr make(const char* directory);
    };

  } // namespace beefi
} // namespace gr

#endif /* INCLUDED_BEEFI_PACKET_GEN_H */

