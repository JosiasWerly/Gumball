All code here is produce by Josias Werly 


############## Dependencies ##############
GLAD, GLFW, Opengl, GLSL
GLM, IMGUI, PicoJson, LUA(embeded)



############## Project Structure ##############

GumballApplication -> is the .exe that loads GumballCore.dll
GumballCore -> is the engine code.
GumballProject -> the project where the programmer can edit while GumballApplication is running, it will generate a .dll and will be hotrealoaded to the application.

-originaly made in vs2022, std C++x20, winSDK 10.0, ToolSet v143


############## Objectives ##############
- a step further then SFML.
- script language (most likely LUA/Rust)
- possible to extend.
- clear patterns && architecture (i'm creating this for me so there is almost none comments, however I try my best to create a clear code and pattern).


############## ToDo ##############
-texture and image class have a bad interaction between both.
-render piperline need improvement, SceneOverlay.cpp  SceneOverlay::OnRender.
-texture atlas reducing the ammount of calls.
-figure out a way to enable drawables to have shader.
-render target.
-gbuffer properly implemented, lighting pass, pos-process pass.
-find a managable way of interop between c++/lua/rust.
-sprite sheets, sprites animation.
-investigate how to do 3d animation.
-octa, quad, kd trees.
-improve interop between math classes.
-render culling.
-simple collisions procedures.



"dont let good ideias spoil good work" - Bjarne Stroustrup.
"[...] value is not equal to work hour [...]" - StarShip troopers.
This project was rebooted in 11/2021, 09/2022


KUDOS to:
Mr. Cherno: from whom I've learned the opengl from youtube.
Lucas Pazze: great friend and programmer.
Leonardo Murillo: great friend and colleague.