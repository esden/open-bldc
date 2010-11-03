function current_sensor()

# Opamp gain resistors
R = 5400.0;
r = 1000.0;

# Opamp input resistor network
r1 = 660.0;
r2 = 560.0;
r3 = 4700.0;

# Shunt resistance
shunt = 0.0025;

# Opamp gain
printf("Opamp gain: ");
G = (R + r)/r

# Relation between the input signal e and signal v calculation

A = -100.0:1.0:100.0;
shunt_v = shunt * A;

# k parameter
k = ( 1/r1 + 1/r2 + 1/r3);
U = 5.0;

# translation voltage
printf("Input translate: ");
trans = U/(k*r3)

printf("Oputput translate: ");
out_trans = trans * G

# opamp input
v = (trans + shunt_v/(k*r1));

# opamp output
S = v * G;

plot(A, shunt_v, "1", A, v, "2", A, S, "3")
legend('e', 'v', 'S');