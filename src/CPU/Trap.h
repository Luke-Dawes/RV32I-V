#pragma once
#include <cstdint>
#include <variant>

enum trap_type {
	Exception, interrupt
};

struct Trap {
	bool interrupt;
	uint32_t cause;
	uint32_t tval;
    static Trap IllegalInstruction(uint32_t instruction)
    {
        return {
            false,
            static_cast<uint32_t>(ExceptionCause::IllegalInstruction),
            instruction
        };
    }

    static Trap Breakpoint()
    {
        return {
            false,
            static_cast<uint32_t>(ExceptionCause::Breakpoint),
            0
        };
    }

    static Trap EcallM()
    {
        return {
            false,
            static_cast<uint32_t>(ExceptionCause::EnvironmentCallFromMMode),
            0
        };
    }

    static Trap LoadMisaligned(uint32_t address)
    {
        return {
            false,
            static_cast<uint32_t>(ExceptionCause::LoadAddressMisaligned),
            address
        };
    }

    static Trap StoreMisaligned(uint32_t address)
    {
        return {
            false,
            static_cast<uint32_t>(ExceptionCause::StoreAddressMisaligned),
            address
        };
    }

    static Trap LoadAccessFault(uint32_t address)
    {
        return {
            false,
            static_cast<uint32_t>(ExceptionCause::LoadAccessFault),
            address
        };
    }

    static Trap StoreAccessFault(uint32_t address)
    {
        return {
            false,
            static_cast<uint32_t>(ExceptionCause::StoreAccessFault),
            address
        };
    }

    static Trap InstructionAccessFault(uint32_t address)
    {
        return {
            false,
            static_cast<uint32_t>(ExceptionCause::InstructionAccessFault),
            address
        };
    }

    static Trap InstructionAddressMisaligned(uint32_t address)
    {
        return {
            false,
            static_cast<uint32_t>(ExceptionCause::InstructionAddressMisaligned),
            address
        };
    }

    // ---------- Interrupts ----------

    static Trap MachineTimerInterrupt()
    {
        return {
            true,
            static_cast<uint32_t>(InterruptCause::MachineTimer),
            0
        };
    }

    static Trap MachineSoftwareInterrupt()
    {
        return {
            true,
            static_cast<uint32_t>(InterruptCause::MachineSoftware),
            0
        };
    }

    static Trap MachineExternalInterrupt()
    {
        return {
            true,
            static_cast<uint32_t>(InterruptCause::MachineExternal),
            0
        };
    }
};

enum class ExceptionCause : uint32_t
{
    InstructionAddressMisaligned = 0,
    InstructionAccessFault = 1,
    IllegalInstruction = 2,
    Breakpoint = 3,
    LoadAddressMisaligned = 4,
    LoadAccessFault = 5,
    StoreAddressMisaligned = 6,
    StoreAccessFault = 7,
    EnvironmentCallFromUMode = 8,
    EnvironmentCallFromSMode = 9,
    EnvironmentCallFromMMode = 11,
    InstructionPageFault = 12,
    LoadPageFault = 13,
    StorePageFault = 15
};

enum class InterruptCause : uint32_t
{
    UserSoftware = 0,
    SupervisorSoftware = 1,
    MachineSoftware = 3,

    UserTimer = 4,
    SupervisorTimer = 5,
    MachineTimer = 7,

    UserExternal = 8,
    SupervisorExternal = 9,
    MachineExternal = 11
};