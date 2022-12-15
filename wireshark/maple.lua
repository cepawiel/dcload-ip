maple_protocol = Proto("maple","MAPLE")

maple_cmd = ProtoField.int8(maple_protocol.name .. ".cmd", "Command", base.DEC)
maple_dest = ProtoField.uint8(maple_protocol.name .. ".dest", "Destination", base.HEX)
maple_src = ProtoField.uint8(maple_protocol.name .. ".src", "Source", base.HEX)
maple_count = ProtoField.uint8(maple_protocol.name .. ".word_count", "Word Count", base.DEC)

maple_devinfo_func = ProtoField.uint32(maple_protocol.name .. ".func", "Function Codes", base.HEX)
maple_devinfo_func_data = ProtoField.uint32(maple_protocol.name .. ".function_data", "Function Data", base.HEX)
maple_devinfo_area_code = ProtoField.uint8(maple_protocol.name .. ".area_code", "Area Code", base.HEX)
maple_devinfo_connector_direction = ProtoField.uint8(maple_protocol.name .. ".connector_direction", "Connector Direction", base.HEX)
maple_devinfo_product_name = ProtoField.string(maple_protocol.name .. ".product_name", "Product Name", base.CHAR)
maple_devinfo_product_license = ProtoField.string(maple_protocol.name .. ".product_license", "Product License", base.CHAR)
maple_devinfo_standby_power = ProtoField.int16(maple_protocol.name .. ".standby_power", "Standby Power", base.DEC)
maple_devinfo_max_power = ProtoField.int16(maple_protocol.name .. ".max_power", "Max Power", base.DEC)

maple_extdevinfo_verstr = ProtoField.string(maple_protocol.name .. ".version", "Version String", base.CHAR)

maple_protocol.fields = { maple_cmd, maple_dest, maple_src, maple_count,
                        maple_devinfo_func, maple_devinfo_func_data, maple_devinfo_area_code,
                        maple_devinfo_connector_direction, maple_devinfo_product_name, maple_devinfo_product_license,
                        maple_devinfo_standby_power, maple_devinfo_max_power, maple_extdevinfo_verstr }

function maple_protocol.dissector(buffer, pinfo, tree)
    pinfo.cols.protocol = maple_protocol.name

    local command = buffer(0,1):int()
    local maple_count_pos = buffer(3, 1)
   
    local maple_cmd_str = decode_maple_command(command)
    pinfo.cols.info:append(" " .. maple_cmd_str)
    tree:add("Command: " .. maple_cmd_str .. " (" .. command .. ")", buffer(0, 1))

    local dest_subtree = tree:add(maple_dest, buffer(1, 1))
    add_address_subtree(dest_subtree, buffer(1,1))
    
    local src_subtree = tree:add(maple_src, buffer(2, 1))
    add_address_subtree(src_subtree, buffer(2,1))
    
    tree:add(maple_count, maple_count_pos)

    if maple_count_pos:int() > 0 then
        local payload_tree = tree:add("Payload", buffer(4, buffer:len() - 4))
        if command == 5 or command == 6 then
            add_device_info_subtree(payload_tree, buffer(4, buffer:len() - 4))
        else
            payload_tree:add("TODO: Add Payload Decoding", buffer(4, buffer:len() - 4))
        end
    end

    pinfo.cols.info:fence()
end

function add_device_info_subtree(tree, buffer) 
    local func_tree = tree:add(maple_devinfo_func, buffer(0, 4))
    decode_function_codes(func_tree, buffer(0, 4))
    fndata_tree = tree:add("Function Data", buffer(4, 12))
    fndata_tree:add(maple_devinfo_func_data, buffer(4, 4))
    fndata_tree:add(maple_devinfo_func_data, buffer(8, 4))
    fndata_tree:add(maple_devinfo_func_data, buffer(12, 4))
    tree:add(maple_devinfo_area_code, buffer(16, 1))
    tree:add(maple_devinfo_connector_direction, buffer(17, 1))
    tree:add(maple_devinfo_product_name, buffer(18, 30))
    tree:add(maple_devinfo_product_license, buffer(48, 60))
    tree:add_le(maple_devinfo_standby_power, buffer(108, 2))
    tree:add_le(maple_devinfo_max_power, buffer(110, 2))

    -- decode version string from extended command
    local pos = 112
    if buffer:len() > pos then
        local ver_buf = buffer(pos, buffer:len() - pos)
        local ver_start = 0
        local ver_end = 0
        local version_tree = tree:add("Versions", ver_buf:tvb())
        
        -- loop through all version
        while(ver_start < ver_buf:len()) do
            ver_end = ver_start

            -- loop though all characters from _start to _end
            while(ver_end < ver_buf:len()) do
                local a = string.char(ver_buf(ver_end, 1):uint());
                ver_end = ver_end + 1
                if a == ',' then
                    break;
                end
            end

            print(ver_buf:len() .. "\t" .. ver_start .. "\t" .. ver_end)
            local substr_buf = ver_buf(ver_start, (ver_end - ver_start) - 1)
            version_tree:add(substr_buf:string(), substr_buf)
            
            print(ver_buf:len() .. "\t" .. ver_start .. "\t" .. ver_end)
            ver_start = ver_end
        end
        
    end
end

function decode_maple_command(code)
    local cmd_str = "Unknown"

        if code == 1 then cmd_str = "Request device information"
    elseif code == 2 then cmd_str = "Request extended device information"
    elseif code == 3 then cmd_str = "Reset device"
    elseif code == 4 then cmd_str = "Shutdown device"
    elseif code == 5 then cmd_str = "Device information (response)"
    elseif code == 6 then cmd_str = "Extended device information (response)"
    elseif code == 7 then cmd_str = "Command acknowledge (response)"
    elseif code == 8 then cmd_str = "Data transfer (response)"
    elseif code == 9 then cmd_str = "Get condition"
    elseif code == 10 then cmd_str = "Get memory information"
    elseif code == 11 then cmd_str = "Block read"
    elseif code == 12 then cmd_str = "Block write"
    elseif code == 14 then cmd_str = "Set condition"

    elseif code == -1 then cmd_str = "No response"
    elseif code == -2 then cmd_str = "Function code unsupported (response)"
    elseif code == -3 then cmd_str = "Unknown command (response)"
    elseif code == -4 then cmd_str = "Command needs to be sent again (response)"
    elseif code == -5 then cmd_str = "File error (response)"
    end

    return cmd_str
end

function add_address_subtree(tree, addr_buf)
    local addr = addr_buf:uint()
    local str = ""

    local port = bit.rshift(bit.band(addr, 0xC0), 6)
        if (port == 0) then str = "Port A"
    elseif (port == 1) then str = "Port B"
    elseif (port == 2) then str = "Port C"
    elseif (port == 3) then str = "Port D"
    end
    tree:add(str, addr_buf)

    if (bit.band(addr, 0x3F) == 0) then tree:add("Host", addr_buf) end
    
    if (bit.band(addr, 0x20) ~= 0) then tree:add("Main Peripheral", addr_buf) end

    if (bit.band(addr, 0x01) ~= 0) then tree:add("Sub Peripheral 1", addr_buf) end
    if (bit.band(addr, 0x02) ~= 0) then tree:add("Sub Peripheral 2", addr_buf) end
    if (bit.band(addr, 0x04) ~= 0) then tree:add("Sub Peripheral 3", addr_buf) end
    if (bit.band(addr, 0x08) ~= 0) then tree:add("Sub Peripheral 4", addr_buf) end
    if (bit.band(addr, 0x10) ~= 0) then tree:add("Sub Peripheral 5", addr_buf) end
end

function decode_function_codes(tree, code_buf)
    local code = code_buf:uint()
    if (bit.band(code, 0x001) ~= 0) then tree:add("Controller", code_buf) end
    if (bit.band(code, 0x002) ~= 0) then tree:add("Memory Card", code_buf) end
    if (bit.band(code, 0x004) ~= 0) then tree:add("LCD Display", code_buf) end
    if (bit.band(code, 0x008) ~= 0) then tree:add("Clock", code_buf) end
    if (bit.band(code, 0x010) ~= 0) then tree:add("Microphone", code_buf) end
    if (bit.band(code, 0x020) ~= 0) then tree:add("AR-Gun", code_buf) end
    if (bit.band(code, 0x040) ~= 0) then tree:add("Keyboard", code_buf) end
    if (bit.band(code, 0x080) ~= 0) then tree:add("Light Gun", code_buf) end
    if (bit.band(code, 0x100) ~= 0) then tree:add("Puru-Puru Pack", code_buf) end
    if (bit.band(code, 0x200) ~= 0) then tree:add("Mouse", code_buf) end
end