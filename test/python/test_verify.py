from __future__ import annotations

import pytest
from mqt import qcec
from qiskit import QuantumCircuit


@pytest.fixture
def original_circuit() -> QuantumCircuit:
    qc = QuantumCircuit(3)
    qc.h(0)
    qc.cx(0, 1)
    qc.cx(0, 2)
    qc.measure_all()
    return qc


@pytest.fixture
def alternative_circuit() -> QuantumCircuit:
    qc = QuantumCircuit(3, 3)
    qc.h(0)
    qc.cx(0, 1)
    qc.swap(0, 1)
    qc.cx(1, 2)
    qc.measure(0, 1)
    qc.measure(1, 0)
    qc.measure(2, 2)
    return qc


def test_verify(original_circuit: QuantumCircuit, alternative_circuit: QuantumCircuit) -> None:
    """
    Test the verification of two equivalent circuits.
    """
    result = qcec.verify(original_circuit, alternative_circuit)
    assert result.equivalence == qcec.EquivalenceCriterion.equivalent
