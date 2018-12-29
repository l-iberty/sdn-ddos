package net.floodlightcontroller.pktinhistory;

import net.floodlightcontroller.core.types.SwitchMessagePair;
import org.restlet.resource.Get;
import org.restlet.resource.ServerResource;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class PktInHistoryResource extends ServerResource {
    @Get("json")
    public List<SwitchMessagePair> retrieve() {
        IPktHistoryService service = (IPktHistoryService)
                getContext().getAttributes().get(IPktHistoryService.class.getCanonicalName());

        List<SwitchMessagePair> list = new ArrayList<>();
        list.addAll(Arrays.asList(service.getBuffer().snapshot()));
        return list;
    }
}
