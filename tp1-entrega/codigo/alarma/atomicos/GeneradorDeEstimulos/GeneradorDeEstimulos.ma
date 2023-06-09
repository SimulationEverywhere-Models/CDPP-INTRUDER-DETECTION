[top]
components : genEv@GeneradorDeEstimulos
out : movimiento_out
out : sonido_out
out : apertura_out
out : control_out
out : estimulos_out
in : eventos_in
in : estabilidad_in
Link : eventos_in eventos_in@genEv
Link : estabilidad_in estabilidad_in@genEv
Link : movimiento_out@genEv movimiento_out
Link : sonido_out@genEv sonido_out
Link : apertura_out@genEv apertura_out
Link : control_out@genEv control_out
Link : estimulos_out@genEv estimulos_out

[genEv]
distribution : normal
mean : 5
variance : 2

