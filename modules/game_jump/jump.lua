--This is my attempt to reacreate the module from the task
--At first, i tried to use ModalWindow, but no take so long to
--figure out this is not the way to go, so searching a lite bit more
--i find the world of modules.
--This tutorial helps me a lot https://github.com/edubart/otclient/wiki/Module-Tutorial
--but some funcitions and properties i had to find reading the code alreary created

--My first try was using lua coroutines to make the loop, but otclient didn't 
--liked and i receive lots of crash log, so i find scheduleEvent that makes
--everything work, i don't know if it is the right implementation, but is the
--best i was able to create

local moveEvent = nil
local jumpWindow = nil
local jumpToggle = nil
local jumpButton = nil
local xDisplacement = 0
local yDisplacement = 0

function init()
    --Here i create a button in topPanel to open the module,
    --create the module, hide it and get the reference of the jumpButton
    --so i can manipulate the position

    jumpToggle = modules.client_topmenu.addLeftGameButton('jumpButton', 'Jump!', '/images/topbuttons/questlog', toggle)
    jumpToggle:setOn(false)
    jumpWindow = g_ui.displayUI('jump')  --Create the module
    jumpWindow:hide()
    jumpButton = jumpWindow:getChildById('jumpButton')
end

function terminate()
    jumpWindow:destroy()
    jumpToggle:destroy()
end

function toggle()
    if jumpToggle:isOn() then
        jumpWindow:hide()
        jumpToggle:setOn(false)
        stopLoop()
    else
        jumpWindow:show()
        jumpWindow:raise()
        jumpWindow:focus()
        jumpToggle:setOn(true)
        doButtonJump()
        startLoop()
    end
end

--Create a recursive loop using scheduleEvent and addresses the event to moveEvent,
--so the code are able to stop the loop
function startLoop()
    moveEvent = scheduleEvent(function()
        moveJumpButton()
        startLoop()
    end, 10)
end

--This is the main funcition that calculates and set the new position to jumpButton
--also keep the button inside at the correct position when dragging the module
function moveJumpButton()
    xDisplacement = xDisplacement - 1
    if xDisplacement < 20 then
        xDisplacement = jumpWindow:getWidth() - jumpButton:getWidth() - 20
        doButtonJump()
    end
    jumpButton:setX(jumpWindow:getX() + xDisplacement)
    jumpButton:setY(jumpWindow:getY() + yDisplacement)
end

--Set new value to displace the Jump button in Y position inside the JumpWindow 
--with some margin to not overlap other elements
function doButtonJump()
    yDisplacement = math.random(50, jumpWindow:getWidth() - 200)  
end

function stopLoop()
    removeEvent(moveEvent)
end


