# Exploit CVE-2016-5195 "Dirty Cow"

Este es un exploit del CVE-2016-5195, también conocido como "Dirty Cow", que permite sobrescribir un archivo de lectura en versiones del kernel de Linux del 2.6.22 al 4.8.

El exploit sobrescribe el archivo con un usuario sin permisos y escribe "Vaca Moo". Esta es una adaptación del exploit "Dirty Cow" que se puede encontrar en el siguiente enlace: https://github.com/dirtycow/dirtycow.github.io/blob/master/pokemon.c

## Uso

Para compilarlo, sigue el siguiente comando:

gcc -pthread cow.c -o cow

Para ejecutarlo, simplemente necesitas pasar como argumento el archivo que deseas modificar:

./cow <archivo>

## Advertencia

Este exploit se proporciona solo con fines educativos y no se debe utilizar para realizar actividades ilegales. El autor no se hace responsable del mal uso de esta herramienta.

Que tengas un buen día lleno de vacas 🐄!
