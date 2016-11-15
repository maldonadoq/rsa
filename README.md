# rsa
rsa algorithm
Percy Maldonado Quispe UCSP-CS

Pasos
1) generacion de primos $p$ y $q$, con la criba se llega hasta 32 en un tiempo .... razonable?
	30 bits: 50.2460000000000022168933355715 s
	32 bits: 202.2290000000000134150468511507 s
	
luego, es que necesitamos numeros primos grandes de 1024 a mas bits, y eso la criba no lo hace, usamos la generacion de primos probabilisticos, ahora, yo estoy usando miller-rabin, con 1000 iteraciones para saber si es primo o no, con 1000 iteraciones es mas seguro si se podria decir.

2) Se calcula:
	$N = pq$ 
3) Se calcula \phi de N
	$\phi(N) = (p-1)(q-1)$ (la funcion phi de euler)
4) Se escoge un entero positivo $e$, que pertenece a $\mathbb{Z}_{\phi(N)}$
	e: es la clave publica
5) Calcular $d = e^{-1} (mod \phi(N))$
	$ed \equiv 1 (mod \phi(N))$
	d: es la clave privada

Par publico: (e, N)
Par privado: (d, N)


Y todo lo demas

Ahora, este algoritmo, esta trabajando hasta ahora, con todo lo que nos dijo Dr. Ana Maria Cuadros (si es que lo esta leyendo, eh hola)

i) resto_chino_rsa() eficiencia del descifrado, conocemos p y q

ii) cifrado() agarra un texto, que esta en la carpeta sws/t_pl.txt, eso lo cifra en bloques, como ya nos explico, lo guarda en sms/t_ci.txt

iii) descifrado() hace exactamente lo mismo que el cifrado pero al revez, lo guarda en ss/t_dc.txt

iv) test_de_primalida() Evalua si p, q y e sean primos (ahora, hoy me dijeron que e no tiene que ser primo, yo entiendo, pero es que a mi me asegura esto es que e tenga inversa, defrente lo calculo, en otro caso si seria un random, este se evaaluaria si es que tiene inversa en \phi(N))

v) miller() hace las iteraciones, verifica, retorna true o false

vi) pthr() hace la evaluacion del test miller, hasta que pase el test, y asi para p, q y e

ahora, yo no se si me equivoque, es que yo hahah habia hecho diffie-hellman y cesar para el rsa, yo entendi que primero se iba a cifrar con cesar y luego con rsa cifrado, pero no es asi, asi que lo comente, ya lo sacare.

Aleatorio.
../func/random.cpp

en este archivo, estan el algoritmo, LSRG o como se escriba, lo que nos explico en laboratorio, la semilla la saco de fback.cpp, opencv.

w) _random(), esta funcion me devuelve un numero random segun la semilla que le pasemos, y el tamaño en bits de este numero random. dentro hay las funciones sum() y dez(), estas hacen:

	sum(): hace el crecimiento de la semilla segun el estado en que este la semilla
	dez(): hace el desplazamiento de la semilla, la parte en bloques de dos, y hace un corrimiento a la derecha y otro a la izquierda, y suma con los randoms que sacamos con anterioridad para la suma (mod 2) y eso le ponemos en las casillas vacias por el corrimiento



otra librerias creadas:
	-) rsa_fun.cpp : Contiene las funciones para el cifrado, concatenar, convertir a string, etc
	-) func.cpp : Contiene las funciones para el calculo de la clave privada, como el algoritmo extendido de euclides, mcd(), modulo().
	-) criba.cpp : La criba de Eratostenes, no es de mi autoria, pero bueno, ahi esta, como le dije en clases, buscando, me encontre con esta, la criba segmentada. 

/*	
	eh, yo eh estado tratando de hacer en paralelo la generacion de p, q y e, pero es que, no se por que me manda error, violacion de segmento generado. Yo eh utilizado thread para algunos otros algoritmos, ahora para hacer lo que le digo, no me funciona, le doy una hebra a p, q, y e, y no lo hace, eh probado para una funcion aparte pequeña, y hace, pero si llam dentro de esa funcion a miller rabin, no lo toca, o no se que hace, me manda error, lo deje ahi nomas, tambien esta comentado, pues si lo calcula al mismo tiempo seria mejor, no se si me deje explicar. Yo lo que eh podido ver, es que no me deja llamar otra funcion dentro de la que esta en un thread, pero tambien eh probado eso en pequelo, y si normal llama, pero con miller no, y ya no se cual es el error en ese caso
*/
