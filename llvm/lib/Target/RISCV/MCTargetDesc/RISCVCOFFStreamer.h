//===-- RISCVTargetWinCOFFStreamer.h - RISC-V Target WinCOFF Streamer ----*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RISCV_MCTARGETDESC_RISCVTARGETWINCOFFSTREAMER_H
#define LLVM_LIB_TARGET_RISCV_MCTARGETDESC_RISCVTARGETWINCOFFSTREAMER_H

#include "RISCVTargetStreamer.h"
#include "llvm/MC/MCWinCOFFStreamer.h"

namespace llvm {

class RISCVTargetWinCOFFStreamer : public RISCVTargetStreamer {
private:
  // Add any WinCOFF-specific state here

public:
  RISCVTargetWinCOFFStreamer(MCStreamer &S, const MCSubtargetInfo &STI);
  
  // Override any ELF-specific methods that don't apply to COFF
  // For now, we can inherit most behavior from the base RISCVTargetStreamer
};

} // end namespace llvm

#endif
