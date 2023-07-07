/*
 * This file has been auto-generated by CppUMockGen v0.6.
 *
 * Contents will NOT be preserved if it is regenerated!!!
 *
 * Generation options: -s c++20
 */

#include <CppUMockGen.hpp>

#include "Thor/lld/common/cortex-m4/interrupts.hpp"

#include <CppUTestExt/MockSupport.h>

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& setPriorityGrouping(CppUMockGen::Parameter<const uint32_t> priorityGroup);
MockExpectedCall& setPriorityGrouping(unsigned int __numCalls__, CppUMockGen::Parameter<const uint32_t> priorityGroup);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& getPriorityGrouping(uint32_t __return__);
MockExpectedCall& getPriorityGrouping(unsigned int __numCalls__, uint32_t __return__);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& setPriority(CppUMockGen::Parameter<const IRQn_Type> IRQn, CppUMockGen::Parameter<const uint32_t> preemptPriority, CppUMockGen::Parameter<const uint32_t> subPriority);
MockExpectedCall& setPriority(unsigned int __numCalls__, CppUMockGen::Parameter<const IRQn_Type> IRQn, CppUMockGen::Parameter<const uint32_t> preemptPriority, CppUMockGen::Parameter<const uint32_t> subPriority);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& getPriority(CppUMockGen::Parameter<const IRQn_Type> IRQn, CppUMockGen::Parameter<const uint32_t> priorityGroup, uint32_t *const preemptPriority, size_t __sizeof_preemptPriority, uint32_t *const subPriority, size_t __sizeof_subPriority);
MockExpectedCall& getPriority(unsigned int __numCalls__, CppUMockGen::Parameter<const IRQn_Type> IRQn, CppUMockGen::Parameter<const uint32_t> priorityGroup, uint32_t *const preemptPriority, size_t __sizeof_preemptPriority, uint32_t *const subPriority, size_t __sizeof_subPriority);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& enableIRQ(CppUMockGen::Parameter<const IRQn_Type> IRQn);
MockExpectedCall& enableIRQ(unsigned int __numCalls__, CppUMockGen::Parameter<const IRQn_Type> IRQn);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& disableIRQ(CppUMockGen::Parameter<const IRQn_Type> IRQn);
MockExpectedCall& disableIRQ(unsigned int __numCalls__, CppUMockGen::Parameter<const IRQn_Type> IRQn);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& setPendingIRQ(CppUMockGen::Parameter<const IRQn_Type> IRQn);
MockExpectedCall& setPendingIRQ(unsigned int __numCalls__, CppUMockGen::Parameter<const IRQn_Type> IRQn);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& clearPendingIRQ(CppUMockGen::Parameter<const IRQn_Type> IRQn);
MockExpectedCall& clearPendingIRQ(unsigned int __numCalls__, CppUMockGen::Parameter<const IRQn_Type> IRQn);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& getPendingIRQ(CppUMockGen::Parameter<const IRQn_Type> IRQn, uint32_t __return__);
MockExpectedCall& getPendingIRQ(unsigned int __numCalls__, CppUMockGen::Parameter<const IRQn_Type> IRQn, uint32_t __return__);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& getActive(CppUMockGen::Parameter<const IRQn_Type> IRQn, uint32_t __return__);
MockExpectedCall& getActive(unsigned int __numCalls__, CppUMockGen::Parameter<const IRQn_Type> IRQn, uint32_t __return__);
} } } }

namespace expect { namespace Thor$ { namespace LLD$ { namespace INT$ {
MockExpectedCall& SystemReset();
MockExpectedCall& SystemReset(unsigned int __numCalls__);
} } } }
