
package net.floodlightcontroller.core.web.serializers;

import java.io.IOException;

import org.projectfloodlight.openflow.protocol.OFPacketIn;
import org.projectfloodlight.openflow.protocol.OFType;

import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonSerializer;
import com.fasterxml.jackson.databind.SerializerProvider;

import net.floodlightcontroller.core.types.SwitchMessagePair;
import net.floodlightcontroller.util.OFMessageUtils;

public class SwitchMessagePairSerializer extends JsonSerializer<SwitchMessagePair> {

    @Override
    public void serialize(SwitchMessagePair m, JsonGenerator jGen, SerializerProvider arg2)
            throws IOException, JsonProcessingException {
        /*jGen.writeStartObject();

        jGen.writeFieldName("message");
        jGen.writeStartObject();
        if (m.getMessage().getType() == OFType.PACKET_IN) {
            jGen.writeNumberField("bufferId", ((OFPacketIn) m.getMessage()).getBufferId().getInt());
            jGen.writeNumberField("inPort", OFMessageUtils.getInPort((OFPacketIn) m.getMessage()).getPortNumber());
            jGen.writeNumberField("packetDataLength", ((OFPacketIn) m.getMessage()).getData().length);
            jGen.writeBinaryField("packetData", ((OFPacketIn) m.getMessage()).getData());
            jGen.writeStringField("reason", ((OFPacketIn) m.getMessage()).getReason().toString());
            jGen.writeNumberField("totalLength", ((OFPacketIn) m.getMessage()).getTotalLen());
        }
        jGen.writeStringField("type", m.getMessage().getType().toString());
        jGen.writeStringField("version", m.getMessage().getVersion().toString());
        jGen.writeNumberField("xid", m.getMessage().getXid());
        jGen.writeEndObject();*/

        jGen.writeFieldName("switch");
        jGen.writeStartObject();
        jGen.writeStringField("dpid", m.getSwitch().getId().toString());
        jGen.writeEndObject();

        //jGen.writeEndObject();
    }
}