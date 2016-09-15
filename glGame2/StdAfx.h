#pragma once

/*
Einstellungen:
Algemain > Zeichensatz > Multibyte-Zeichensatz-Verwenden
CV++ Verzeichnisse > Include verzeichnisse > "C:\A\C_Programme\gluw\Glew and Glut\freeglut\include" (add)
CV++ Verzeichnisse > Include verzeichnisse > "C:\A\C_Programme\gluw\Glew and Glut\glew-1.11.0\include" (add)
Linker > Eingabe > Zusätzliche Abhänighkeiten > "freeglut.lib; glew32.lib" (add)
CV++ Verzeichnisse > Bibliotecsverzeichnisse > "C:\A\C_Programme\gluw\Glew and Glut\glew-1.11.0\lib" (add)
CV++ Verzeichnisse > Bibliotecsverzeichnisse > "C:\A\C_Programme\gluw\Glew and Glut\freeglut\lib" (add)
Copy "C:\A\C_Programme\gluw\Glew and Glut\freeglut.dll" and "C:\A\C_Programme\gluw\Glew and Glut\glew32.dll" to project Folder
*/

#include <Windows.h>

#include <fstream>
#include <string>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
//Includeverzeichnisse "C:\A\C_Programme\glm\glm"
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include <time.h>

#include <GL\glew.h>
#include <GL\glut.h>
#include <GL\freeglut.h>
#include <GL\GL.h>