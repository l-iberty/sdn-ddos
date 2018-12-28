package net.floodlightcontroller.mactracker;

import net.floodlightcontroller.core.FloodlightContext;
import net.floodlightcontroller.core.IFloodlightProviderService;
import net.floodlightcontroller.core.IOFMessageListener;
import net.floodlightcontroller.core.IOFSwitch;
import net.floodlightcontroller.core.module.FloodlightModuleContext;
import net.floodlightcontroller.core.module.FloodlightModuleException;
import net.floodlightcontroller.core.module.IFloodlightModule;
import net.floodlightcontroller.core.module.IFloodlightService;
import net.floodlightcontroller.packet.Ethernet;
import org.projectfloodlight.openflow.protocol.OFMessage;
import org.projectfloodlight.openflow.protocol.OFType;
import org.projectfloodlight.openflow.util.HexString;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.*;
import java.util.concurrent.ConcurrentSkipListSet;

public class MacTracker implements
        IOFMessageListener, IFloodlightModule, IFloodlightService {
    private IFloodlightProviderService floodlightProvider;
    private Set<Long> macAddress;
    private Logger logger;

    @Override
    public String getName() {
        return MacTracker.class.getSimpleName();
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
        collection.add(IFloodlightService.class);
        return collection;
    }

    @Override
    public Map<Class<? extends IFloodlightService>, IFloodlightService> getServiceImpls() {
        Map<Class<? extends IFloodlightService>, IFloodlightService> map = new HashMap<>();
        map.put(IFloodlightService.class, this);
        return map;
    }

    @Override
    public Collection<Class<? extends IFloodlightService>> getModuleDependencies() {
        Collection<Class<? extends IFloodlightService>> collection = new ArrayList<>();
        collection.add(IFloodlightProviderService.class);
        return collection;
    }

    @Override
    public void init(FloodlightModuleContext context) throws FloodlightModuleException {
        floodlightProvider = context.getServiceImpl(IFloodlightProviderService.class);
        macAddress = new ConcurrentSkipListSet();
        logger = LoggerFactory.getLogger(MacTracker.class);
    }

    @Override
    public void startUp(FloodlightModuleContext context) {
        floodlightProvider.addOFMessageListener(OFType.PACKET_IN, this);
    }

    @Override
    public Command receive(IOFSwitch sw, OFMessage msg, FloodlightContext context) {
        Ethernet eth = IFloodlightProviderService.bcStore.get(context,
                IFloodlightProviderService.CONTEXT_PI_PAYLOAD);

        Long sourceMacHash = Ethernet.toLong(eth.getSourceMACAddress().getBytes());
        if (!macAddress.contains(sourceMacHash)) {
            macAddress.add(sourceMacHash);
            logger.info("MAC Address: " + HexString.toHexString(sourceMacHash) +
                    " seen on switch " + sw.getId());
        }
        return Command.CONTINUE;
    }
}
