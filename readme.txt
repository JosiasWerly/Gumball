This project was rebooted in 11/2021, secundary branch is the old version
	
	~~~~dependencies~~~~
GLAD, GLFW, Opengl, GLSL
GLM, IMGUI, PicoJson, LUA(embeded)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	~~~~General Points~~~~
Currently gumballCore is the Engine and generates an .exe. 
The user can create it's onw project by cloning GumballProject
as the engine is looking for a DLL for hotreload; In other words:
-execute the engine .exe.
-extend your own version of GumballProject.


Editor part is very crap(editorOverlay.hpp/cpp), 
I'll have to remake everything soon.


The proccess for loading assets is need to be redone for supporting editor
features.

I've notice that an "overlay" should have a close relation with 
its "instance". So most likely I'll rewrite the render system to 
support this relation.

LUA is removed for the time being.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Gumball Objective:
Make a "GameEngine" that give the programmer all the access for exploring and build new ideias;

Gumball heading Rules:
-be 2d and 3d;
-possibility to load assets(textures, shaders, meshes, sounds);
-support c++, lua;
-editorTime/engineTime Scripts, runtime Scripts;
-the workflow cannot be too strange or wierd in base what we have now in the market(ue4, unity);
-the programmer must have all the access, thus exploring the morphology of code and experimenting. DON'T BE RELIGIOUS, make some multiple inheritance, functionPointers, 
get the memory by force. 

Be awesome:
"dont let good ideias spoil good work" - Bjarne Stroustrup.
"[...] value is not equal to work hour [...]" - StarShip troopers.

Programmers can create almost anything dont let concepts or ideias slow your creational process.
Have some fun \o/
