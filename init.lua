if adc.force_init_mode(adc.INIT_ADC)
then
  node.restart()
  return -- don't bother continuing, the restart is scheduled
end

led1 = 3 --D3 port controlled
led2 = 1
sensor = 2
gpio.mode(led1, gpio.OUTPUT) --set gpio as OUTPUT\
gpio.mode(led2, gpio.OUTPUT) --set gpio as OUTPUT
gpio.mode(sensor, gpio.INPUT) 

wifi.setmode(wifi.STATION) --set as station mode
wifi.sta.config("Demian", "ustb1994") --connect to wifi router
wifi.sta.connect() --give access to Internet
print(wifi.sta.getip()) --prin

tmp = 0 --adc input
mytimer = tmr.create() --create timer
mytimer:register(500, tmr.ALARM_AUTO, function()

 voltage = adc.read(tmp)
 voltage = voltage * 3.3 / 1024.0; 
 temperatureC = (voltage - 0.5) * 100
 print(temperatureC)
 tmr.stop(1) --stop timer

 print(gpio.read(sensor))
 if (gpio.read(sensor) == 1) then --make sure ip is got
     gpio.write(led2, gpio.HIGH)
 elseif (gpio.read(sensor) == 0) then
     gpio.write(led2, gpio.LOW)
     tmr.stop(1) --stop timer
 end
 
if (wifi.sta.getip() == nil) then --make sure ip is got
        print('Waiting for IP ...')
    else
        print('IP is ' .. wifi.sta.getip())
        http.request("http://39.106.107.244:8080/springTest/led/v1?id=3&email=sensor&name="..temperatureC, "GET", "", "", function(code, data) --request API
            if (code < 0) then --code should be 200 if success
                print("HTTP request failed")
            else
                print(code, data)
            end
        end)

        http.request("http://39.106.107.244:8080/springTest/user/id/1", "GET", "", "", function(code, data) --request API
            if (code < 0) then --code should be 200 if success
                print("HTTP request failed")
            else
                print(code, data)
                t = cjson.decode(data) --cjson used to convert data to lua table
                if (t["name"] == "on") then 
                    gpio.write(led1, gpio.HIGH) --if name is on, D3 high
                elseif (t["name"] == "off") then 
                    gpio.write(led1, gpio.LOW) 
                end --else D3 low
            end
        end)
        
        tmr.stop(1) --stop timer
    end
end)
mytimer:interval(500) --set interval as 200 ms
mytimer:start() 
