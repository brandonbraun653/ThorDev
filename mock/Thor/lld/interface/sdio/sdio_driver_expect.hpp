/*
 * This file has been auto-generated by CppUMockGen v0.6.
 *
 * Contents will NOT be preserved if it is regenerated!!!
 *
 * Generation options: -s gnu++20
 */

#include <CppUMockGen.hpp>

#include "Thor/lld/interface/sdio/sdio_driver.hpp"

#include <CppUTestExt/MockSupport.h>

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& Driver$ctor();
MockExpectedCall& Driver$ctor(unsigned int __numCalls__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& Driver$dtor(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__ = ::CppUMockGen::IgnoreParameter::YES);
MockExpectedCall& Driver$dtor(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__ = ::CppUMockGen::IgnoreParameter::YES);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& attach(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Thor::LLD::SDIO::RegisterMap *const peripheral, Chimera::Status_t __return__);
MockExpectedCall& attach(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Thor::LLD::SDIO::RegisterMap *const peripheral, Chimera::Status_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& reset(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
MockExpectedCall& reset(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& clockEnable(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
MockExpectedCall& clockEnable(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& clockDisable(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
MockExpectedCall& clockDisable(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& busClockEnable(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
MockExpectedCall& busClockEnable(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& busClockDisable(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
MockExpectedCall& busClockDisable(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& enterCriticalSection(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
MockExpectedCall& enterCriticalSection(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& exitCriticalSection(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
MockExpectedCall& exitCriticalSection(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& getBusFrequency(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& getBusFrequency(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& init(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
MockExpectedCall& init(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& deinit(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
MockExpectedCall& deinit(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& setPowerStateOn(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
MockExpectedCall& setPowerStateOn(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& setPowerStateOff(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
MockExpectedCall& setPowerStateOff(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, Chimera::Status_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& setSDMMCReadWaitMode(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<uint32_t> mode, Chimera::Status_t __return__);
MockExpectedCall& setSDMMCReadWaitMode(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<uint32_t> mode, Chimera::Status_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cpsmPutCmd(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const Thor::LLD::SDIO::CPSMCommand &> cmd, Chimera::Status_t __return__);
MockExpectedCall& cpsmPutCmd(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const Thor::LLD::SDIO::CPSMCommand &> cmd, Chimera::Status_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cpsmGetCmdResponse(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint8_t __return__);
MockExpectedCall& cpsmGetCmdResponse(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint8_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cpsmGetRespX(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint8_t> which, uint32_t __return__);
MockExpectedCall& cpsmGetRespX(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint8_t> which, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& dpsmConfigure(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const Thor::LLD::SDIO::DPSMConfig &> config, Chimera::Status_t __return__);
MockExpectedCall& dpsmConfigure(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const Thor::LLD::SDIO::DPSMConfig &> config, Chimera::Status_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& dpsmGetDataCounter(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& dpsmGetDataCounter(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& dpsmGetFIFOCount(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& dpsmGetFIFOCount(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdAppCommand(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
MockExpectedCall& cmdAppCommand(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdAppOperCommand(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
MockExpectedCall& cmdAppOperCommand(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdBlockLength(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> BlockSize, uint32_t __return__);
MockExpectedCall& cmdBlockLength(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> BlockSize, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdBusWidth(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> BusWidth, uint32_t __return__);
MockExpectedCall& cmdBusWidth(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> BusWidth, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdErase(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& cmdErase(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdEraseEndAdd(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> EndAdd, uint32_t __return__);
MockExpectedCall& cmdEraseEndAdd(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> EndAdd, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdEraseStartAdd(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> StartAdd, uint32_t __return__);
MockExpectedCall& cmdEraseStartAdd(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> StartAdd, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdGoIdleState(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& cmdGoIdleState(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdOpCondition(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
MockExpectedCall& cmdOpCondition(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdOperCond(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& cmdOperCond(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdReadMultiBlock(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> ReadAdd, uint32_t __return__);
MockExpectedCall& cmdReadMultiBlock(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> ReadAdd, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdReadSingleBlock(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> ReadAdd, uint32_t __return__);
MockExpectedCall& cmdReadSingleBlock(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> ReadAdd, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSDEraseEndAdd(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> EndAdd, uint32_t __return__);
MockExpectedCall& cmdSDEraseEndAdd(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> EndAdd, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSDEraseStartAdd(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> StartAdd, uint32_t __return__);
MockExpectedCall& cmdSDEraseStartAdd(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> StartAdd, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSelDesel(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint64_t> Addr, uint32_t __return__);
MockExpectedCall& cmdSelDesel(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint64_t> Addr, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSendCID(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& cmdSendCID(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSendCSD(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
MockExpectedCall& cmdSendCSD(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSendEXTCSD(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
MockExpectedCall& cmdSendEXTCSD(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSendSCR(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& cmdSendSCR(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSendStatus(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
MockExpectedCall& cmdSendStatus(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSetRelAdd(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint16_t * pRCA, size_t __sizeof_pRCA, uint32_t __return__);
MockExpectedCall& cmdSetRelAdd(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint16_t * pRCA, size_t __sizeof_pRCA, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSetRelAddMmc(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint16_t> RCA, uint32_t __return__);
MockExpectedCall& cmdSetRelAddMmc(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint16_t> RCA, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdStatusRegister(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& cmdStatusRegister(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdStopTransfer(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& cmdStopTransfer(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdSwitch(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
MockExpectedCall& cmdSwitch(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> Argument, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdWriteMultiBlock(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> WriteAdd, uint32_t __return__);
MockExpectedCall& cmdWriteMultiBlock(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> WriteAdd, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& cmdWriteSingleBlock(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> WriteAdd, uint32_t __return__);
MockExpectedCall& cmdWriteSingleBlock(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<const uint32_t> WriteAdd, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& getCmdResp1(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<uint8_t> SD_CMD, CppUMockGen::Parameter<uint32_t> Timeout, uint32_t __return__);
MockExpectedCall& getCmdResp1(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<uint8_t> SD_CMD, CppUMockGen::Parameter<uint32_t> Timeout, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& getCmdResp2(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& getCmdResp2(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& getCmdResp3(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& getCmdResp3(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& getCmdResp6(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<uint8_t> SD_CMD, uint16_t * pRCA, size_t __sizeof_pRCA, uint32_t __return__);
MockExpectedCall& getCmdResp6(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, CppUMockGen::Parameter<uint8_t> SD_CMD, uint16_t * pRCA, size_t __sizeof_pRCA, uint32_t __return__);
} } } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace SDIO$ { namespace Driver$ {
MockExpectedCall& getCmdResp7(CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
MockExpectedCall& getCmdResp7(unsigned int __numCalls__, CppUMockGen::Parameter<const Thor::LLD::SDIO::Driver*> __object__, uint32_t __return__);
} } } } }

