//===-- RISCVCOFFStreamer.cpp - RISC-V COFF Target Streamer Methods -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides RISC-V specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "RISCVTargetStreamer.h"
#include "RISCVAsmBackend.h"
#include "RISCVBaseInfo.h"
#include "RISCVMCTargetDesc.h"
#include "llvm/BinaryFormat/COFF.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCWin64EH.h"
#include "llvm/MC/MCWinCOFFStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCObjectWriter.h"

using namespace llvm;

namespace {
class RISCVWinCOFFStreamer : public MCWinCOFFStreamer {
  Win64EH::UnwindEmitter EHStreamer;
public:
  RISCVWinCOFFStreamer(MCContext &C, std::unique_ptr<MCAsmBackend> AB,
                     std::unique_ptr<MCCodeEmitter> CE,
                     std::unique_ptr<MCObjectWriter> OW)
      : MCWinCOFFStreamer(C, std::move(AB), std::move(CE), std::move(OW)) {}

  void emitInstruction(const MCInst &Inst, const MCSubtargetInfo &STI) override;
  void emitWinEHHandlerData(SMLoc Loc) override;
  void emitWindowsUnwindTables(WinEH::FrameInfo *Frame) override;
  void emitWindowsUnwindTables() override;
  void emitCVFPOData(const MCSymbol *ProcSym, SMLoc Loc) override;
  void finishImpl() override;
};

void RISCVWinCOFFStreamer::emitInstruction(const MCInst &Inst,
                                         const MCSubtargetInfo &STI) {
  this->MCObjectStreamer::emitInstruction(Inst, STI);
}

void RISCVWinCOFFStreamer::emitWinEHHandlerData(SMLoc Loc) {
  MCStreamer::emitWinEHHandlerData(Loc);

  // We have to emit the unwind info now, because this directive
  // actually switches to the .xdata section.
  if (WinEH::FrameInfo *CurFrame = getCurrentWinFrameInfo())
    EHStreamer.EmitUnwindInfo(*this, CurFrame, /* HandlerData = */ true);
}

void RISCVWinCOFFStreamer::emitWindowsUnwindTables(WinEH::FrameInfo *Frame) {
  EHStreamer.EmitUnwindInfo(*this, Frame, /* HandlerData = */ false);
}

void RISCVWinCOFFStreamer::emitWindowsUnwindTables() {
  if (!getNumWinFrameInfos())
    return;
  EHStreamer.Emit(*this);
}

void RISCVWinCOFFStreamer::emitCVFPOData(const MCSymbol *ProcSym, SMLoc Loc) {
  /*RISCVTargetStreamer *XTS =
      static_cast<RISCVTargetStreamer *>(getTargetStreamer());
  XTS->emitFPOData(ProcSym, Loc);*/
  // TODO!!
}

void RISCVWinCOFFStreamer::finishImpl() {
  emitFrames(nullptr);
  emitWindowsUnwindTables();

  MCWinCOFFStreamer::finishImpl();
}
} // namespace

namespace llvm {

MCStreamer *
createRISCVWinCOFFStreamer(MCContext &C, std::unique_ptr<MCAsmBackend> &&AB,
                               std::unique_ptr<MCObjectWriter> &&OW,
                               std::unique_ptr<MCCodeEmitter> &&CE) {
  return new RISCVWinCOFFStreamer(C, std::move(AB), std::move(CE), std::move(OW));
}

} // namespace llvm
