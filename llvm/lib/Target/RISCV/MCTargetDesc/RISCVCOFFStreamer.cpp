//===-- RISCVTargetWinCOFFStreamer.cpp - RISC-V Target WinCOFF Streamer ---*- C++ -*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "RISCVCOFFStreamer.h"
#include "RISCVMCAsmInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"

using namespace llvm;

RISCVTargetWinCOFFStreamer::RISCVTargetWinCOFFStreamer(MCStreamer &S,
                                                       const MCSubtargetInfo &STI)
    : RISCVTargetStreamer(S) {
  // Initialize any WinCOFF-specific state here
  // For UEFI, we might want to set certain section flags or characteristics
}

// Note: Most of the functionality is inherited from RISCVTargetStreamer
// We only need to override methods that are ELF-specific
