#!/bin/bash
   
################################################################################

# Tipos de respuestas para todos los protocolos.
    REPLY_YES=%s  # Respuesta Si.
    REPLY_NO=%s   # Respuesta No.

# Protocolos de Error. 
# Espera una respuesta de parte de NiNo, Sí o No.
    ERR_Q_PROTO=%s # Error de pregunta.
    ERR_I_PROTO=%s # Error de información.
    ERR_W_PROTO=%s # Error de alerta.

# Protocolos de Informaciones.
# Espera una respuesta de parte de NiNo, Sí solamente.
    INFO_PROTO=%s  # Información.
    WARN_PROTO=%s  # Alerta.

# Protocolos de Salida. Silenciosa.
# No espera ninguna respuesta de parte de NiNo
    QUIT_PROTO=%s  # Salir de NiNo

# Delimitador de formato   
    DELIM=%s
#########################################################################

    fifo=%s  
    retraso=%s
    repetir=%s  

# Ubicación y nombre del archivo gif
    gifanimado="%s"

# Lista de archivos de imágenes a convertir.
    lista_imgs="%s"

    app=convert
#########################################################################
    
    function BeforeConvert () {
	
	if [ ! -x /usr/bin/$app ]; then
	    titulo="GifAnimado - Error"
	    mensaje="No se encontró /usr/bin/$app."
	    echo $WARN_PROTO$DELIM$titulo$DELIM$mensaje > $fifo
	    sync
	    read -n 1 continuar < $fifo
	    return 0
	fi

	if [ -e `pwd`/$gifanimado ]; then
	    titulo="GifAnimado - Pregunta"
	    mensaje="Ya existe el archivo $gifanimado. ¿Sobrescribirlo?"
	    echo $ERR_Q_PROTO$DELIM$titulo$DELIM$mensaje > $fifo
	    sync
	    read -n 1 continuar < $fifo
	    if [ "$continuar" == "$REPLY_NO" ]; then 
		return 0
	    fi
	fi
	return 1
    }

    function Convert () {
		local imgs=$(echo $lista_imgs | sed "s/'/ /g") # Elimina el caract. '

		$app -delay $retraso -loop $repetir $imgs  $gifanimado
		
		if [ $? == 1 ]; then
			titulo="GifAnimado - Error"
			mensaje="Ocurrió un error al ejecutar '$app'"
			
			echo $WARN_PROTO$DELIM$titulo$DELIM$mensaje > $fifo
			sync
			
			read -n 1 continuar < $fifo
		else
			titulo="GifAnimado - Información"
			mensaje="La conversión terminó." 
			
			echo $INFO_PROTO$DELIM$titulo$DELIM$mensaje > $fifo
			sync
			
			read -n 1 continuar < $fifo
			
				echo $QUIT_PROTO$DELIM$REPLY_YES > $fifo
				sync
		fi
    }


########################################################################

BeforeConvert;

if [ "$?" == "1" ]; then
    Convert;
fi

exit 0
########################################################################
