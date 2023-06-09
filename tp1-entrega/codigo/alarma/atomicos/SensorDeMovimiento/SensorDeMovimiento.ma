[top]
components : movimiento@SensorDeMovimiento
out : senial_movimiento_out
in : movimiento_in
in : cmd_movimiento_in
Link : movimiento_in movimiento_in@movimiento
Link : cmd_movimiento_in cmd_movimiento_in@movimiento
Link : senial_movimiento_out@movimiento senial_movimiento_out

