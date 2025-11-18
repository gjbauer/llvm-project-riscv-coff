//===-- RISCVCOFFObjectWriter.cpp - RISC-V COFF Writer --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/RISCVFixupKinds.h"
#include "MCTargetDesc/RISCVMCAsmInfo.h"
#include "MCTargetDesc/RISCVMCTargetDesc.h"
#include "llvm/BinaryFormat/COFF.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCWinCOFFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class RISCVWinCOFFObjectWriter : public MCWinCOFFObjectTargetWriter {
public:
  RISCVWinCOFFObjectWriter(bool Is64Bit);

  ~RISCVWinCOFFObjectWriter() override;
  
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                     const MCFixup &Fixup, bool IsPCRel) const {
    llvm_unreachable("Relocations not yet implemented for RISC-V COFF");
    return 0;
  }
};
}

RISCVWinCOFFObjectWriter::RISCVWinCOFFObjectWriter(bool Is64Bit)
    : MCWinCOFFObjectTargetWriter(Is64Bit) {}

RISCVWinCOFFObjectWriter::~RISCVWinCOFFObjectWriter() = default;
