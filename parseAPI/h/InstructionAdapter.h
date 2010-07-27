/*
 * Copyright (c) 1996-2009 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#if !defined(INSTRUCTION_ADAPTER_H)
#define INSTRUCTION_ADAPTER_H

#include "dynutil/h/dyntypes.h"

#include "parseAPI/h/CodeObject.h"
#include "parseAPI/h/CFG.h"

#if !defined(ESSENTIAL_PARSING_ENUMS)
#define ESSENTIAL_PARSING_ENUMS
// There are three levels of function-level "instrumentability":
// 1) The function can be instrumented normally with no problems (normal case)
// 2) The function contains unresolved indirect branches; we have to assume
//    these can go anywhere in the function to be safe, so we must instrument
//    safely (e.g., with traps)
// 3) The function is flatly uninstrumentable and must not be touched.
enum InstrumentableLevel {
    NORMAL,
    HAS_BR_INDIR,
    UNINSTRUMENTABLE
};
#endif //!defined(ESSENTIAL_PARSING_ENUMS)

namespace Dyninst {
namespace InsnAdapter {

class InstructionAdapter
{
    public:
        InstructionAdapter(Address start, ParseAPI::CodeObject *o , 
            ParseAPI::CodeRegion* r, InstructionSource * isrc);
        virtual ~InstructionAdapter();
    // Implemented
    virtual bool hasCFT() const = 0;
    virtual size_t getSize() const = 0;
    virtual bool isFrameSetupInsn() const = 0;
    virtual bool isAbortOrInvalidInsn() const = 0;
    virtual void
            getNewEdges(std::vector<std::pair<Address,ParseAPI::EdgeTypeEnum> >&
            outEdges, 
            ParseAPI::Function* context,
            ParseAPI::Block* currBlk,
            unsigned int num_insns,
            dyn_hash_map<Address, std::string> *pltFuncs) const =
0;
    virtual bool isDynamicCall() const = 0;
    virtual bool isAbsoluteCall() const = 0;
    virtual InstrumentableLevel getInstLevel(ParseAPI::Function* context, unsigned int num_insns) const;
    virtual ParseAPI::FuncReturnStatus getReturnStatus(ParseAPI::Function* context, unsigned int num_insns) const ;
    virtual bool hasUnresolvedControlFlow(ParseAPI::Function* context, unsigned int num_insns)
const;
    virtual ParseAPI::StackTamper tampersStack(ParseAPI::Function *, Address &) const 
        { return ParseAPI::TAMPER_NONE; }
    virtual bool isNopJump() const { return false; }
    virtual bool simulateJump() const= 0;
    virtual void advance() = 0;
    virtual void retreat() = 0;
    virtual bool isNop() const = 0;
    virtual bool isLeave() const = 0;
    virtual bool isDelaySlot() const = 0;
    virtual bool isRelocatable(InstrumentableLevel lvl) const = 0;
    virtual Address getAddr() const;
    virtual Address getPrevAddr() const;
    virtual Address getNextAddr() const;
    virtual Address getCFT() const = 0;
    virtual bool isStackFramePreamble() const = 0;
    virtual bool savesFP() const = 0;
    virtual bool cleansStack() const = 0;
    virtual bool isConditional() const = 0;
    virtual bool isBranch() const = 0;
    virtual bool isInterruptOrSyscall() const = 0;
    virtual bool isCall() const = 0;
    virtual bool isReturnAddrSave() const = 0;
    virtual bool isTailCall(ParseAPI::Function *,unsigned int num_insns) const = 0;
    protected:
        virtual bool isReturn() const = 0;
        virtual bool isRealCall() const = 0;
        Address current;
    Address previous;
    mutable bool parsedJumpTable;
    mutable bool successfullyParsedJumpTable;
    mutable bool isDynamicCall_;
    mutable bool checkedDynamicCall_;
    mutable bool isInvalidCallTarget_;
    mutable bool checkedInvalidCallTarget_;
    ParseAPI::CodeObject * _obj;
    ParseAPI::CodeRegion * _cr;
    InstructionSource * _isrc;
};

} // Dyninst
} // InsnAdapter

#endif // !defined(INSTRUCTION_ADAPTER_H)
