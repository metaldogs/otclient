controller = Controller:new()

-- uncomment this line to apply an effect on the local player, just for testing purposes.

function controller:onGameStart()
    --g_game.getLocalPlayer():attachEffect(g_attachedEffects.getById(5))
    registerNewDashEffect()
    connect(g_game, {
        onGameStart = testStart,
        onGameEnd = testEnd
    }, true)

    ProtocolGame.registerExtendedOpcode(56, onRecvOpcode)
end

function controller:onGameEnd()
    g_game.getLocalPlayer():clearAttachedEffects()
end

function controller:onTerminate()
    disconnect(g_game, {
        onGameStart = testStart,
        onGameEnd = testEnd
    })

    g_attachedEffects.clear()
end

local function onAttach(effect, owner)
    local category, thingId = AttachedEffectManager.getDataThing(owner)
    local config = AttachedEffectManager.getConfig(effect:getId(), category, thingId)
    if config.isThingConfig then
        AttachedEffectManager.executeThingConfig(effect, category, thingId)
    end

    if config.onAttach then
        config.onAttach(effect, owner, config.__onAttach)
    end
end

local function onDetach(effect, oldOwner)
    local category, thingId = AttachedEffectManager.getDataThing(oldOwner)
    local config = AttachedEffectManager.getConfig(effect:getId(), category, thingId)

    if config.onDetach then
        config.onDetach(effect, oldOwner, config.__onDetach)
    end
end

local function onOutfitChange(creature, outfit, oldOutfit)
    for _i, effect in pairs(creature:getAttachedEffects()) do
        AttachedEffectManager.executeThingConfig(effect, ThingCategoryCreature, outfit.type)
    end
end

controller:attachExternalEvent(EventController:new(LocalPlayer, {
    onOutfitChange = onOutfitChange
}))
controller:attachExternalEvent(EventController:new(Creature, {
    onOutfitChange = onOutfitChange
}))
controller:attachExternalEvent(EventController:new(AttachedEffect, {
    onAttach = onAttach,
    onDetach = onDetach
}))

function onRecvOpcode(protocol, opcode, packet)
    local effect = tonumber(packet)
    local player = g_game.getLocalPlayer()
    if (effect < 10) then
        player:attachEffect(g_attachedEffects.getById(effect))
    else       
        for i = 5, 1, -1 do
            player:attachEffect(g_attachedEffects.getById(9 + i))
        end
        player:setShader('Outfit - Furious')
        scheduleEvent(function()
            player:setShader('Default')
        end, 500)
    end
end

function testStart()
    print('opcode registred')
end

function testEnd()
    ProtocolGame.unregisterExtendedOpcode(56, true)
end
