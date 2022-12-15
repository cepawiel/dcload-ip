dcload_protocol = Proto("dcloadip","DCLOADIP")

key_proto = ProtoField.string(dcload_protocol.name .. ".packet_tag", "Tag", base.CHAR)
address = ProtoField.uint32(dcload_protocol.name .. ".address", "Address", base.HEX)
data_size = ProtoField.uint32(dcload_protocol.name .. ".data_size", "Payload Size", base.DEC)
data = ProtoField.bytes(dcload_protocol.name .. ".data", "Payload Data", base.NONE)

version_proto = ProtoField.string(dcload_protocol.name .. ".version", "Version", base.CHAR)

dcload_protocol.fields = { key_proto, address, data_size, data, version_proto }

function dcload_protocol.dissector(buffer,pinfo,tree)
    pinfo.cols.protocol = dcload_protocol.name
    
    -- if dcload is src
    if (pinfo.src_port == 53535 or pinfo.src_port == 31313) then
        msg_type = "Reply "
    -- if dcload is dest
    elseif pinfo.dst_port == 53535 or pinfo.dst_port == 31313 then
        msg_type = "Command "
    else
        msg_type = ""
    end
    
    local subtree = tree:add(dcload_protocol,buffer(), "DCLOAD over UDP")
    -- key
    key_buf = buffer(0, 4)
    key = key_buf:string()

    -- address
    arg1_buf = buffer(4, 4) 
    arg1 = arg1_buf:uint()
    
    -- datasize
    arg2_buf = buffer(8, 4) 
    arg2 = arg2_buf:uint()

    data_start = 12
    data_len = buffer:len() - data_start

    -- Key Packet Identifier
    key_description = get_key_name(key)
    subtree:add_le(key_proto, key_buf):append_text(" (" .. key_description .. ") ")
    pinfo.cols.info:append(" \t" .. key_description .. " " .. msg_type)

    -- address
    subtree:add(address, arg1_buf)

    -- data size
    subtree:add(data_size, arg2_buf)

    if key == "EXEC" or key == "PBIN" then 
        pinfo.cols.info:append(" @0x" .. arg1_buf:bytes():tohex())
    end

    -- data
    if data_len > 0 and arg2 > 0 then
        if data_len == arg2 then
            data_pos = buffer(data_start, arg2)
            if key == "MAPL" then
                pinfo.cols.info:set("")
                
                maple_dissector = Dissector.get("maple")
                num_bytes = maple_dissector:call(data_pos:tvb(), pinfo, 
                                    subtree:add("Maple Packet", data_pos))
                
                pinfo.cols.info:fence()
            elseif key == "VERS" then
                subtree:add(version_proto, data_pos)
            end

        end
    end    
end

function get_key_name(key)
    local key_name = "Unknown"

        if key == "RBOT" then key_name = "Reboot" 
    elseif key == "VERS" then key_name = "Version"
    elseif key == "MAPL" then key_name = "Maple Packet"
    elseif key == "LBIN" then key_name = "Load Binary" 
    elseif key == "PBIN" then key_name = "Partial Binary"
    elseif key == "DBIN" then key_name = "Done Binary"
    elseif key == "SBIN" then key_name = "Send Binary"
    elseif key == "SBIQ" then key_name = "Send Binary Quiet"
    elseif key == "EXEC" then key_name = "Execute"
    elseif key == "RETV" then key_name = "Return Value"
    elseif key == "PMCR" then key_name = "Performance Counters"
    
    end

    return key_name
end

udp_table = DissectorTable.get("udp.port")
udp_table:add(53535, dcload_protocol)
udp_table:add(31313, dcload_protocol)

