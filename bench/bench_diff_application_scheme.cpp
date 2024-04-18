#include "EquivalenceCheckingManager.hpp"
#include "benchmark/benchmark.h"

const ec::Configuration CONFIG = {
    {dd::RealNumber::eps, true,
     std::max(2U, std::thread::hardware_concurrency()), 0.0, false, false, true,
     false},
    {false, false, false, false, false, false},
    {},
    {},
    {},
    {}};

static void EquivalentDeutschDiff(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Diff;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("./circuits/dj_nativegates_ibm_qiskit_opt0_8.qasm");
  qc2.import("./circuits/dj_nativegates_ibm_qiskit_opt3_8.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentDeutschDiff);

static void EquivalentDeutschProportional(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Proportional;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("./circuits/dj_nativegates_ibm_qiskit_opt0_8.qasm");
  qc2.import("./circuits/dj_nativegates_ibm_qiskit_opt3_8.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentDeutschProportional);

static void EquivalentGroverDiff(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Diff;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("./circuits/grover-noancilla_nativegates_ibm_qiskit_opt0_8.qasm");
  qc2.import("./circuits/grover-noancilla_nativegates_ibm_qiskit_opt3_8.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentGroverDiff);

static void EquivalentGroverProportional(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Proportional;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("./circuits/grover-noancilla_nativegates_ibm_qiskit_opt0_8.qasm");
  qc2.import("./circuits/grover-noancilla_nativegates_ibm_qiskit_opt3_8.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentGroverProportional);

static void EquivalentGroverFullDiff(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Diff;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("./circuits/grover-noancilla_indep_qiskit_8.qasm");
  qc2.import(
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentGroverFullDiff);

static void EquivalentGroverFullProportional(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Proportional;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("./circuits/grover-noancilla_indep_qiskit_8.qasm");
  qc2.import(
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentGroverFullProportional);

static void EquivalentAlu1Diff(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Diff;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("../test/circuits/original/alu1_198.real");
  qc2.import("../test/circuits/transpiled/alu1_198_transpiled.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentAlu1Diff);

static void EquivalentAlu1Proportional(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Proportional;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("../test/circuits/original/alu1_198.real");
  qc2.import("../test/circuits/transpiled/alu1_198_transpiled.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentAlu1Proportional);

static void EquivalentShorDiff(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Diff;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("./circuits/shor_15_4_nativegates_ibm_qiskit_opt0_18.qasm");
  qc2.import("./circuits/shor_15_4_nativegates_ibm_qiskit_opt2_18.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentShorDiff);

static void EquivalentShorProportional(benchmark::State& state) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Proportional;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("./circuits/shor_15_4_nativegates_ibm_qiskit_opt0_18.qasm");
  qc2.import("./circuits/shor_15_4_nativegates_ibm_qiskit_opt2_18.qasm");

  size_t totalPeakUniqueTableSize = 0U;

  for (auto _ : state) {
    ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
    ecm.run();
    totalPeakUniqueTableSize += ecm.getResults().peakUniqueTableSize;
  }

  std::cout << state.name() << ": average peak unique node count = "
            << totalPeakUniqueTableSize /
                   static_cast<size_t>(state.iterations())
            << "\n";
}
BENCHMARK(EquivalentShorProportional);