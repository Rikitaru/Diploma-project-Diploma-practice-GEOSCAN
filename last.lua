local boardNumber = boardNumber
local tblUnpack = table.unpack

function callback(event)

end

local Color = {}
    Color.leds_number = 4
    Color.leds = Ledbar.new(Color.leds_number)
    Color.colors = {    red =       {1, 0, 0},
                        green =     {0, 1, 0},
                        blue =      {0, 0, 1},
                        purple =    {0.5, 0, 0.5},
                        cyan =      {0, 0.5, 0.5},
                        yellow =    {0.5, 0.5, 0},
                        white =     {0.33, 0.33, 0.33},
                        black =     {0, 0, 0}
                    }
function Color.setAllLEDs(r, g, b)
    for i = 0, (Color.leds_number - 1), 1 do
        Color.leds:set(i, r, g, b)
    end
end
function Color.setLEDs(r, g, b, n)
    for i = 0, n - 1, 1 do
        Color.leds:set(i, r, g, b)
    end
end

local rc = Sensors.rc
local obj = {}
    obj.origin_alt, obj.origin_lat, obj.origin_lon = NandLua.readPositionOrigin()
    obj.rotation = NandLua.getRotationOrigin()
local blink = 0
local flag = 0
local cnt = 0
--функция цикличной проверки имеющихся значений
WaitNewPoint= Timer.new(1, function ()
    if (flag==1) then
        obj.rotation = NandLua.getRotationOrigin()--получение значения угла поворота
        obj.origin_alt, obj.origin_lat, obj.origin_lon = NandLua.readPositionOrigin()--получение значения стартовой точки
        if(obj.origin_alt == 10.0) then --если значение совпадает с введенным значением в CURSED_GUI
            cnt=cnt+1
            Color.setLEDs(0, 1, 0, cnt%5)--зажигать светодиоды
            ap.setGpsOrigin(NandLua.readPositionOrigin())
        else
            Color.setAllLEDs(tblUnpack(Color.colors.black))
        end

        if (cnt%4 == 0) then
            Color.setAllLEDs(tblUnpack(Color.colors.black))
        end
            
    end
end)


--функция для цикличной проверки тумблера номер 6.
timerControler = Timer.new(1, function ()
    ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8 = rc()
    if (flag==0) then
        if(ch6==0.0) then
            WaitNewPoint:start() --если тублер номер 6 опущен, запустить функцию ожидания Новой стартовой точки и угла поворота анимации
            Color.setAllLEDs(tblUnpack(Color.colors.red))
            flag=1
        else
            Color.setAllLEDs(tblUnpack(Color.colors.white))   
        end 
    end
end)

timerControler:start();
