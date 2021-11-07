javac -h . FluidSynth.java
gcc -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" -shared -o FluidSynth.dll FluidSynth.c -lfluidsynth