[top]
components : uc@UnidadDeControl
out : cmd_apertura_out
out : cmd_movimiento_out
out : cmd_sonido_out
out : sensores_out
out : alarma_out
in : comando_in
in : senial_apertura_in
in : senial_movimiento_in
in : senial_sonido_in
Link : comando_in comando_in@uc
Link : senial_apertura_in senial_apertura_in@uc
Link : senial_movimiento_in senial_movimiento_in@uc
Link : senial_sonido_in senial_sonido_in@uc
Link : cmd_apertura_out@uc cmd_apertura_out
Link : cmd_movimiento_out@uc cmd_movimiento_out
Link : cmd_sonido_out@uc cmd_sonido_out
Link : sensores_out@uc sensores_out
Link : alarma_out@uc alarma_out

