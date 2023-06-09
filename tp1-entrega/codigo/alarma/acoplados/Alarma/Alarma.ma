[top]
components : uc@UnidadDeControl sA@SensorDeApertura sM@SensorDeMovimiento sS@SensorDeSonido
out : sensores_out
out : alarma_out
in : movimiento_in
in : apertura_in
in : sonido_in
in : comando_in
Link : movimiento_in movimiento_in@sM
Link : apertura_in apertura_in@sA
Link : sonido_in sonido_in@sS
Link : comando_in comando_in@uc
Link : cmd_movimiento_out@uc cmd_movimiento_in@sM
Link : cmd_apertura_out@uc cmd_apertura_in@sA
Link : cmd_sonido_out@uc cmd_sonido_in@sS
Link : senial_apertura_out@sA senial_apertura_in@uc
Link : senial_movimiento_out@sM senial_movimiento_in@uc
Link : senial_sonido_out@sS senial_sonido_in@uc
Link : sensores_out@uc sensores_out
Link : alarma_out@uc alarma_out
