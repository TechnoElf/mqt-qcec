OPENQASM 2.0;
include "qelib1.inc";
qreg q[11];
x q[10];
h q[0];
h q[1];
h q[2];
h q[3];
h q[4];
h q[5];
h q[6];
h q[7];
h q[8];
h q[9];
h q[10];
mcx q[0], q[10];
mcx q[0], q[1], q[10];
mcx q[0], q[1], q[2], q[10];
mcx q[0], q[1], q[2], q[3], q[10];
mcx q[0], q[1], q[2], q[3], q[4], q[10];
mcx q[0], q[1], q[2], q[3], q[4], q[5], q[10];
mcx q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[10];
mcx q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[10];
mcx q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[8], q[10];
mcx q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[8], q[9], q[10];
h q[0];
h q[1];
h q[2];
h q[3];
h q[4];
h q[5];
h q[6];
h q[7];
h q[8];
h q[9];
x q[0];
x q[1];
x q[2];
x q[3];
x q[4];
x q[5];
x q[6];
x q[7];
x q[8];
x q[9];
h q[9];
mcx q[0], q[1], q[2], q[3], q[4], q[5], q[6], q[7], q[8], q[9];
h q[9];
x q[0];
x q[1];
x q[2];
x q[3];
x q[4];
x q[5];
x q[6];
x q[7];
x q[8];
x q[9];
h q[0];
h q[1];
h q[2];
h q[3];
h q[4];
h q[5];
h q[6];
h q[7];
h q[8];
h q[9];
