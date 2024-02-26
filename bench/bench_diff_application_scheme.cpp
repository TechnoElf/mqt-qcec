#include "EquivalenceCheckingManager.hpp"
#include "benchmark/benchmark.h"

static void EquivalentProportional(benchmark::State& state) {
  std::size_t            nqubits = 1U;
  qc::QuantumComputation qc1     = qc::QuantumComputation(nqubits);
  qc::QuantumComputation qc2     = qc::QuantumComputation(nqubits);
  ec::Configuration      config{};

  config.functionality.traceThreshold       = 1e-2;
  config.optimizations.fuseSingleQubitGates = false;
  config.optimizations.reorderOperations    = false;
  config.optimizations.reconstructSWAPs     = false;
  config.execution.runSimulationChecker     = false;
  config.execution.runAlternatingChecker    = true;
  config.execution.runConstructionChecker   = false;
  config.execution.runZXChecker             = false;
  config.application.alternatingScheme      = ec::ApplicationSchemeType::Proportional;

  qc1.import("./circuits/5xp1_194.real");
  qc2.import("./circuits/5xp1_194_transpiled.qasm");

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, config);
    ecm.run();
  }
}
BENCHMARK(EquivalentProportional);

static void EquivalentDiff(benchmark::State& state) {
  std::size_t            nqubits = 1U;
  qc::QuantumComputation qc1     = qc::QuantumComputation(nqubits);
  qc::QuantumComputation qc2     = qc::QuantumComputation(nqubits);
  ec::Configuration      config{};

  config.functionality.traceThreshold       = 1e-2;
  config.optimizations.fuseSingleQubitGates = false;
  config.optimizations.reorderOperations    = false;
  config.optimizations.reconstructSWAPs     = false;
  config.execution.runSimulationChecker     = false;
  config.execution.runAlternatingChecker    = true;
  config.execution.runConstructionChecker   = false;
  config.execution.runZXChecker             = false;
  config.application.alternatingScheme      = ec::ApplicationSchemeType::Diff;

  qc1.import("./circuits/5xp1_194.real");
  qc2.import("./circuits/5xp1_194_transpiled.qasm");

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, config);
    ecm.run();
  }
}
BENCHMARK(EquivalentDiff);