/* -*- c++ -*- */

#define BEEFI_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "beefi_swig_doc.i"

%{
#include "beefi/packet_gen.h"
#include "beefi/pwm_source.h"
#include "beefi/param_logger.h"
%}

%include "beefi/packet_gen.h"
GR_SWIG_BLOCK_MAGIC2(beefi, packet_gen);

%include "beefi/pwm_source.h"
GR_SWIG_BLOCK_MAGIC2(beefi, pwm_source);
%include "beefi/param_logger.h"
GR_SWIG_BLOCK_MAGIC2(beefi, param_logger);
