#include "EquivalenceCheckingManager.hpp"
#include "benchmark/benchmark.h"

static void EquivalentDeutschProportional(benchmark::State& state) {
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

  qc1.import("./circuits/dj_mapped_ibm_washington_qiskit_opt0_8.qasm");
  qc2.import("./circuits/dj_mapped_ibm_washington_qiskit_opt3_8.qasm");

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, config);
    ecm.run();
  }
}
BENCHMARK(EquivalentDeutschProportional);

static void EquivalentDeutschDiff(benchmark::State& state) {
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

  qc1.import("./circuits/dj_mapped_ibm_washington_qiskit_opt0_8.qasm");
  qc2.import("./circuits/dj_mapped_ibm_washington_qiskit_opt3_8.qasm");

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, config);
    ecm.run();
  }
}
BENCHMARK(EquivalentDeutschDiff);

static void EquivalentGroverProportional(benchmark::State& state) {
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

  qc1.import("./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt0_8.qasm");
  qc2.import("./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm");

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, config);
    ecm.run();
  }
}
BENCHMARK(EquivalentGroverProportional);

static void EquivalentGroverDiff(benchmark::State& state) {
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

  qc1.import("./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt0_8.qasm");
  qc2.import("./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm");

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, config);
    ecm.run();
  }
}
BENCHMARK(EquivalentGroverDiff);

static void EquivalentGroverFullProportional(benchmark::State& state) {
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

  qc1.import("./circuits/grover-noancilla_indep_qiskit_8.qasm");
  qc2.import("./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm");

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, config);
    ecm.run();
  }
}
BENCHMARK(EquivalentGroverFullProportional);

static void EquivalentGroverFullDiff(benchmark::State& state) {
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

  qc1.import("./circuits/grover-noancilla_indep_qiskit_8.qasm");
  qc2.import("./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm");

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, config);
    ecm.run();
  }
}
BENCHMARK(EquivalentGroverFullDiff);