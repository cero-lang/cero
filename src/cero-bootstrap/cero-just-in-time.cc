// SPDX-License-Identifier: Apache-2.0
// https://github.com/llvm/llvm-project/tree/main/llvm/examples/OrcV2Examples

#include "cero-just-in-time.hh"

#include "cero-codegen-llvm.hh"

#include <llvm/ExecutionEngine/Orc/LLJIT.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/Orc/ThreadSafeModule.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/TargetSelect.h>

using namespace llvm;
using namespace llvm::orc;

namespace Cero {

JIT::JIT()
{
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  auto jit_process_control = ExitOnErr(SelfExecutorProcessControl::Create());
  auto jit = ExitOnErr(LLJITBuilder().setExecutorProcessControl(std::move(jit_process_control)).create());
  auto jit_thread_safe_module = ThreadSafeModule(std::move(module), std::move(context));

  ExitOnErr(jit->addIRModule(std::move(jit_thread_safe_module)));

  // Hook to print modules as they are compiled:
  jit->getIRTransformLayer().setTransform([&jit](ThreadSafeModule TSM, const MaterializationResponsibility &R) -> Expected<ThreadSafeModule> {
        TSM.withModuleDo([&jit](const Module &M) {
          dbgs() << M << "\n"; jit->getExecutionSession().dump(dbgs()); dbgs() << "\n\n";
        });
    return TSM;
  });

  const auto jit_main = ExitOnErr(jit->lookup("main"));
  const auto jit_main_callback = reinterpret_cast<int (*)()>(jit_main.getAddress());
  outs() << "JIT ... Running main()\n";
  const auto jit_exit_result = jit_main_callback();
  outs() << "JIT ... main exited with code " << jit_exit_result << ".\n";
}

} // namespace Cero
