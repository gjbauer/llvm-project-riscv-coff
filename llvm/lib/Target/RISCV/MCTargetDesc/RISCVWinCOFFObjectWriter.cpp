//===-- RISCVWinCOFFObjectWriter.cpp - RISC-V COFF Writer -----------------===//
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
#include "RISCVWinCOFFObjectWriter.h"

using namespace llvm;

namespace {
class RISCVWinCOFFObjectWriter : public MCWinCOFFObjectTargetWriter {
public:
  RISCVWinCOFFObjectWriter(bool Is64Bit);

  ~RISCVWinCOFFObjectWriter() override;
  
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsCrossSection,
                        const MCAsmBackend &MAB) const override;
};

RISCVWinCOFFObjectWriter::RISCVWinCOFFObjectWriter(bool Is64Bit)
    : MCWinCOFFObjectTargetWriter(Is64Bit ? COFF::IMAGE_FILE_MACHINE_RISCV64
                                          : COFF::IMAGE_FILE_MACHINE_RISCV32) {}

RISCVWinCOFFObjectWriter::~RISCVWinCOFFObjectWriter() = default;

unsigned RISCVWinCOFFObjectWriter::getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsCrossSection,
                        const MCAsmBackend &MAB) const {
  const bool Is64Bit = getMachine() == COFF::IMAGE_FILE_MACHINE_RISCV64;
  unsigned Kind = Fixup.getKind();
  bool IsPCRel = Fixup.isPCRel();

  // For static linking, we need object file relocation types
  // The linker will convert these to base relocations for the final UEFI executable
  
  if (IsPCRel) {
    switch (Kind) {
    case RISCV::fixup_riscv_pcrel_hi20:
      return COFF::IMAGE_REL_RISCV_PCREL_HI20;
    case RISCV::fixup_riscv_pcrel_lo12_i:
      return COFF::IMAGE_REL_RISCV_PCREL_LO12_I;
    case RISCV::fixup_riscv_pcrel_lo12_s:
      return COFF::IMAGE_REL_RISCV_PCREL_LO12_S;
    case RISCV::fixup_riscv_jal:
      return COFF::IMAGE_REL_RISCV_JAL;
    case RISCV::fixup_riscv_branch:
      return COFF::IMAGE_REL_RISCV_BRANCH;
    case RISCV::fixup_riscv_call:
      return COFF::IMAGE_REL_RISCV_CALL;
    case RISCV::fixup_riscv_call_plt:
      return COFF::IMAGE_REL_RISCV_CALL_PLT;
    case RISCV::fixup_riscv_rvc_jump:
      return COFF::IMAGE_REL_RISCV_BRANCH; // Map to regular branch
    case RISCV::fixup_riscv_rvc_branch:
      return COFF::IMAGE_REL_RISCV_BRANCH;
    case FK_Data_4:
      return COFF::IMAGE_REL_RISCV_32;
    case FK_Data_8:
      return Is64Bit ? COFF::IMAGE_REL_RISCV_64 : COFF::IMAGE_REL_RISCV_32;
    default:
      break;
    }
  } else {
    switch (Kind) {
    case RISCV::fixup_riscv_hi20:
      return COFF::IMAGE_REL_RISCV_HI20;
    case RISCV::fixup_riscv_lo12_i:
      return COFF::IMAGE_REL_RISCV_LO12_I;
    case RISCV::fixup_riscv_lo12_s:
      return COFF::IMAGE_REL_RISCV_LO12_S;
    case FK_Data_4:
      return COFF::IMAGE_REL_RISCV_32;
    case FK_Data_8:
      return Is64Bit ? COFF::IMAGE_REL_RISCV_64 : COFF::IMAGE_REL_RISCV_32;
    case FK_SecRel_4:
      return COFF::IMAGE_REL_RISCV_32; // Use regular 32-bit for section-relative
    default:
      break;
    }
  }

  // Handle remaining fixups
  switch (Kind) {
  case RISCV::fixup_riscv_rvc_imm:
    Ctx.reportError(Fixup.getLoc(), "No direct relocation for CI-type instructions");
    return COFF::IMAGE_REL_RISCV_32;
  default:
    Ctx.reportError(Fixup.getLoc(), "unsupported relocation type for RISC-V COFF");
    return COFF::IMAGE_REL_RISCV_32;
  }
}
} // namespace

namespace llvm {
LLVM_EXTERNAL_VISIBILITY std::unique_ptr<MCObjectTargetWriter>
createRISCVWinCOFFObjectWriter(bool Is64Bit) {
  return std::make_unique<RISCVWinCOFFObjectWriter>(Is64Bit);
}
}
