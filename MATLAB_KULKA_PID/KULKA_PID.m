clc; clear all; close all;

s = tf('s');

%% Parametry obiektu
 
m = 0.0027;             % masa kulki 
R = 0.02;               %  pilki
d = 0.03;               % dlugosc ramienia silnika
g = -9.8;               % grawitacja
L = 0.24;                % dlugosc belki
J = 9.96 * 10^(-6);     % bezwladnosc
r = 0;                  % pozycja pilki



G_obiekt = (-m*g*d) / ( L * (J/(R^2)+m) ) / s^2
licznik = -m * g * d;
mianownik = L * (J/(R^2) + m);

%% Parametry serwonapędu SG90

K = 1;
T = 0.06;

G_sg90 = K/(s*T+1)

%% Parametry regulatora PID

kp = 0;
ki = 0;
kd = 0;


