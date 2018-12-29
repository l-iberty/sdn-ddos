package net.floodlightcontroller.pktinhistory;

import net.floodlightcontroller.core.FloodlightContext;
import net.floodlightcontroller.core.IFloodlightProviderService;
import net.floodlightcontroller.core.IOFMessageListener;
import net.floodlightcontroller.core.IOFSwitch;
import net.floodlightcontroller.core.module.FloodlightModuleContext;
import net.floodlightcontroller.core.module.FloodlightModuleException;
import net.floodlightcontroller.core.module.IFloodlightModule;
import net.floodlightcontroller.core.module.IFloodlightService;
import net.floodlightcontroller.core.types.SwitchMessagePair;
import net.floodlightcontroller.restserver.IRestApiService;
import net.floodlightcontroller.util.ConcurrentCircularBuffer;
import org.projectfloodlight.openflow.protocol.OFMessage;
import org.projectfloodlight.openflow.protocol.OFType;

import java.util.*;

public class PktInHistory implements
        IOFMessageListener, IFloodlightModule, IPktHistoryService {
    private static final int BUFFSIZE = 100;
    private IFloodlightProviderService floodlightProvider;
    private ConcurrentCircularBuffer<SwitchMessagePair> buffer;
    private IRestApiService restApi;

    @Override
    public String getName() {
        return "PktInHistory";
    }

    @Override
    public boolean isCallbackOrderingPrereq(OFType type, String name) {
        return false;
    }

    @Override
    public boolean isCallbackOrderingPostreq(OFType type, String name) {
        return false;
    }

    @Override
    public Collection<Class<? extends IFloodlightService>> getModuleServices() {
        Collection<Class<? extends IFloodlightService>> collection = new ArrayList<>();
        collection.add(IPktHistoryService.class);
        return collection;
    }

    @Override
    public Map<Class<? extends IFloodlightService>, IFloodlightService> getServiceImpls() {
        Map<Class<? extends IFloodlightService>, IFloodlightService> map = new HashMap<>();
        map.put(IPktHistoryService.class, this);
        return map;
    }

    @Override
    public Collection<Class<? extends IFloodlightService>> getModuleDependencies() {
        Collection<Class<? extends IFloodlightService>> collection = new ArrayList<>();
        collection.add(IFloodlightProviderService.class);
        collection.add(IRestApiService.class);
        return collection;
    }

    @Override
    public void init(FloodlightModuleContext context) throws FloodlightModuleException {
        floodlightProvider = context.getServiceImpl(IFloodlightProviderService.class);
        buffer = new ConcurrentCircularBuffer<>(SwitchMessagePair.class, BUFFSIZE);
        restApi = context.getServiceImpl(IRestApiService.class);
    }

    @Override
    public void startUp(FloodlightModuleContext context) {
        floodlightProvider.addOFMessageListener(OFType.PACKET_IN, this);
        restApi.addRestletRoutable(new PktInHistoryWebRoutable());
    }

    @Override
    public Command receive(IOFSwitch sw, OFMessage msg, FloodlightContext context) {
        switch (msg.getType()) {
            case PACKET_IN:
                buffer.add(new SwitchMessagePair(sw, msg));
                break;
            default:
                break;
        }

        return Command.CONTINUE;
    }

    @Override
    public ConcurrentCircularBuffer<SwitchMessagePair> getBuffer() {
        return buffer;
    }
}
