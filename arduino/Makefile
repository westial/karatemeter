# This script creates the Karatemeter Arduino based library files.

KARATEDIRNAME=Karatemeter
RELEASEDIRNAME=prebuild
ARDUINOSKETCHSRC=karatemeterino.src
ARDUINOSKETCHDST=karatemeterino
CONFIGDIRNAME=config

install: library.properties.template ../include/usecase/MeasureAndDisplayByModeUseCase.h ../src/usecase/MeasureAndDisplayByModeUseCase.c
	mkdir -p "${RELEASEDIRNAME}/${KARATEDIRNAME}/${CONFIGDIRNAME}"
	cp -R "${ARDUINOSKETCHSRC}" "${RELEASEDIRNAME}/"
	mv "${RELEASEDIRNAME}/${ARDUINOSKETCHSRC}" "${RELEASEDIRNAME}/${ARDUINOSKETCHDST}"
	find ../include/ -type f -name *.h -exec cp {} "${RELEASEDIRNAME}/${KARATEDIRNAME}/" \;
	find ../src/ -type f -name *.c -exec cp {} "${RELEASEDIRNAME}/${KARATEDIRNAME}/" \;
	mv "${RELEASEDIRNAME}/${KARATEDIRNAME}/displayconfig.h" "${RELEASEDIRNAME}/${ARDUINOSKETCHDST}/"
	mv "${RELEASEDIRNAME}/${KARATEDIRNAME}/impactconfig.h" "${RELEASEDIRNAME}/${ARDUINOSKETCHDST}/"
	mv "${RELEASEDIRNAME}/${KARATEDIRNAME}/typeconfig.h" "${RELEASEDIRNAME}/${KARATEDIRNAME}/${CONFIGDIRNAME}/"
	cp ./library.properties.template "${RELEASEDIRNAME}/${KARATEDIRNAME}/library.properties"

clean:
	rm -Rf "${RELEASEDIRNAME}"
