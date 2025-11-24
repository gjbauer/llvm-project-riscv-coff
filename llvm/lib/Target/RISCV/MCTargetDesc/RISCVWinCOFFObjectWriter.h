// RISCVWinCOFFObjectWriter.h
#ifndef LLVM_LIB_TARGET_RISCV_MCTARGETDESC_RISCVWINCOFFOBJECTWRITER_H
#define LLVM_LIB_TARGET_RISCV_MCTARGETDESC_RISCVWINCOFFOBJECTWRITER_H

#include "llvm/MC/MCObjectWriter.h"

namespace llvm {

LLVM_EXTERNAL_VISIBILITY std::unique_ptr<MCObjectTargetWriter> createRISCVWinCOFFObjectWriter(bool Is64Bit);

} // namespace llvm

#endif
