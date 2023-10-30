This is my fork of OTCLient - Redemption 

Here i replicate the module from the Job Application task
There are the scripts i created: https://github.com/metaldogs/otclient/tree/main/modules/game_jump
All the description from what i made are in the comments from jump.lua

This video shows what i can archive:

https://github.com/metaldogs/otclient/assets/12102397/055e52e4-1c3e-4779-9a1b-b0339a64e904

==========================================================================================

One week lather....

I still trying to replicate the question 6 effect, it's almost finished, whatch the video below.



https://github.com/metaldogs/otclient/assets/12102397/547a619f-2f86-4846-974b-25f786b40e15


I'm realy enjoying how things are going, i've had some great progress this past week. To get here, i tryed a lot of ideas that failed, but the paths i took taught me many things like:
- Opcodes and Networkpackets - Sending packets beteen server and client is realy easy with networkpackets, i realy liked how they are made, but I think I've only scratched the surface of this subject.
- Effects - Realy useful tool, but i still have to make some improvement, so when the effect render a outfit, the effect copies the colors and the animation of the main character, or it is already implement, i just dont know how to use.
- Fragment Shaders - Fragment shaders are easy to use, until you realise that is only applied to the base outfit, and you have to figure out how to mix with addons and the mount.
- Vertex Shaders - I tried use vertex shader to replicate the dash effect, i even did some modifications to the client to aceppt custom vetex shaders, but it gone wrong when i saw the color layer in the wrong position, as you can see in the video below.




https://github.com/metaldogs/otclient/assets/12102397/4bf95122-20ee-43bd-adcc-f788174323c3



Quick access to the scripts I made:

Outline Shader: https://github.com/metaldogs/otclient/blob/816f499930a35165d8f93e3118bf9517e2ab2bea/modules/game_shaders/shaders/fragment/furious.frag
Attempt to create the vertex dash shader: https://github.com/metaldogs/otclient/blob/816f499930a35165d8f93e3118bf9517e2ab2bea/modules/game_shaders/shaders/core/vertex/fast.vert
Added the new shaders to the array and created the function that accepts fragment and vertex shaders: https://github.com/metaldogs/otclient/blob/816f499930a35165d8f93e3118bf9517e2ab2bea/modules/game_shaders/shaders.lua
Creating the dash effect: https://github.com/metaldogs/otclient/blob/816f499930a35165d8f93e3118bf9517e2ab2bea/modules/game_attachedeffects/effects.lua
Applying shader and effect when receiving a network packet: https://github.com/metaldogs/otclient/blob/816f499930a35165d8f93e3118bf9517e2ab2bea/modules/game_attachedeffects/attachedeffects.lua
Creation of the action dash and sending the package to the client: https://github.com/metaldogs/forgottenserver/blob/9771c6dcb144e0c85b8859c19cac8d5b422b2e67/data/actions/scripts/others/dash.lua

There is still a lot of work to be done, such as applying the outfit color to the dash effect and, most importantly, improving the quality of the code.


==========================================================================================

Link to server i used: https://github.com/metaldogs/forgottenserver

I do not have problems to compile the client, since i already know how to compile the server,
the only problem i had, was to find the tibia client to copy the assest folder.

[# OTCLient - Redemption](https://github.com/edubart/otclient/wiki/Module-Tutorial#some-helpful-guidance)https://github.com/edubart/otclient/wiki/Module-Tutorial#some-helpful-guidance
