jumpWindow = nil
jumpToggle = nil
jumpButton = nil
looping = false
loopCoroutine = nil
function init()
    jumpToggle = modules.client_topmenu.addLeftGameButton('jumpButton', 'Jump!', '/images/topbuttons/questlog', toggle)
    jumpToggle:setOn(false)
    jumpWindow = g_ui.displayUI('jump')
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
    else
        jumpWindow:show()
        jumpWindow:raise()
        jumpWindow:focus()
        jumpToggle:setOn(true)
        looping = false;
    end
end

local myLoop = function ()
    print("StartLoop")
    print("EndLoop")
end

function jump()
    loopCoroutine = coroutine.create(myLoop)
    coroutine.resume(loopCoroutine)
end
 